//
// Created by admin on 04.11.2024.
//

#include "Point.h"
#include "distributions.h"
#include <iostream>
#include <vector>
#include <memory>

std::vector<double> VectorPoint::neighborhood_operator_vector(const std::vector<double>& m, const std::vector<double>& x, double min_value, double max_value) {
    std::vector<double> neighbor = x;
    for (double &i : neighbor) {
        std::vector<double> rand = generate_normal_distribution(0, 1, 1);
        i += rand[0];  // Dodaj losową liczbę
        if (i < min_value) {
            i = min_value;  // Przycinanie w dół
        } else if (i > max_value) {
            i = max_value;  // Przycinanie w górę
        }
    }
    return neighbor;
}

int IntPoint::neighborhood_operator_int(double m, int x) {
    int neighbor = x;
    int total_bits = BITS_PER_DIMENSION * dimensions;

    for (int i = 0; i < total_bits; ++i) {
        std::vector<int> rand_int = generate_uniform_int_distribution(0, total_bits - 1, 1);
        std::vector<double> rand_double = generate_uniform_distribution(0, 1, 1);

        int bit_to_flip = rand_int[0];
        if (rand_double[0] < m / total_bits) {
            neighbor ^= (1 << bit_to_flip);
        }
    }

    return neighbor;
}


