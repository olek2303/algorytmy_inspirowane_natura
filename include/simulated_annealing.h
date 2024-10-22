
#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <vector>
#include <string>



double random_uniform();
template<typename T>
T neighborhood_operator(double m, T x);
double mapping_value(int decimal, double min_value, double max_value);
template <typename T>
double evaluation_function_1(const std::vector<T>& x);
double evaluation_function_1(int x);
template <typename T>
std::pair<double, std::vector<double>> simulated_annealing(T x, double m, int evaluation_function, double T0, double TN );
std::vector<std::vector<double>> excercise3(int evalutaion_function, std::string numbers_representation);
template<typename T>
std::vector<std::vector<double>> run_simulation(double T0, double TN, double m, T starting_point, int evalutaion_function);
#endif //SIMULATED_ANNEALING_H
