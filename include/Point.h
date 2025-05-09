//
// Created by admin on 04.11.2024.
//

#ifndef MYPROJECT_POINT_H
#define MYPROJECT_POINT_H

#include <vector>

class Point {
public:
    int dimensions = 10;
    Point() = default;
    virtual ~Point() = default;
};

class IntPoint : public Point {
    int bits_per_dimension = 16;
    int coordinates;
public:
    IntPoint() : Point(), coordinates(0){};
    IntPoint(int dimensions, int bits_per_dimension, int coordinates) : Point(), bits_per_dimension(bits_per_dimension), coordinates(coordinates) {this->dimensions = dimensions;};
    explicit IntPoint(int coordinates) : Point(),coordinates(coordinates){};
    ~IntPoint() override = default;
    IntPoint& operator=(const IntPoint& other);


    void SetPoint(int coordinates) { this->coordinates = coordinates; }
    int GetPoint() const { return coordinates; }
    int GetBitsPerDim() const { return bits_per_dimension; }
    int neighborhood_operator_int(double m);
    double mapping_value(int decimal, double min_value, double max_value);
};

class VectorPoint : public Point {
    std::vector<double> coordinates;
public:
    VectorPoint() : Point(), coordinates(std::vector<double>()){};
    VectorPoint(int dimensions, std::vector<double> coordinates) : Point(), coordinates(std::move(coordinates)) {this->dimensions = dimensions;};
    explicit VectorPoint(std::vector<double> coordinates) : Point(), coordinates(std::move(coordinates)){};
    ~VectorPoint() override = default;


    void SetPoint(std::vector<double> coordinates) { this->coordinates = coordinates; }
    void SetPoint(int index, double value) { this->coordinates[index] = value; }
    std::vector<double> GetPoint() const { return coordinates; }
    double GetPoint(int index) const { return coordinates[index]; }
    std::vector<double> neighborhood_operator_vector(double m, double min_value, double max_value);

};

#endif //MYPROJECT_POINT_H
