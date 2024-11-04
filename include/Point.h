//
// Created by admin on 04.11.2024.
//

#ifndef MYPROJECT_POINT_H
#define MYPROJECT_POINT_H

#include <vector>

class Point {
public:
    int dimensions = 10;
    virtual ~Point() = default;
};

class IntPoint : public Point {
    int BITS_PER_DIMENSION = 16;
public:
    int neighborhood_operator_int(double m, int solution);
};

class VectorPoint : public Point {
public:
    std::vector<double> neighborhood_operator_vector(const std::vector<double>& m, const std::vector<double>& x, double min_value, double max_value);

};

#endif //MYPROJECT_POINT_H
