
#ifndef EVALUATION_FUNCTIONS_H
#define EVALUATION_FUNCTIONS_H

#include <vector>
#include <cmath>
#include <iostream>
#include "Point.h"


//template for evaluation function 1
template <typename T>
double evaluation_function_1(T x) {
    double sum_x = 0.0;
    if constexpr (std::is_same_v<T, IntPoint>) {
        for (int i = 0; i < x.dimensions; ++i)
        {
            int binary_segment = (x.GetPoint() >> (x.GetBitsPerDim() * (x.GetPoint() - 1 - i))) & ((1 << x.GetBitsPerDim()) - 1);
            double mapped_value = x.mapping_value(binary_segment, -3,3);
            sum_x += mapped_value * mapped_value;
        }
    } else if constexpr (std::is_same_v<T, VectorPoint>) {
        std::vector<double> coordinates = x.GetPoint();
        int n = coordinates.size();
        for (int i = 0; i < n; ++i) {
            sum_x += (coordinates[i]) * (coordinates[i]);
        }
    } else {
        std::cerr << "Unsupported type" << std::endl;
    }

    // Evaluation function
    double expr1 = -5.0 / (1.0 + sum_x);
    double exponent = exp(expr1);
    double cot_expr = 1.0 / tan(exponent);  // cotangent = 1 / tan(x)
    double result = expr1 + sin(cot_expr);
    return result;
}

//template for evaluation function 2
template <typename T>
double evaluation_function_2(T x) {
    double sum_x_1 = 0.0;
    double sum_x_2 = 0.0;
    double a = 20.0;
    double b = 0.2;
    double c = 2 * M_PI;

    if constexpr (std::is_same_v<T, IntPoint>) {
        for (int i = 0; i < x.dimensions; ++i)
        {
            int binary_segment = (x.GetPoint() >> (x.GetBitsPerDim() * (x.GetPoint() - 1 - i))) & ((1 << x.GetBitsPerDim()) - 1);
            double mapped_value = x.mapping_value(binary_segment, -32.768,32.768);
            sum_x_1 += mapped_value * mapped_value;
            sum_x_2 += std::cos(c * mapped_value);
        }
    } else if constexpr (std::is_same_v<T, VectorPoint>) {
        std::vector<double> coordinates = x.GetPoint();
        int n = coordinates.size();
        // Sum of x_1^2, x_2^2, ..., x_n^2 and cos(2 * pi * x_i)
        for (int i = 0; i < n; ++i) {
            sum_x_1 += coordinates[i] * coordinates[i];
            sum_x_2 += std::cos(c * coordinates[i]);
        }
    } else {
        std::cerr << "Unsupported type" << std::endl;
    }

    // Evaluation function
    double expr1 = -a * std::exp(-b * std::sqrt(sum_x_1 / x.dimensions));
    double expr2 = std::exp(sum_x_2 / x.dimensions);
    double result = expr1 - expr2 + a + std::exp(1.0);

    return result;
}
#endif //EVALUATION_FUNCTIONS_H
