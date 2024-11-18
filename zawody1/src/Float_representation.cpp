#include "../include/Float_representation.h"
#include <iostream>
#include <vector>
#include <memory>
#include <random>

double Float_representation::GetDimensionValue(int dimension) const {
    if (dimension >= 0 && dimension < dimensions) {
        return data[dimension];
    } else {
        std::cerr << "Invalid dimension!" << std::endl;
        return 0.0f;
    }
}

void Float_representation::SetDimensionValue(int dimension, double value) {
    if (dimension >= 0 && dimension < dimensions) {
        data[dimension] = value;
    } else {
        std::cerr << "Invalid dimension!" << std::endl;
    }
}
