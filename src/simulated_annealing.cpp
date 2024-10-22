#include "simulated_annealing.h"
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
const int MAX_ITER_EXPERIMENT = 1;
const int DIMENSIONS = 10;
const int N = 10; //N*L_k = 10000
const int L_k = 10;
const double COOLING_FACTOR = 0.98;

double random_uniform()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

//template for neighborhood operator
template<typename T>
T neighborhood_operator(double m, T x) {
    throw std::invalid_argument("Unsupported type for neighborhood operator");
}

// specify for double
template<>
std::vector<double> neighborhood_operator(double m, std::vector<double> x) {
    std::vector<double> x_prim = x;
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0, 1);
    for (double &i : x_prim) {
        i += dist(rng);
    }
    return x_prim;
}

// Specify for binary representation
template<>
int neighborhood_operator(double m, int x) {
    int neighbor = x;
    int total_bits = BITS_PER_DIMENSION * DIMENSIONS;
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

double mapping_value(int decimal, int min_value, int max_value) {
    int max_decimal = (1 << BITS_PER_DIMENSION) - 1;
    return min_value + (max_value - min_value) * decimal / max_decimal;
}

//template for evaluation function 1
template <typename T>
double evaluation_function_1(const std::vector<T>& x) {
    throw std::invalid_argument("Unsupported type for evaluation function 1");
}

double evaluation_function_1(int x) {
    double sum_x = 0.0;
    for (int i = 0; i < DIMENSIONS; ++i)
    {
        int binary_segment = (x >> (BITS_PER_DIMENSION * (x - 1 - i))) & ((1 << BITS_PER_DIMENSION) - 1);
        double mapped_value = mapping_value(binary_segment, -3,3);
        sum_x += mapped_value;
    }

    // Evaluation function
    double expr1 = -5.0 / (1.0 + sum_x);
    double exponent = exp(expr1);
    double cot_expr = 1.0 / tan(exponent);  // cotangent = 1 / tan(x)
    double result = expr1 + sin(cot_expr);
    return result;
}

template <>
double evaluation_function_1(const std::vector<double>& x) {
    int n = x.size();
    double sum_x = 0.0;

    // Sum of x_1, x_2, ..., x_n
    for (int i = 0; i < n; ++i) {
        sum_x += static_cast<double>(x[i]);
    }

    // Evaluation function
    double expr1 = -5.0 / (1.0 + sum_x);
    double exponent = exp(expr1);
    double cot_expr = 1.0 / tan(exponent);  // cotangent = 1 / tan(x)
    double result = expr1 + sin(cot_expr);

    return result;
}


template<typename T>
std::pair<double, std::vector<double>> simulated_annealing(T x, double m, int evaluation_function, double T0 ) {
    double T_k = T0;
    double best_value = DBL_MAX;
    double evaluation_value_x = 0;
    double evaluation_value_x_prime = 0;
    std::vector<double> evaluation_values;
    for(int k = 0; k < N; ++k)
    {
        for(int l = 0; l < L_k; ++l)
        {
            //select random neighbor
            T x_prime = neighborhood_operator(m, x);

            //calculate evaluation function 1 or 2
            if(evaluation_function == 1){
                evaluation_value_x = evaluation_function_1(x);
                evaluation_value_x_prime = evaluation_function_1(x_prime);
            }else if(evaluation_function == 2){
                //evaluation_value_x = evaluation_function_2(x);
                //evaluation_value_x_prime = evaluation_function_2(x_prime);
                //TODO: implement evaluation function 2
            }

            //save the best value
            if(fabs(evaluation_value_x_prime) < best_value)
                best_value = evaluation_value_x_prime;
            if(fabs(evaluation_value_x) < best_value)
                best_value = evaluation_value_x;

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
        T_k *= COOLING_FACTOR;
    }
    return {best_value, evaluation_values};
}

std::vector<std::vector<double>> excercise3(int evalutaion_function, std::string numbers_representation) {
    //initial temperature
    double T0 = 100.0;
    //mutation probability
    double m = 4;
    std::vector<std::vector<double>> all_evaluation_series;
    std::vector<long> execution_times;
    int starting_point =  (1 << (BITS_PER_DIMENSION * DIMENSIONS)) - 1;
    //binary representation
    if(numbers_representation == "binary")
    {
        //starting_point = (1 << (BITS_PER_DIMENSION * DIMENSIONS)) - 1;
    }

    for (int i = 0; i < MAX_ITER_EXPERIMENT; ++i) {
        //start time
        auto start_time = std::chrono::high_resolution_clock::now();
        auto [best_value, evaluation_values] = simulated_annealing(starting_point, m, evalutaion_function, T0);
        //end time
        auto end_time = std::chrono::high_resolution_clock::now();
        long execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        all_evaluation_series.push_back(evaluation_values);
        execution_times.push_back({execution_time});
        std::cout << "Execution time of experiment: " << i << " is: " << execution_time << " ms" << std::endl;
        std::cout << "Best value of experiment: " << i << " is: " << best_value << std::endl;
    }
    // std::string file_name = "execution_times_" + std::to_string(dimensions) + ".csv";
    // save_execution_times_to_csv(execution_times, file_name);

    return all_evaluation_series;
}
