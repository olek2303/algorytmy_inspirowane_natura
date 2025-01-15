//
// Created by admin on 20.12.2024.
//

#include "../include/naive_solution.h"

std::vector<Point> naiveParetoFront(const std::vector<Point> &points) {
    std::vector<Point> front;
    for (const auto &point : points) {
        if (!isDominated(point, points)) {
            front.push_back(point);
        }
    }
    return front;
}