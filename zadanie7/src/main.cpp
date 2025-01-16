#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <random>

struct Individual {
    std::vector<double> variables;
    std::vector<double> objectives;
    double fitness;
};

std::vector<double> evaluateZDT1(const std::vector<double>& variables) {
    double f1 = variables[0];
    double g = 1.0 + 9.0 * std::accumulate(variables.begin() + 1, variables.end(), 0.0) / (variables.size() - 1);
    double h = 1.0 - std::sqrt(f1 / g);
    double f2 = g * h;
    return {f1, f2};
}

std::vector<Individual> initializePopulation(int populationSize, int numVariables, std::mt19937& rng) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::vector<Individual> population(populationSize);
    for (auto& ind : population) {
        ind.variables.resize(numVariables);
        for (auto& var : ind.variables) {
            var = dist(rng);
        }
        ind.objectives = evaluateZDT1(ind.variables);
    }
    return population;
}

void calculateFitness(std::vector<Individual>& population) {
    for (auto& ind : population) {
        ind.fitness = 0.0;
        for (const auto& other : population) {
            if (other.objectives[0] <= ind.objectives[0] && other.objectives[1] <= ind.objectives[1] && (other.objectives != ind.objectives)) {
                ind.fitness += 1.0;
            }
        }
    }
}

std::vector<Individual> tournamentSelection(const std::vector<Individual>& population, int size, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, population.size() - 1);
    std::vector<Individual> selected;
    for (int i = 0; i < size; ++i) {
        int a = dist(rng);
        int b = dist(rng);
        if (population[a].fitness < population[b].fitness) {
            selected.push_back(population[a]);
        } else {
            selected.push_back(population[b]);
        }
    }
    return selected;
}

Individual crossover(const Individual& parent1, const Individual& parent2, std::mt19937& rng) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    Individual child;
    child.variables.resize(parent1.variables.size());
    for (size_t i = 0; i < parent1.variables.size(); ++i) {
        child.variables[i] = 0.5 * (parent1.variables[i] + parent2.variables[i]);
    }
    child.objectives = evaluateZDT1(child.variables);
    return child;
}

void mutate(Individual& individual, double mutationRate, std::mt19937& rng) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (auto& var : individual.variables) {
        if (dist(rng) < mutationRate) {
            var = dist(rng);
        }
    }
    individual.objectives = evaluateZDT1(individual.variables);
}

void writeParetoFront(const std::vector<Individual>& population, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& ind : population) {
        if (ind.fitness == 0.0) { // Pareto-optimal front
            file << ind.objectives[0] << " " << ind.objectives[1] << "\n";
        }
    }
}

int main() {
    const int populationSize = 500;
    const int numGenerations = 250;
    const int numVariables = 2;
    const double mutationRate = 0.4;

    std::random_device rd;
    std::mt19937 rng(rd());

    // Initialize population
    auto population = initializePopulation(populationSize, numVariables, rng);

    for (int generation = 0; generation < numGenerations; ++generation) {
        // Evaluate fitness
        calculateFitness(population);

        // Select parents
        auto parents = tournamentSelection(population, populationSize, rng);

        // Create offspring
        std::vector<Individual> offspring;
        for (size_t i = 0; i < parents.size(); i += 2) {
            if (i + 1 < parents.size()) {
                offspring.push_back(crossover(parents[i], parents[i + 1], rng));
            }
        }

        // Mutate offspring
        for (auto& child : offspring) {
            mutate(child, mutationRate, rng);
        }

        // Combine population and offspring
        population.insert(population.end(), offspring.begin(), offspring.end());

        // Evaluate fitness again
        calculateFitness(population);

        // Select next generation
        std::sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
            return a.fitness < b.fitness;
        });
        population.resize(populationSize);
    }

    // Write Pareto front to file
    writeParetoFront(population, "pareto_front.txt");

    std::cout << "Optimization completed. Pareto front written to pareto_front.txt" << std::endl;

    return 0;
}
