#ifndef MYPROJECT_EVALUATION_FUNCTIONS_H
#define MYPROJECT_EVALUATION_FUNCTIONS_H

#include "Float_representation.h"
#include <cmath>
#include <stdexcept>

double evaluate_rosenbrock(const Float_representation& float_repr, double min_value, double max_value) {
    int dimensions = float_repr.GetDimensions();
    if (dimensions < 2) {
        throw std::invalid_argument("Rosenbrock function requires at least 2 dimensions.");
    }

    double result = 0.0;
    for (int j = 0; j < dimensions - 1; ++j) {
        double xj = float_repr.GetDimensionValue(j);
        double xj1 = float_repr.GetDimensionValue(j + 1);

        result += 100.0 * std::pow((xj1 - xj * xj), 2) + std::pow((xj - 1), 2);
    }

    return result;
}

double evaluate_salomon(const Float_representation& float_repr, double min_value, double max_value) {
    int dimensions = float_repr.GetDimensions();
    if (dimensions < 1) {
        throw std::invalid_argument("Salomon function requires at least 1 dimension.");
    }

    double norm = 0.0;
    for (int j = 0; j < dimensions; ++j) {
        double xj = float_repr.GetDimensionValue(j);
        norm += xj * xj;
    }
    norm = std::sqrt(norm);

    return -std::cos(2 * M_PI * norm) + 0.1 * norm + 1.0;
}

double evaluate_whitley(const Float_representation& float_repr, double min_value, double max_value) {
    int dimensions = float_repr.GetDimensions();
    if (dimensions < 2) {
        throw std::invalid_argument("Whitley function requires at least 2 dimensions.");
    }

    double result = 0.0;
    for (int k = 0; k < dimensions; ++k) {
        double x_k = float_repr.GetDimensionValue(k);
        for (int j = 0; j < dimensions; ++j) {
            double x_j = float_repr.GetDimensionValue(j);
            double y_jk = 100 * std::pow((x_k * x_k - x_j), 2) + std::pow((1 - x_k), 2);
            result += (y_jk * y_jk / 4000.0) - std::cos(y_jk) + 1.0;
        }
    }

    return result;
}

#endif //MYPROJECT_EVALUATION_FUNCTIONS_H
