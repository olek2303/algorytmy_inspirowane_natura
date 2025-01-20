#include "../include/spea2.h"

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

void calculateStrength(std::vector<Individual>& population) {
    for (auto& ind : population) {
        ind.strength = 0;
        for (const auto& other : population) {

            double f1_ind = ind.objectives[0];
            double f2_ind = ind.objectives[1];
            double f1_other = other.objectives[0];
            double f2_other = other.objectives[1];

            if (f1_other <= f1_ind && f2_other <= f2_ind && (f1_other != f1_ind || f2_other != f2_ind)) {
                ind.strength += 1.0;
            }
        }
    }
}

void calculateDensity(std::vector<Individual>& population, int n_neighbours) {
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
        int k = std::min(static_cast<int>(distances.size()), n_neighbours);
        ind.density = 1.0 / (distances[k - 1] + 2.0);
    }
}

void calculateFitness(std::vector<Individual>& population) {
    calculateStrength(population);
    calculateDensity(population, densityNeighbours);
    for (auto& ind : population) {
        ind.fitness = ind.strength + ind.density;
    }
}

std::vector<Individual> tournamentSelection(const std::vector<Individual>& population, int tournamentSize, int selectionSize, std::mt19937& rng) {
    std::vector<Individual> selected;
    std::uniform_int_distribution<int> dist(0, population.size() - 1);

    for (int i = 0; i < selectionSize; ++i) {
        std::vector<const Individual*> tournament;

        for (int j = 0; j < tournamentSize; ++j) {
            tournament.push_back(&population[dist(rng)]);
        }

        auto best = *std::min_element(tournament.begin(), tournament.end(),
            [](const Individual* a, const Individual* b) {
                return a->fitness < b->fitness;
            });

        selected.push_back(*best);
    }
    return selected;
}

Individual crossover(const Individual& parent1, const Individual& parent2, std::mt19937& rng, double alpha) {
    Individual child;
    child.variables.resize(parent1.variables.size());

    for (size_t i = 0; i < parent1.variables.size(); ++i) {
        double minVal = std::min(parent1.variables[i], parent2.variables[i]);
        double maxVal = std::max(parent1.variables[i], parent2.variables[i]);

        double range = maxVal - minVal;
        double lowerBound = std::max(0.0, minVal - alpha * range);
        double upperBound = std::min(1.0, maxVal + alpha * range);

        child.variables[i] = std::uniform_real_distribution<double>(lowerBound, upperBound)(rng);
        child.variables[i] = clamp(child.variables[i], 0.0, 1.0);
    }

    child.objectives = evaluateZDT1(child.variables);
    return child;
}

void mutate(Individual& individual, double mutationRate, double mutationStrength, std::mt19937& rng) {
    std::uniform_real_distribution<double> uniformDist(0.0, 1.0);
    std::normal_distribution<double> mutationDist(0.0, mutationStrength);

    for (auto& var : individual.variables) {
        if (uniformDist(rng) < mutationRate) {
            var += mutationDist(rng);
            var = clamp(var, 0.0, 1.0);
        }
    }

    individual.objectives = evaluateZDT1(individual.variables);
}

std::vector<Individual> spea2 (std::vector<Individual> population, std::mt19937 rng) {
    //initialize population and empty archive
    std::vector<Individual> archive;

    for (int generation = 0; generation < numGenerations; ++generation) {
        //combine population and archive
        std::vector<Individual> combinedPopulation = population;
        combinedPopulation.insert(combinedPopulation.end(), archive.begin(), archive.end());

        //evaluate fitness
        calculateFitness(combinedPopulation);

        //sort by fitness
        std::sort(combinedPopulation.begin(), combinedPopulation.end(), [](const Individual& a, const Individual& b) {
            return a.fitness < b.fitness;
        });

        //update archive with the best individuals from old archive and new population
        archive.clear();
        for (int i = 0; i < archiveSize && i < combinedPopulation.size(); ++i) {
            archive.push_back(combinedPopulation[i]);
        }

        //tournament selection
        auto parents = tournamentSelection(combinedPopulation, tournamentSize ,populationSize, rng);

        //crossover
        std::vector<Individual> offspring;
        for (size_t i = 0; i < parents.size(); i += 2) {
            if (i + 1 < parents.size()) {
                offspring.push_back(crossover(parents[i], parents[i + 1], rng, crossoverStrength));
            }
        }

        //muatuion
        for (auto& child : offspring) {
            mutate(child, mutationRate, mutationStrength, rng);
        }

        //update population
        population = offspring;

        if (generation == 19 || generation == 49 || generation == 99 || generation == 499) {
            std::string filename = "archive_" + std::to_string(generation + 1) + ".txt";
            writeArchive(archive, filename);
            filename = "pareto_" + std::to_string(generation + 1) + ".txt";
            writeParetoFront(archive, filename);
        }
    }
    return archive;
}
