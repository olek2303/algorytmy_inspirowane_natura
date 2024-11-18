#ifndef MYPROJECT_BINARY_REPRESENTATION_H
#define MYPROJECT_BINARY_REPRESENTATION_H
#include <iostream>
#include <vector>
#include <bitset>
#include <cmath>

class Float_representation {
    int dimensions;
    int bits_per_dimension;
    std::vector<double> data;

public:
    Float_representation(int dimensions = 10, int bits_per_dimension = 16)
            : dimensions(dimensions), bits_per_dimension(bits_per_dimension), data(dimensions, 0.0f) {}

    ~Float_representation() = default;

    void SetDimensionValue(int dimension, double value);
    double GetDimensionValue(int dimension) const;
    int GetDimensions() const { return dimensions; }
    int GetBitsPerDim() const { return bits_per_dimension; }
    void SetPoint(std::vector<double> coordinates) { this->data = coordinates; }
    void SetPoint(int index, double value) { this->data[index] = value; }
    std::vector<double> GetPoint() const { return data; }
    double GetPoint(int index) const { return data[index]; }


    void Print() const {
        for (int i = 0; i < dimensions; ++i) {
            std::cout << "Dimension " << i << ": " << data[i] << std::endl;
        }
    }
};


#endif //MYPROJECT_BINARY_REPRESENTATION_H
