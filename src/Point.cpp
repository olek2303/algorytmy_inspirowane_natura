//
// Created by admin on 04.11.2024.
//

#include "Point.h"
#include "distributions.h"
#include <iostream>
#include <vector>
#include <memory>
#include <random>

std::vector<double> VectorPoint::neighborhood_operator_vector(double m, double min_value, double max_value) {
    std::vector<double> neighbor = this->coordinates;
    std::mt19937 rng(std::random_device{}());
    std::normal_distribution<double> dist(0.0, 1);
    for (double &i : neighbor) {
        i += dist(rng);  // Dodaj losową liczbę
        if (i < min_value) {
            i = min_value;  // Przycinanie w dół
        } else if (i > max_value) {
            i = max_value;  // Przycinanie w górę
        }
    }
    return neighbor;
}

int IntPoint::neighborhood_operator_int(double m) {
    int neighbor = this->coordinates;
    int total_bits = bits_per_dimension * dimensions;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, total_bits - 1);
    std::uniform_real_distribution<> dis_uniform(0, 1);

    for (int i = 0; i < total_bits; ++i) {
        int bit_to_flip = dis(gen);
        if (dis_uniform(gen) < m / total_bits) {
            neighbor ^= (1 << bit_to_flip);
        }
    }

    return neighbor;
}

double IntPoint::mapping_value(int decimal, double min_value, double max_value) {
    int max_decimal = (1 << bits_per_dimension) - 1;
    return min_value + (max_value - min_value) * decimal / max_decimal;
}

IntPoint& IntPoint::operator=(const IntPoint& other) {
    if (this == &other) {
        return *this; // handle self assignment
    }
    this->bits_per_dimension = other.bits_per_dimension;
    this->coordinates = other.coordinates;
    this->dimensions = other.dimensions;
    return *this;
}



