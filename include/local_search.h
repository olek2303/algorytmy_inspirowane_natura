// lab2

#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include <vector>
#include "../include/Point.h"

// Constants
extern const int N_1;
extern const int N_2;
extern const int N_3;

// Function declarations
double evaluation_function(IntPoint x);
std::vector<double> first_improvement_local_search(IntPoint x, double m);
std::vector<std::vector<double>> exercise2(int dimensions);


#endif // LOCAL_SEARCH_H