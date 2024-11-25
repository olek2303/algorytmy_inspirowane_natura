#ifndef MYPROJECT_FLOAT_REPRESENTATION_H
#define MYPROJECT_FLOAT_REPRESENTATION_H
#include <iostream>
#include <vector>
#include <bitset>
#include <cmath>

class Float_representation {
    int dimensions;
    std::vector<double> data;

public:
    Float_representation(int dimensions = 10)
            : dimensions(dimensions), data(dimensions, 0.0f) {}

    ~Float_representation() = default;

    void SetDimensionValue(int dimension, double value);
    double GetDimensionValue(int dimension) const;  // zwraca wartosc w danym wymiarze
    int GetDimensions() const { return dimensions; }    // liczba wymiarow
    void SetPoint(std::vector<double> coordinates) { this->data = coordinates; }
    void SetPoint(int index, double value) { this->data[index] = value; }
    std::vector<double> GetPoint() const { return data; }
    double GetPoint(int index) const { return data[index]; }
    //operator kopiowania
    Float_representation& operator=(const Float_representation& other) {
        if (this == &other) {
            return *this; // handle self assignment
        }
        dimensions = other.dimensions;
        data = other.data;
        return *this;
    }

    void Print() const {
        for (int i = 0; i < dimensions; ++i) {
            std::cout << "Dimension " << i << ": " << data[i] << std::endl;
        }
    }
};


#endif //MYPROJECT_FLOAT_REPRESENTATION_H
