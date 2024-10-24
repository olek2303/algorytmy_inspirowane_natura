
#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <vector>
#include <string>



template<typename T>
T neighborhood_operator(double m, T x, double min_value, double max_value);

double mapping_value(int decimal, double min_value, double max_value);

template <typename T>
double evaluation_function_1(const std::vector<T>& x);
double evaluation_function_1(int x);

template <typename T>
std::tuple<double, T, std::vector<double>> simulated_annealing(T x, double m, int evaluation_function, double T0, double TN );
std::vector<std::vector<double>> exercise3(int evalutaion_function, std::string numbers_representation);

template<typename T>
std::vector<std::vector<double>> run_simulation(double T0, double TN, double m, T starting_point, int evalutaion_function, std::string numbers_representation);

template <typename T>
double evaluation_function_2(const std::vector<T>& x);
double evaluation_function_2(int x);
#endif //SIMULATED_ANNEALING_H
