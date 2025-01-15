#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <fstream>

struct Point {
    double f1, f2;

    bool operator==(const Point &other) const {
        return std::fabs(f1 - other.f1) < 1e-6 && std::fabs(f2 - other.f2) < 1e-6;
    }
};

struct Individual {
    std::vector<double> vars; // Decision variables
    Point fitness;            // Fitness values
    double strength;          // Strength value
    double rawFitness;        // Raw fitness value
    double density;           // Density estimate

    bool operator!=(const Individual &other) const {
        return vars != other.vars;
    }
};

const int POP_SIZE = 500;
const int ARCHIVE_SIZE = 500;
const int MAX_GEN = 200;
const int NUM_VARS = 30;
const double CROSSOVER_RATE = 0.9;
//tak było wcześneij
//const double MUTATION_RATE = 1.0 / NUM_VARS;
const double MUTATION_RATE = 0.1;

std::mt19937 rng(std::random_device{}());
std::uniform_real_distribution<> dist(0.0, 1.0);

// ZDT1 objectives
Point evaluateFitness(const std::vector<double> &vars) {
    double f1 = vars[0];
    double g = 1.0 + 9.0 * std::accumulate(vars.begin() + 1, vars.end(), 0.0) / (NUM_VARS - 1);
    double h = 1.0 - std::sqrt(f1 / g);
    double f2 = g * h;
    return {f1, f2};
}

// Generate random individual
Individual randomIndividual() {
    Individual ind;
    ind.vars.resize(NUM_VARS);
    for (double &var : ind.vars) {
        var = dist(rng);
    }
    ind.fitness = evaluateFitness(ind.vars);
    ind.strength = 0.0;
    ind.rawFitness = 0.0;
    ind.density = 0.0;
    return ind;
}

// Calculate distance between two individuals
double distance(const Point &a, const Point &b) {
    return std::sqrt(std::pow(a.f1 - b.f1, 2) + std::pow(a.f2 - b.f2, 2));
}

// SPEA2 fitness evaluation
void evaluateSPEA2Fitness(std::vector<Individual> &population, std::vector<Individual> &archive) {
    // Combine population and archive
    std::vector<Individual> combined = population;
    combined.insert(combined.end(), archive.begin(), archive.end());

    // Calculate strength values
    for (Individual &ind : combined) {
        ind.strength = 0.0;
        for (const Individual &other : combined) {
            if (ind.fitness.f1 <= other.fitness.f1 && ind.fitness.f2 <= other.fitness.f2 && ind != other) {
                ind.strength += 1.0;
            }
        }
    }

    // Calculate raw fitness values
    for (Individual &ind : combined) {
        ind.rawFitness = 0.0;
        for (const Individual &other : combined) {
            if (other.fitness.f1 <= ind.fitness.f1 && other.fitness.f2 <= ind.fitness.f2 && ind != other) {
                ind.rawFitness += other.strength;
            }
        }
    }

    // Calculate density values
    for (Individual &ind : combined) {
        std::vector<double> distances;
        for (const Individual &other : combined) {
            if (ind != other) {
                distances.push_back(distance(ind.fitness, other.fitness));
            }
        }
        std::sort(distances.begin(), distances.end());
        ind.density = 1.0 / (distances[std::min((size_t)std::sqrt(combined.size()), distances.size() - 1)] + 2.0);
    }

    // Assign final fitness values
    for (Individual &ind : combined) {
        ind.rawFitness += ind.density;
    }

    // Update archive with non-dominated solutions
    archive.clear();
    for (const Individual &ind : combined) {
        if (ind.rawFitness < 1.0) {
            archive.push_back(ind);
        }
    }

    // If archive exceeds size, reduce it
    if (archive.size() > ARCHIVE_SIZE) {
        std::sort(archive.begin(), archive.end(), [](const Individual &a, const Individual &b) {
            return a.rawFitness < b.rawFitness;
        });
        archive.resize(ARCHIVE_SIZE);
    }
}

// Tournament selection
Individual tournamentSelection(const std::vector<Individual> &population) {
    int i1 = dist(rng) * population.size();
    int i2 = dist(rng) * population.size();
    return population[i1].rawFitness < population[i2].rawFitness ? population[i1] : population[i2];
}

// Crossover
Individual crossover(const Individual &p1, const Individual &p2) {
    Individual child;
    child.vars.resize(NUM_VARS);
    for (int i = 0; i < NUM_VARS; ++i) {
        child.vars[i] = dist(rng) < 0.5 ? p1.vars[i] : p2.vars[i];
    }
    child.fitness = evaluateFitness(child.vars);
    return child;
}


double clamp(double value, double min, double max) {
    return std::max(min, std::min(value, max));
}
// Mutation
void mutate(Individual &ind) {
    for (double &var : ind.vars) {
        if (dist(rng) < MUTATION_RATE) {
            var += dist(rng) - 0.5;
            var = clamp(var, 0.0, 1.0);
        }
    }
    ind.fitness = evaluateFitness(ind.vars);
}

int main() {
    // Initialize population and archive
    std::vector<Individual> population(POP_SIZE);
    for (Individual &ind : population) {
        ind = randomIndividual();
    }
    std::vector<Individual> archive;

    // Evolution loop
    for (int gen = 0; gen < MAX_GEN; ++gen) {
        evaluateSPEA2Fitness(population, archive);

        // Create offspring population
        std::vector<Individual> offspring;
        while (offspring.size() < POP_SIZE) {
            Individual p1 = tournamentSelection(archive);
            Individual p2 = tournamentSelection(archive);
            Individual child = crossover(p1, p2);
            mutate(child);
            offspring.push_back(child);
        }

        population = offspring;
    }

    // Output final Pareto front
    std::ofstream outFile("pareto_front.txt");
    for (const Individual &ind : archive) {
        outFile << ind.fitness.f1 << " " << ind.fitness.f2 << "\n";
    }
    outFile.close();

    std::cout << "Pareto front saved to pareto_front.txt\n";
    return 0;
}
