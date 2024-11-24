#include "../include/operators_and_selection.h"
#include "../include/evaluation_functions.h"

#include <vector>
#include <random>


std::mt19937 rng(std::random_device{}());
std::vector<Float_representation> crossover(const Float_representation& p1, const Float_representation& p2, double r_cross) {
    std::uniform_real_distribution<> dis(0.0, 1.0); // Dystrybucja dla r_cross i alpha

    Float_representation c1 = p1; // Potomek 1
    Float_representation c2 = p2; // Potomek 2

    // Sprawdź, czy krzyżowanie się odbywa
    if (dis(rng) < r_cross) {
        for (int i = 0; i < p1.GetPoint().size(); ++i) {
            double alpha = dis(rng); // Współczynnik liniowej kombinacji
            double x_a = p1.GetPoint(i);
            double x_b = p2.GetPoint(i);

            // Oblicz geny dla dzieci
            c1.SetPoint(i, alpha * x_a + (1 - alpha) * x_b);
            c2.SetPoint(i, alpha * x_b + (1 - alpha) * x_a);
        }
    }

    return {c1, c2};
}

void mutate(Float_representation& individual, double min_value, double max_value, double mutation_rate, int dim) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::normal_distribution<double> normal_dis(0, 1);
    double sigma = 1.0 - (double)function_call_count / (10000 * dim);

    for (int i = 0; i < individual.GetDimensions(); ++i) {
        if (dist(rng) < 0.15) {
            double new_single_coordinate = individual.GetPoint(i) + normal_dis(rng) * sigma;

            // Adaptive boundary handling (clamping)
            new_single_coordinate = std::max(min_value, std::min(max_value, new_single_coordinate));

            // Random jump mutation (10% chance)
            if (dist(rng) < 0.1) {
                new_single_coordinate = min_value + dist(rng) * (max_value - min_value);
            }

            // Set new value
            individual.SetPoint(i, new_single_coordinate);
        }
    }
}


Float_representation tournamentSelection(std::vector<Float_representation>& population, int tournament_size, const std::vector<double>& eval_values) {
    int POP_SIZE = population.size();


    std::uniform_int_distribution<int> dist(0, POP_SIZE - 1);
    int idx = dist(rng);
    Float_representation best_individual = population[idx];
    double best_fitness = eval_values[idx];

    for (int i = 1; i < tournament_size; ++i) {
        idx = dist(rng);
        double fitness = eval_values[idx];

        if (fitness < best_fitness) {
            best_individual = population[idx];
            best_fitness = fitness;
        }
    }

    return best_individual;
}