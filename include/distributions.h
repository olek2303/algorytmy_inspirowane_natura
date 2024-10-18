#ifndef DISTRIBUTIONS_H
#define DISTRIBUTIONS_H

#include <vector>

std::vector<double> generate_normal_distribution(double mean, double std_dev, int size);
std::vector<double> generate_uniform_distribution(double low, double high, int size);

#endif // DISTRIBUTIONS_H