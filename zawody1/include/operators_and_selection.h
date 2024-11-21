#ifndef MYPROJECT_OPERATORS_AND_SELECTION_H
#define MYPROJECT_OPERATORS_AND_SELECTION_H

#include "../include/Float_representation.h"

std::vector<Float_representation> crossover(const Float_representation& p1, const Float_representation& p2, double r_cross=0.5);
void mutate(Float_representation& individual, double min_value, double max_value);
Float_representation tournamentSelection(std::vector<Float_representation>& population, int tournament_size, const std::vector<double>& eval_values);
#endif //MYPROJECT_OPERATORS_AND_SELECTION_H
