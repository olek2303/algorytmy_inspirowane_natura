

#ifndef EVOLUTION_ALGORITHM_H
#define EVOLUTION_ALGORITHM_H

#include <vector>
#include <string>

template <typename T>
double getFitness(const T& point);
template<typename T>
T rouletteWheelSelection(const std::vector<T>& population);

template<typename T>
std::tuple<double, T, std::vector<double>> evolutionary_algorithm_real_valued(T x, double m, int evaluation_function);
template<typename T>
std::vector<std::vector<double>> run_simulation(double m, T population, int evalutaion_function, std::string numbers_representation);
std::vector<std::vector<double>> exercise4(int evalutaion_function, std::string numbers_representation);

#endif //EVOLUTION_ALGORITHM_H
