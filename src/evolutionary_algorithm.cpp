
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
const int MAX_ITER_EXPERIMENT = 10;
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

// Specialized crossover function for IntPoint
std::vector<IntPoint> crossover(const IntPoint& p1, const IntPoint& p2, double r_cross=0.5) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> dis_int(1, p1.GetBitsPerDim() * p1.dimensions - 2);

    IntPoint c1 = p1;
    IntPoint c2 = p2;

    if (dis(gen) < r_cross) {
        int pt = dis_int(gen);

        int mask = (1 << pt) - 1;
        int new_c1 = (p1.GetPoint() & mask) | (p2.GetPoint() & ~mask);
        int new_c2 = (p2.GetPoint() & mask) | (p1.GetPoint() & ~mask);

        c1.SetPoint(new_c1);
        c2.SetPoint(new_c2);
    }

    return {c1, c2};
}

// Specialized crossover function for VectorPoint
std::vector<VectorPoint> crossover(const VectorPoint& p1, const VectorPoint& p2, double r_cross=0.5) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> dis_int(1, p1.GetPoint().size() - 2);

    VectorPoint c1 = p1;
    VectorPoint c2 = p2;

    if (dis(gen) < r_cross) {
        int pt = dis_int(gen);

        for (int i = pt; i < p1.GetPoint().size(); ++i) {
            c1.GetPoint()[i] = p2.GetPoint()[i];
            c2.GetPoint()[i] = p1.GetPoint()[i];
        }
    }

    return {c1, c2};
}

template<typename T>
std::tuple<double, T, std::vector<double>> evolutionary_algorithm_real_valued(std::vector<T> x, double m, int evaluation_function) {
    std::vector<T> p = x;
    std::vector<double> evaluation_values;

    for (int iter = 0; iter < MAX_ITER; ++iter) {
        std::vector<T> p_prime;
        p_prime.reserve(p.size()); // Reserve space to avoid reallocations

        for (int i = 0; i < p.size() / 2; ++i) {
            T parent_1 = rouletteWheelSelection(p);
            T parent_2 = rouletteWheelSelection(p);
            auto offspring = crossover(parent_1, parent_2, m);
            p_prime.push_back(offspring[0]);
            p_prime.push_back(offspring[1]);
        }

        // Ensure p_prime has the same size as p
        if (p_prime.size() > p.size()) {
            p_prime.resize(p.size());
        }

        // Mutation and evaluation logic should be added here

        p = p_prime;
    }

    double best_value = getFitness(p[0]);
    T best_x = p[0];
    return {best_value, best_x, evaluation_values};
}

template<typename T>
std::vector<std::vector<double>> run_simulation(double m, std::vector<T> population, int evaluation_function, std::string numbers_representation) {
    std::vector<std::vector<double>> all_evaluation_series;
    std::vector<long> execution_times;
    std::vector<double> best_values;
    std::vector<T> best_xs; // Ensure best_xs is a vector of type T
    for (int i = 0; i < MAX_ITER_EXPERIMENT; ++i) {
        std::cout << "start iter:  " << i << std::endl;
        auto start_time = std::chrono::high_resolution_clock::now();
        auto [best_value, best_x, evaluation_values] = evolutionary_algorithm_real_valued(population, m, evaluation_function);
        auto end_time = std::chrono::high_resolution_clock::now();
        std::cout << "clock: " << std::endl;
        long execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        all_evaluation_series.push_back(evaluation_values);
        execution_times.push_back(execution_time);
        best_values.push_back(best_value);
        best_xs.push_back(best_x); // This should now work correctly
        std::cout << "Best value:";
        std::cout << best_value << std::endl;
    }
    return all_evaluation_series;
}

std::vector<std::vector<double>> exercise4(int evaluation_function, std::string numbers_representation) {
    //mutation probability
    int size_of_population = 10;
    double m = 4;
    double max_value = 0;
    double min_value = 0;
    std::vector<std::vector<double>> all_evaluation_series;

    if(evaluation_function == 1) {
        min_value = -3;
        max_value = 3;
    }
    else if(evaluation_function == 2) {
        min_value = -32.768;
        max_value = 32.768;
    }

    //binary representation
    if(numbers_representation == "binary") {
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

        all_evaluation_series = run_simulation(m, population, evaluation_function, numbers_representation);
    }
    else if (numbers_representation == "double") {
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

        all_evaluation_series = run_simulation(m, population, evaluation_function, numbers_representation);
    }

    return all_evaluation_series;
}