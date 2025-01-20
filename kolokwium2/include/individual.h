#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>

struct Individual {
    std::vector<double> variables;
    std::vector<double> objectives;
    double fitness;
    double strength;
    double density;
};

#endif //INDIVIDUAL_H
