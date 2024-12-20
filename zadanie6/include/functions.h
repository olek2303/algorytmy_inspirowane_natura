#ifndef MYPROJECT_FUNCTIONS_H
#define MYPROJECT_FUNCTIONS_H

#include <vector>
#include "representation.h"
#include "kung_solution.h"
#include "naive_solution.h"


bool isDominated(const Point &point, const std::vector<Point> &front);
std::vector<std::vector<Point>> findAllFronts(const std::vector<Point> &points,
                                              const std::string &method);

#endif //MYPROJECT_FUNCTIONS_H