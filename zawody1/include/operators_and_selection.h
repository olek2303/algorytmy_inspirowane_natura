#ifndef MYPROJECT_OPERATORS_AND_SELECTION_H
#define MYPROJECT_OPERATORS_AND_SELECTION_H

#include "../include/Float_representation.h"

std::vector<Float_representation> crossover(const Float_representation& p1, const Float_representation& p2, double r_cross=0.5);
void mutate(Float_representation& individual, double min_value, double max_value);
Float_representation tournamentSelection(const std::vector<Float_representation>& population, int eval_function, int tournament_size = 5);
#endif //MYPROJECT_OPERATORS_AND_SELECTION_H
