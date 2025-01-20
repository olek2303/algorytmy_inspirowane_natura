#include "../include/spea2.h"

std::vector<Individual> initializePopulation(int populationSize, std::mt19937& rng, Evaluator* evaluate) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::vector<Individual> population(populationSize);
    for (auto& ind : population) {
        ind.variables.resize(evaluate->numVariables);
        for (auto& var : ind.variables) {
            var = dist(rng);
        }
        ind.objectives = evaluate->evaluate(ind.variables);
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

Individual crossover(const Individual& parent1, const Individual& parent2, std::mt19937& rng, Evaluator* evaluate) {
    Individual child;
    child.variables.resize(parent1.variables.size());

    for (size_t i = 0; i < parent1.variables.size(); ++i) {
        double minVal = std::min(parent1.variables[i], parent2.variables[i]);
        double maxVal = std::max(parent1.variables[i], parent2.variables[i]);

        double range = maxVal - minVal;
        double lowerBound = std::max(0.0, minVal - crossoverStrength * range);
        double upperBound = std::min(1.0, maxVal + crossoverStrength * range);

        child.variables[i] = std::uniform_real_distribution<double>(lowerBound, upperBound)(rng);
        child.variables[i] = clamp(child.variables[i], 0.0, 1.0);
    }

    child.objectives = evaluate->evaluate(child.variables);
    return child;
}

void mutate(Individual& individual, std::mt19937& rng, Evaluator* evaluate) {
    std::uniform_real_distribution<double> uniformDist(0.0, 1.0);
    std::normal_distribution<double> mutationDist(0.0, mutationStrength);

    for (auto& var : individual.variables) {
        if (uniformDist(rng) < mutationRate) {
            var += mutationDist(rng);
            var = clamp(var, 0.0, 1.0);
        }
    }

    individual.objectives = evaluate->evaluate(individual.variables);
}

void spea2(std::mt19937 rng, Evaluator* evaluate) {
    //initialize population and empty archive
    std::vector<Individual> population = initializePopulation(populationSize, rng, evaluate);
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
                offspring.push_back(crossover(parents[i], parents[i + 1], rng, evaluate));
            }
        }

        //muatuion
        for (auto& child : offspring) {
            mutate(child, rng, evaluate);
        }

        //update population
        population = offspring;

        if (generation == 19 || generation == 49 || generation == 99 || generation == 499) {
            std::string filename = "archive_" + evaluate->name + "_" + std::to_string(evaluate->numVariables)
                    + "_" + std::to_string(generation + 1) + ".txt";
            writeArchive(archive, filename);
            filename = "pareto_" + evaluate->name + "_" + std::to_string(evaluate->numVariables)
                    + "_" + std::to_string(generation + 1) + ".txt";
            writeParetoFront(archive, filename);
        }
    }
}
