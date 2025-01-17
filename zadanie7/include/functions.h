//
// Created by xiii5 on 17.01.2025.
//

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <algorithm>
#include <cmath>
#include <fstream>
#include <vector>
#include "individual.h"

double clamp(double value, double min, double max);
void writeParetoFront(const std::vector<Individual>& population, const std::string& filename);

#endif //FUNCTIONS_H
