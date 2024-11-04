#include "simulated_annealing.h"
#include "functions.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <filesystem>
#include <algorithm>
#include <float.h>
#include <string>


// Constants

const int BITS_PER_DIMENSION = 16;
const int MAX_ITER_EXPERIMENT = 100;
const int DIMENSIONS = 10;
const int N = 100; //N*L_k = 10000
const int L_k = 100;


//template for evaluation function 1

template <typename T>
double evaluation_function_1(T x) {
    double sum_x = 0.0;
    if constexpr (std::is_same_v<T, IntPoint>) {
        for (int i = 0; i < DIMENSIONS; ++i)
        {
            int binary_segment = (x.GetPoint() >> (x.GetBitsPerDim() * (x.GetPoint() - 1 - i))) & ((1 << x.GetBitsPerDim()) - 1);
            double mapped_value = x.mapping_value(binary_segment, -3,3);
            sum_x += mapped_value * mapped_value;
        }
    } else if constexpr (std::is_same_v<T, VectorPoint>) {
        std::vector<double> coordinates = x.GetPoint();
        int n = coordinates.size();
        for (int i = 0; i < n; ++i) {
            sum_x += (coordinates[i]) * (coordinates[i]);
        }
    } else {
        std::cerr << "Unsupported type" << std::endl;
    }

    // Evaluation function
    double expr1 = -5.0 / (1.0 + sum_x);
    double exponent = exp(expr1);
    double cot_expr = 1.0 / tan(exponent);  // cotangent = 1 / tan(x)
    double result = expr1 + sin(cot_expr);
    return result;
}

//template for evaluation function 2
template <typename T>
double evaluation_function_2(T x) {
    double sum_x_1 = 0.0;
    double sum_x_2 = 0.0;
    double a = 20.0;
    double b = 0.2;
    double c = 2 * M_PI;

    if constexpr (std::is_same_v<T, IntPoint>) {
        for (int i = 0; i < DIMENSIONS; ++i)
        {
            int binary_segment = (x.GetPoint() >> (x.GetBitsPerDim() * (x.GetPoint() - 1 - i))) & ((1 << x.GetBitsPerDim()) - 1);
            double mapped_value = x.mapping_value(binary_segment, -32.768,32.768);
            sum_x_1 += mapped_value * mapped_value;
            sum_x_2 += std::cos(c * mapped_value);
        }
    } else if constexpr (std::is_same_v<T, VectorPoint>) {
        std::vector<double> coordinates = x.GetPoint();
        int n = coordinates.size();
        // Sum of x_1^2, x_2^2, ..., x_n^2 and cos(2 * pi * x_i)
        for (int i = 0; i < n; ++i) {
            sum_x_1 += coordinates[i] * coordinates[i];
            sum_x_2 += std::cos(c * coordinates[i]);
        }
    } else {
        std::cerr << "Unsupported type" << std::endl;
    }

    // Evaluation function
    double expr1 = -a * std::exp(-b * std::sqrt(sum_x_1 / DIMENSIONS));
    double expr2 = std::exp(sum_x_2 / DIMENSIONS);
    double result = expr1 - expr2 + a + std::exp(1.0);

    return result;
}


