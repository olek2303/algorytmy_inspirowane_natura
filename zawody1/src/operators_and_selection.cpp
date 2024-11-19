#include "../include/operators_and_selection.h"
#include "../include/evaluation_functions.h"

#include <vector>
#include <random>


std::mt19937 rng(std::random_device{}());
std::vector<Float_representation> crossover(const Float_representation& p1, const Float_representation& p2, double r_cross) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> dis_int(1, p1.GetPoint().size() - 2);

    Float_representation c1 = p1;
    Float_representation c2 = p2;

    if (dis(rng) < r_cross) {
        int pt = dis_int(rng);

        for (int i = pt; i < p1.GetPoint().size(); ++i) {
            c1.SetPoint(i,p2.GetPoint()[i]);
            c2.SetPoint(i, p1.GetPoint()[i]);
        }
    }

    return {c1, c2};
}

void mutate(Float_representation& individual, double min_value, double max_value) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::normal_distribution<double> normal_dis(0,1);
    for (int i = 0; i < individual.GetDimensions(); ++i) {
        if (dist(rng) < 0.5) {
            //new single coordinate value (add normal distribution(0,1)
            double new_single_coordinate = individual.GetPoint(i) + normal_dis(rng);
            //check if new value is in range
            if (new_single_coordinate < min_value) {
                new_single_coordinate = min_value;
            } else if (new_single_coordinate > max_value) {
                new_single_coordinate = max_value;
            }
            //set new value
            individual.SetPoint(i, new_single_coordinate);
        }
    }
}

Float_representation tournamentSelection(const std::vector<Float_representation>& population, int eval_function, int tournament_size) {
    int POP_SIZE = population.size();

    std::uniform_int_distribution<int> dist(0, POP_SIZE - 1);
    Float_representation best_individual = population[dist(rng)];
    double best_fitness = evaluate(best_individual, eval_function);

    for (int i = 1; i < tournament_size; ++i) {
        const Float_representation& individual = population[dist(rng)];
        double fitness = evaluate(individual, eval_function);
        if (fitness < best_fitness) {
            best_individual = individual;
            best_fitness = fitness;
        }
    }

    return best_individual;
}