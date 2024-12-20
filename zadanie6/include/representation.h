#ifndef MYPROJECT_REPRESENTATION_H
#define MYPROJECT_REPRESENTATION_H

#include <iostream>
#include <cmath>

using namespace std;

struct Point {
    double f1, f2;

    bool operator==(const Point &other) const {
        return std::fabs(f1 - other.f1) < 1e-6 && std::fabs(f2 - other.f2) < 1e-6;
    }
};

#endif //MYPROJECT_REPRESENTATION_H