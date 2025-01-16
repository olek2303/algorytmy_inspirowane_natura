#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <random>
#include <numeric>

struct Individual {
    std::vector<double> variables;
    std::vector<double> objectives;
    double fitness;
    double strength;
    double density;
};

double clamp(double value, double min, double max) {
    return std::max(min, std::min(value, max));
}

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

double roundTo(double value, int precision) {
    double scale = std::pow(10.0, precision);
    return std::round(value * scale) / scale;
}

// W zaktualizowanym warunku
void calculateStrength(std::vector<Individual>& population) {
    for (auto& ind : population) {
        ind.strength = 0;
        for (const auto& other : population) {
            // Zaokrąglamy wartości f1 i f2 do 3 miejsc po przecinku (możesz zmienić liczbę miejsc)
            double f1_ind = roundTo(ind.objectives[0], 3);
            double f2_ind = roundTo(ind.objectives[1], 3);
            double f1_other = roundTo(other.objectives[0], 3);
            double f2_other = roundTo(other.objectives[1], 3);

            if (f1_other <= f1_ind && f2_other <= f2_ind && (f1_other != f1_ind || f2_other != f2_ind)) {
                ind.strength += 1.0;
            }
        }
    }
}

// Calculate density (distance to neighbors in objective space)
void calculateDensity(std::vector<Individual>& population) {
    for (auto& ind : population) {
        std::vector<double> distances;
        for (const auto& other : population) {
            if (&ind != &other) {
                double distance = 0.0;
                for (size_t i = 0; i < ind.objectives.size(); ++i) {
                    distance += std::pow(ind.objectives[i] - other.objectives[i], 2);
                }
                distances.push_back(std::sqrt(distance));
            }
        }
        std::sort(distances.begin(), distances.end());
        int k = std::min(static_cast<int>(distances.size()), 6); // Use the 5th nearest neighbor
        ind.density = 1.0 / (distances[k - 1] + 2.0); // Avoid division by zero
    }
}

// Calculate fitness combining strength and density
void calculateFitness(std::vector<Individual>& population) {
    calculateStrength(population);
    calculateDensity(population);
    for (auto& ind : population) {
        ind.fitness = ind.strength + ind.density;
    }
}

// Tournament selection
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

// Crossover operation
Individual crossover(const Individual& parent1, const Individual& parent2, std::mt19937& rng) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    Individual child;
    child.variables.resize(parent1.variables.size());
    for (size_t i = 0; i < parent1.variables.size(); ++i) {
        child.variables[i] = 0.5 * (parent1.variables[i] + parent2.variables[i]);
        // Ensure variable stays within [0, 1]
        child.variables[i] = clamp(child.variables[i], 0.0, 1.0);
    }
    child.objectives = evaluateZDT1(child.variables);
    return child;
}

// Mutation operation
void mutate(Individual& individual, double mutationRate, std::mt19937& rng) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (auto& var : individual.variables) {
        if (dist(rng) < mutationRate) {
            var = dist(rng);
        }
        // Ensure variable stays within [0, 1]
        var = clamp(var, 0.0, 1.0);
    }
    individual.objectives = evaluateZDT1(individual.variables);
}

// Write Pareto front to file
void writeParetoFront(const std::vector<Individual>& population, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& ind : population) {
        file << ind.objectives[0] << " " << ind.objectives[1] << "\n";
    }
}

int main() {
    const int populationSize = 300;
    const int numGenerations = 300;
    const int numVariables = 2;
    const double mutationRate = 0.4;
    const int archiveSize = 300;

    std::random_device rd;
    std::mt19937 rng(rd());

    // Initialize population
    auto population = initializePopulation(populationSize, numVariables, rng);
    std::vector<Individual> archive;

    for (int generation = 0; generation < numGenerations; ++generation) {
        // Combine population and archive
        std::vector<Individual> combinedPopulation = population;
        combinedPopulation.insert(combinedPopulation.end(), archive.begin(), archive.end());

        // Evaluate fitness of the combined population
        calculateFitness(combinedPopulation);

        // Sort population based on fitness
        std::sort(combinedPopulation.begin(), combinedPopulation.end(), [](const Individual& a, const Individual& b) {
            return a.fitness < b.fitness;
        });

        // Update archive: Select the best individuals
        archive.clear();
        for (int i = 0; i < archiveSize && i < combinedPopulation.size(); ++i) {
            archive.push_back(combinedPopulation[i]);
        }

        // Select parents from the population and archive
        auto parents = tournamentSelection(combinedPopulation, populationSize, rng);

        // Create offspring through crossover
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

        // Replace the population with the offspring
        population = offspring;
    }

    // Write Pareto front to file
    writeParetoFront(archive, "pareto_front.txt");

    std::cout << "Optimization completed. Pareto front written to pareto_front.txt" << std::endl;

    return 0;
}
