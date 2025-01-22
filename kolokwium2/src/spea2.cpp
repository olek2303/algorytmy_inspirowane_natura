#include "../include/spea2.h"

std::vector<Individual> initializePopulation(int populationSize, std::mt19937& rng, Evaluator* evaluate) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::uniform_real_distribution<double> distZDT4(-5.0, 5.0);

    std::vector<Individual> population(populationSize);
    for (auto& ind : population) {
        ind.variables.resize(evaluate->numVariables);
        int counter = 0;
        for (auto& var : ind.variables) {
            if(evaluate->name == "ZDT4" && counter > 0) {
                var = distZDT4(rng);
            }
            else {
                var = dist(rng);
            }
            counter++;
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

        int k = static_cast<int>(std::sqrt(population.size()));

        k = std::min(k, static_cast<int>(distances.size()));

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
        double lowerBound;
        double upperBound;
        if(evaluate->name == "ZDT4" && i > 0) {
            lowerBound = std::max(-5.0, minVal - crossoverStrength * range);
            upperBound = std::min(5.0, maxVal + crossoverStrength * range);
        }
        else {
            lowerBound = std::max(0.0, minVal - crossoverStrength * range);
            upperBound = std::min(1.0, maxVal + crossoverStrength * range);
        }


        child.variables[i] = std::uniform_real_distribution<double>(lowerBound, upperBound)(rng);
        if(evaluate->name == "ZDT4" && i > 0) {
            child.variables[i] = clamp(child.variables[i], -5.0, 5.0);
        }
        else {
            child.variables[i] = clamp(child.variables[i], 0.0, 1.0);
        }
    }

    child.objectives = evaluate->evaluate(child.variables);
    return child;
}

void mutate(Individual& individual, std::mt19937& rng, Evaluator* evaluate) {
    std::uniform_real_distribution<double> uniformDist(0.0, 1.0);
    std::normal_distribution<double> mutationDist(0.0, mutationStrength);
    bool mutated = false;
    int counter = 0;
    for (auto& var : individual.variables) {
        if (uniformDist(rng) < mutationRate) {
            var += mutationDist(rng);
            if(evaluate->name == "ZDT4" && counter > 0) {
                var = clamp(var, -5.0, 5.0);
            }
            else {
                var = clamp(var, 0.0, 1.0);
            }
            mutated = true;
        }
        counter++;
    }
    if (mutated) {
        individual.objectives = evaluate->evaluate(individual.variables);
    }
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

        //update archive
        archive.clear();
        for (const auto& ind : combinedPopulation) {
            if (ind.strength == 0) {
                archive.push_back(ind);
            }
        }

        if (archive.size() < archiveSize) {
            std::sort(combinedPopulation.begin(), combinedPopulation.end(), [](const Individual& a, const Individual& b) {
                return a.fitness < b.fitness;
            });

            for (const auto& ind : combinedPopulation) {
                if (archive.size() >= archiveSize) break;  // Przerywamy, gdy archiwum jest pełne
                if (ind.strength > 0) {
                    archive.push_back(ind);
                }
            }
        }

        if (archive.size() > archiveSize) {
            while (archive.size() > archiveSize) {
                std::vector<std::vector<double>> distances(archive.size(), std::vector<double>(archive.size(), 0.0));
                for (size_t i = 0; i < archive.size(); ++i) {
                    for (size_t j = i + 1; j < archive.size(); ++j) {
                        double distance = 0.0;
                        for (size_t k = 0; k < archive[i].objectives.size(); ++k) {
                            distance += std::pow(archive[i].objectives[k] - archive[j].objectives[k], 2);
                        }
                        distances[i][j] = distances[j][i] = std::sqrt(distance);
                    }
                }

                double minDistance = std::numeric_limits<double>::infinity();
                size_t idxToRemove = 0;
                for (size_t i = 0; i < archive.size(); ++i) {
                    for (size_t j = i + 1; j < archive.size(); ++j) {
                        if (distances[i][j] < minDistance) {
                            minDistance = distances[i][j];
                            idxToRemove = i;
                        }
                    }
                }
                archive.erase(archive.begin() + idxToRemove);
            }
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
