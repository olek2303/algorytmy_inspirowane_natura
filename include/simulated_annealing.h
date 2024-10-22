
#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <vector>
#include <string>



double random_uniform();
template<typename T>
T neighborhood_operator(double m, T x);
double mapping_value(int decimal, int min_value, int max_value);
template <typename T>
double evaluation_function_1(const std::vector<T>& x);
double evaluation_function_1(int x);
template <typename T>
std::pair<double, std::vector<double>> simulated_annealing(T x, double m, int evaluation_function, double T0 );
std::vector<std::vector<double>> excercise3(int evalutaion_function, std::string numbers_representation);
#endif //SIMULATED_ANNEALING_H
