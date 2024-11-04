#include "local_search.h"
#include "functions.h"
#include <vector>
#include <fstream>
#include <random>
#include <filesystem>


// Constants

const double MIN_VALUE = -10;
const double MAX_VALUE = 10;
const int MAX_ITER = 10000;
const int MAX_ITER_EXPERIMENT = 100;
const int N_1 = 2;
const int N_2 = 5;
const int N_3 = 10;


double evaluation_function(IntPoint x) {
    double total_sum = 0;
    for (int i = 0; i < x.dimensions; ++i) {
        int binary_segment = (x.GetPoint() >> (x.GetBitsPerDim() * (x.dimensions - 1 - i))) & ((1 << x.GetBitsPerDim()) - 1);
        double mapped_value = x.mapping_value(binary_segment, MIN_VALUE, MAX_VALUE);
        total_sum += mapped_value * mapped_value;
    }
    return total_sum;
}

std::vector<double> first_improvement_local_search(IntPoint x, double m) {
    int iteration = 0;
    std::vector<double> evaluation_values;
    while (iteration < MAX_ITER) {
        ++iteration;
        IntPoint x_prime = x;
        x_prime.SetPoint(x.neighborhood_operator_int(m));

        double evaluation_value_x = evaluation_function(x);
        double evaluation_value_x_prime = evaluation_function(x_prime);
        if (evaluation_value_x_prime < evaluation_value_x) {
            evaluation_values.push_back(evaluation_value_x_prime);
            x = x_prime;
            continue;
        }
        evaluation_values.push_back(evaluation_value_x);

    }
    return evaluation_values;
}

std::vector<std::vector<double>> exercise2(int dimensions) {
    int bits_per_dimension = 16;
    int starting_point_coordinates = (1 << (bits_per_dimension * dimensions)) - 1;
    IntPoint starting_point = IntPoint(dimensions, bits_per_dimension, starting_point_coordinates);
    //permuation
    double m = 4;

    std::vector<std::vector<double>> all_evaluation_series;
    std::vector<long> execution_times;
    for (int i = 0; i < MAX_ITER_EXPERIMENT; ++i) {
        //start time
        auto start_time = std::chrono::high_resolution_clock::now();
        auto evaluation_values = first_improvement_local_search(starting_point, m);
        //end time
        auto end_time = std::chrono::high_resolution_clock::now();
        long execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        all_evaluation_series.push_back(evaluation_values);
        execution_times.push_back({execution_time});
    }
    std::string file_name = "execution_times_" + std::to_string(dimensions) + ".csv";
    save_execution_times_to_csv(execution_times, file_name);
    return all_evaluation_series;
}
