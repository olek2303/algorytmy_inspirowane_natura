
#include "evolutionary_algorithm.h"
#include "Point.h"
#include "evaluation_functions.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <functions.h>
#include <numeric>

// Constants
const int BITS_PER_DIMENSION = 16;
const int MAX_ITER_EXPERIMENT = 1;
const int DIMENSIONS = 10;
const int MAX_ITER = 10000;

template <typename T>
double getFitness(T point) {
    if constexpr (std::is_same_v<T, IntPoint>) {
        // For IntPoint, use the integer coordinates as fitness
        return static_cast<double>(point.GetPoint());
    } else if constexpr (std::is_same_v<T, VectorPoint>) {
        // For VectorPoint, use the sum of coordinates as fitness
        std::vector<double> coordinates = point.GetPoint();
        double sum = std::accumulate(coordinates.begin(), coordinates.end(), 0.0);
        return sum;
    } else {
        static_assert("Unsupported type for getFitness.");
        return 0.0;
    }
}

template<typename T>
T rouletteWheelSelection(const std::vector<T>& population) {
    int n = population.size();
    std::vector<double> adjustedFitness(n);

    // Step 1: Adjusted fitness for minimization (1 / fitness)
    for (int i = 0; i < n; ++i) {
        double fitness = getFitness(population[i]);
        adjustedFitness[i] = 1.0 / fitness; // Use 1/fitness for minimization
    }

    // Step 2: Compute selection probabilities
    double totalAdjustedFitness = std::accumulate(adjustedFitness.begin(), adjustedFitness.end(), 0.0);
    std::vector<double> probabilities(n);
    for (int i = 0; i < n; ++i) {
        probabilities[i] = adjustedFitness[i] / totalAdjustedFitness;
    }

    // Step 3: Calculate cumulative probabilities
    std::vector<double> cumulativeProbabilities(n);
    std::partial_sum(probabilities.begin(), probabilities.end(), cumulativeProbabilities.begin());

    // Step 4: Generate random number and select based on probabilities
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    double r = dis(gen);
    for (int i = 0; i < n; ++i) {
        if (r < cumulativeProbabilities[i]) {
            return population[i];
        }
    }
    return population.back(); // Fallback in case rounding errors occur
}

// TO TRZEBA NAPISAĆ LEPEIEJ NIŻ COPILOT TO ZROBIŁ XD
// Intermediate recombination function
// template<typename T>
// T intermediateRecombination(const T& parent1, const T& parent2, double alpha = 0.5) {
//     T offspring(parent1.size());
//     for (size_t i = 0; i < parent1.size(); ++i) {
//         offspring[i] = alpha * parent1[i] + (1 - alpha) * parent2[i];
//     }
//     return offspring;
// }

template<typename T>
std::tuple<double, T, std::vector<double>> evolutionary_algorithm_real_valued(std::vector<T> x, double m, int evaluation_function) {

    //P - inital population
    std::vector<T> p = x;

    //TODO: implement evolutionary algorithm
    //evaluation(P)
    //REPEAT

    //selection(P)

    std::vector<T> p_prime = std::vector<T>(p.size());
    for (int i = 0; i < p.size(); ++i) {

        //T parent_1 = rouletteWheelSelection(p);
        //T parent_2 = rouletteWheelSelection(p);
        //T offspring = intermediateRecombination(parent_1, parent_2);
        //p_prime.push_back(offspring);
    }

      //P_prime = recombination(selection(P))
      //P_prime_prime = mutation(P_prime)
      //evaluation(P_prime_prime)
      //P = replacement(P, P_prime_prime)

    return {0, p[0], std::vector<double>()};
}

template<typename T>
std::vector<std::vector<double>> run_simulation(double m, T population, int evalutaion_function, std::string numbers_representation)
{
    std::vector<std::vector<double>> all_evaluation_series;
    std::vector<long> execution_times;
    std::vector<double> best_values;
    std::vector<T> best_xs;
    for (int i = 0; i < MAX_ITER_EXPERIMENT; ++i) {
        //start time
        auto start_time = std::chrono::high_resolution_clock::now();
        auto [best_value, best_x, evaluation_values] = evolutionary_algorithm_real_valued(population, m, evalutaion_function);
        //end time
        auto end_time = std::chrono::high_resolution_clock::now();
        long execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        all_evaluation_series.push_back(evaluation_values);
        execution_times.push_back({execution_time});
        best_values.push_back(best_value);
        //best_xs.push_back(best_x);
    }
    //save_execution_times_to_csv(execution_times, "execution_times_evolutionary_fun_" + std::to_string(evalutaion_function) + "_" + numbers_representation + ".csv");
    //save_best_values_to_csv(best_values, "best_values_evolutionary_fun_" + std::to_string(evalutaion_function) + "_" + numbers_representation + ".csv");
    return all_evaluation_series;
}

std::vector<std::vector<double>> exercise4(int evalutaion_function, std::string numbers_representation) {

    //mutation probability
    int size_of_population = 10;
    double m = 4;
    double max_value = 0;
    double min_value = 0;
    std::vector<std::vector<double>> all_evaluation_series;

    if(evalutaion_function == 1) {
        min_value = -3;
        max_value = 3;
    }
    else if(evalutaion_function == 2) {
        min_value = -32.768;
        max_value = 32.768;
    }

    //binary representation
    if(numbers_representation == "binary")
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        //generate random population
        auto population = std::vector<IntPoint>(size_of_population);
        for (int j = 0; j < size_of_population; ++j) {
            // Generate random point coordinates
            int point_coordinates = 0;
            for (int i = 0; i < BITS_PER_DIMENSION * DIMENSIONS; ++i) {
                point_coordinates |= (dis(gen) << i);
            }
            population[j] = IntPoint(DIMENSIONS, BITS_PER_DIMENSION, point_coordinates);
        }

        all_evaluation_series = run_simulation(m, population, evalutaion_function, numbers_representation);
    }
    else if (numbers_representation == "double")
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min_value, max_value);

        //generate random population
        auto population = std::vector<VectorPoint>(size_of_population);
        for (int j = 0; j < size_of_population; ++j) {
            // Generate random point coordinates
            std::vector<double> point_coordinates(DIMENSIONS);
            for (int i = 0; i < DIMENSIONS; ++i) {
                point_coordinates[i] = dis(gen);
            }
            population[j] = VectorPoint(DIMENSIONS, point_coordinates);
        }

        all_evaluation_series = run_simulation(m, population, evalutaion_function, numbers_representation);
    }

    return all_evaluation_series;
}