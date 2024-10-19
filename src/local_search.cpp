#include "local_search.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <filesystem>
#include <algorithm>

// Constants
const int BITS_PER_DIMENSION = 16;
const double MIN_VALUE = -10;
const double MAX_VALUE = 10;
const int MAX_ITER = 10000;
const int MAX_ITER_EXPERIMENT = 100;
const int N_1 = 2;
const int N_2 = 5;
const int N_3 = 10;

double mapping_value(int decimal, int bits_per_dimension) {
    int max_decimal = (1 << bits_per_dimension) - 1;
    return MIN_VALUE + (MAX_VALUE - MIN_VALUE) * decimal / max_decimal;
}

int neighborhood_operator(double m, int solution, int dimensions) {
    int neighbor = solution;
    int total_bits = BITS_PER_DIMENSION * dimensions;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, total_bits - 1);
    std::uniform_real_distribution<> dis_uniform(0, 1);

    for (int i = 0; i < total_bits; ++i) {
        int bit_to_flip = dis(gen);
        if (dis_uniform(gen) < m / total_bits) {
            neighbor ^= (1 << bit_to_flip);
        }
    }
    return neighbor;
}

double evaluation_function(int dimensions, int solution) {
    double total_sum = 0;
    for (int i = 0; i < dimensions; ++i) {
        int binary_segment = (solution >> (BITS_PER_DIMENSION * (dimensions - 1 - i))) & ((1 << BITS_PER_DIMENSION) - 1);
        double mapped_value = mapping_value(binary_segment, BITS_PER_DIMENSION);
        total_sum += mapped_value * mapped_value;
    }
    return total_sum;
}

std::pair<int, std::vector<double>> first_improvement_local_search(int x, double m, int dimensions) {
    int iteration = 0;
    std::vector<double> evaluation_values;
    while (iteration < MAX_ITER) {
        ++iteration;
        bool improved = false;

        int total_bits = BITS_PER_DIMENSION * dimensions;
        for (int i = 0; i < total_bits; ++i) {
            int x_prime = neighborhood_operator(m, x, dimensions);
            double evaluation_value_x = evaluation_function(dimensions, x);
            double evaluation_value_x_prime = evaluation_function(dimensions, x_prime);
            if (evaluation_value_x_prime < evaluation_value_x) {
                evaluation_values.push_back(evaluation_value_x_prime);
                x = x_prime;
                improved = true;
                break;
            } else {
                evaluation_values.push_back(evaluation_value_x);
            }
        }

        if (!improved) {
            break;
        }
    }
    return {x, evaluation_values};
}

std::vector<std::vector<double>> excercise2(int dimensions) {
    int starting_point = (1 << (BITS_PER_DIMENSION * dimensions)) - 1;
    double m = 4;
    std::vector<std::vector<double>> all_evaluation_series;
    for (int i = 0; i < MAX_ITER_EXPERIMENT; ++i) {
        auto [_, evaluation_values] = first_improvement_local_search(starting_point, m, dimensions);
        all_evaluation_series.push_back(evaluation_values);
    }
    return all_evaluation_series;
}

std::vector<std::vector<double>> pad_evaluation_series(const std::vector<std::vector<double>>& all_evaluation_series) {
    size_t max_length = 0;
    for (const auto& series : all_evaluation_series) {
        if (series.size() > max_length) {
            max_length = series.size();
        }
    }

    std::vector<std::vector<double>> padded_series;
    for (const auto& series : all_evaluation_series) {
        std::vector<double> padded = series;
        padded.resize(max_length, series.back());
        padded_series.push_back(padded);
    }
    return padded_series;
}

std::vector<double> average_evaluation_series(const std::vector<std::vector<double>>& all_evaluation_series) {
    auto padded_series = pad_evaluation_series(all_evaluation_series);
    size_t series_length = padded_series[0].size();
    std::vector<double> avg_series(series_length, 0.0);

    for (size_t i = 0; i < series_length; ++i) {
        for (const auto& series : padded_series) {
            avg_series[i] += series[i];
        }
        avg_series[i] /= padded_series.size();
    }
    return avg_series;
}

void print_current_working_directory() {
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
}

void save_to_txt(const std::vector<std::vector<double>>& data, const std::string& filename) {
    std::cout << "Saving to file: " << filename << std::endl;
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    for (const auto& series : data) {
        for (double value : series) {
            file << value << " ";
        }
        file << "\n";
    }
    file.close();
    std::cout << "File saved: " << filename << std::endl;
}

void save_avg_to_txt(const std::vector<double>& avg_data, const std::string& filename) {
    std::cout << "Saving to file: " << filename << std::endl;
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    for (double value : avg_data) {
        file << value << "\n";
    }
    file.close();
    std::cout << "File saved: " << filename << std::endl;
}