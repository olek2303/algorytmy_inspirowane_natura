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
void writeArchive(const std::vector<Individual>& archive, const std::string& filename);
void writeParetoFront(const std::vector<Individual>& archive, const std::string& filename);

#endif //FUNCTIONS_H
