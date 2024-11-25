
#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <vector>
#include <string>


template <typename T>
std::tuple<double, T, std::vector<double>> simulated_annealing(T x, double m, int evaluation_function, double T0, double TN );
std::vector<std::vector<double>> exercise3(int evalutaion_function, std::string numbers_representation);

template<typename T>
std::vector<std::vector<double>> run_simulation(double T0, double TN, double m, T starting_point, int evalutaion_function, std::string numbers_representation);
#endif //SIMULATED_ANNEALING_H