template<typename T>
std::tuple<double, T, std::vector<double>> simulated_annealing(T x, double m, int evaluation_function, double T0, double TN ) {
    double T_k = T0;
    double best_value = DBL_MAX;
    T best_x = x;
    double evaluation_value_x = 0;
    double evaluation_value_x_prime = 0;
    std::vector<double> evaluation_values;
    double min_value = 0;
    double max_value = 0;
    if(evaluation_function == 1)
    {
        min_value = -3;
        max_value = 3;
    }
    else if(evaluation_function == 2)
    {
        min_value = -32.768;
        max_value = 32.768;
    }
    for(int k = 0; k < N; ++k)
    {
        for(int l = 0; l < L_k; ++l)
        {
            //select random neighbor
            T x_prime = x;
            if constexpr (std::is_same_v<T, IntPoint>) {
                x_prime.SetPoint(x.neighborhood_operator_int(m));
            } else if constexpr (std::is_same_v<T, VectorPoint>) {
                x_prime.SetPoint(x.neighborhood_operator_vector(m, min_value, max_value));
            } else {
                std::cerr << "Unsupported type" << std::endl;
            }
            switch (evaluation_function) {
                case 1:
                    evaluation_value_x = evaluation_function_1(x);
                    evaluation_value_x_prime = evaluation_function_1(x_prime);
                    break;
                case 2:
                    evaluation_value_x = evaluation_function_2(x);
                    evaluation_value_x_prime = evaluation_function_2(x_prime);
                    break;
                default:
                    std::cerr << "Unsupported evaluation function" << std::endl;
                    break;
            }

            //save the best value
            if(evaluation_value_x_prime < best_value)
            {
                best_value = evaluation_value_x_prime;
                best_x = x_prime;
            }
            if(evaluation_value_x < best_value)
            {
                best_value = evaluation_value_x;
                best_x = x;
            }

            //compare evaluation values
            if (evaluation_value_x_prime <= evaluation_value_x)
            {
                evaluation_values.push_back(evaluation_value_x_prime);
                x = x_prime;
            }else{
                if( random_uniform() < exp((evaluation_value_x - evaluation_value_x_prime) / T_k)){
                    evaluation_values.push_back(evaluation_value_x_prime);
                    x = x_prime;
                }else{
                    evaluation_values.push_back(evaluation_value_x);
                }
            }
        }
        //from lecture #3, example c
        double a = (T0 - TN) * (N + 1) / N;
        T_k = a/(k+1) + T0 - a;
    }
    std::cout << "Best value: " << best_value << std::endl;
    std::cout << "Final temperature: " << T_k << std::endl;
    return {best_value, best_x, evaluation_values};
}

template<typename T>
std::vector<std::vector<double>> run_simulation(double T0, double TN, double m, T starting_point, int evalutaion_function, std::string numbers_representation)
{
    std::vector<std::vector<double>> all_evaluation_series;
    std::vector<long> execution_times;
    std::vector<double> best_values;
    std::vector<T> best_xs;
    for (int i = 0; i < MAX_ITER_EXPERIMENT; ++i) {
        //start time
        auto start_time = std::chrono::high_resolution_clock::now();
        auto [best_value, best_x, evaluation_values] = simulated_annealing(starting_point, m, evalutaion_function, T0, TN);
        //end time
        auto end_time = std::chrono::high_resolution_clock::now();
        long execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        all_evaluation_series.push_back(evaluation_values);
        execution_times.push_back({execution_time});
        best_values.push_back(best_value);
        best_xs.push_back(best_x);
    }
    save_execution_times_to_csv(execution_times, "execution_times_annealing_fun_" + std::to_string(evalutaion_function) + "_" + numbers_representation + ".csv");
    save_best_values_to_csv(best_values, "best_values_annealing_fun_" + std::to_string(evalutaion_function) + "_" + numbers_representation + ".csv");
    return all_evaluation_series;
}

std::vector<std::vector<double>> exercise3(int evalutaion_function, std::string numbers_representation) {
    //initial temperature
    double T0 = 100.0;
    double TN = 1.0;
    //mutation probability
    double m = 4;
    double max_value = 0;
    std::vector<std::vector<double>> all_evaluation_series;

    //binary representation
    if(numbers_representation == "binary")
    {
        int starting_point_coordinates = (1 << (BITS_PER_DIMENSION * DIMENSIONS)) - 1;
        IntPoint starting_point = IntPoint(DIMENSIONS, BITS_PER_DIMENSION, starting_point_coordinates);
        all_evaluation_series = run_simulation(T0, TN, m, starting_point, evalutaion_function, numbers_representation);
    }
    else if (numbers_representation == "double")
    {
        if(evalutaion_function == 1)
            max_value = 3;
        else if(evalutaion_function == 2)
            max_value = 32.768;

        std::vector<double> starting_point_coordinates = std::vector<double>(DIMENSIONS,max_value);
        VectorPoint starting_point = VectorPoint(DIMENSIONS, starting_point_coordinates);
        all_evaluation_series = run_simulation(T0, TN, m, starting_point, evalutaion_function, numbers_representation);
    }

    return all_evaluation_series;
}
