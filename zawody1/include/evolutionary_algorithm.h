
#ifndef EVOLUTIONARY_ALGORITHM_H
#define EVOLUTIONARY_ALGORITHM_H
#include "Float_representation.h"
#include <vector>

void initializePopulation(std::vector<Float_representation>& population, double min_value, double max_value);
std::vector<double> evolutionary_algorithm(double m, int evaluation_function, double min_value, double max_value, int pop_size, int dimensions);
#endif //EVOLUTIONARY_ALGORITHM_H
