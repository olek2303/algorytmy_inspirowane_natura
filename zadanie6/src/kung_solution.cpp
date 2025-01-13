//
// Created by admin on 20.12.2024.
//

#include <algorithm>
#include "../include/kung_solution.h"

std::vector<Point> kungParetoFront(std::vector<Point> points) {
    if (points.size() <= 1) {
        return points;
    }

    std::sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
        return a.f1 < b.f1;
    });

    size_t mid = points.size() / 2;
    std::vector<Point> left(points.begin(), points.begin() + mid);
    std::vector<Point> right(points.begin() + mid, points.end());

    std::vector<Point> leftFront = kungParetoFront(left);
    std::vector<Point> rightFront = kungParetoFront(right);

    std::vector<Point> merged;
    size_t i = 0, j = 0;

    while (i < leftFront.size() && j < rightFront.size()) {
        if (leftFront[i].f1 < rightFront[j].f1) {
            if (merged.empty() || merged.back().f2 > leftFront[i].f2) {
                merged.push_back(leftFront[i]);
            }
            i++;
        } else {
            if (merged.empty() || merged.back().f2 > rightFront[j].f2) {
                merged.push_back(rightFront[j]);
            }
            j++;
        }
    }

// Add remaining points from leftFront
    while (i < leftFront.size()) {
        if (merged.empty() || merged.back().f2 > leftFront[i].f2) {
            merged.push_back(leftFront[i]);
        }
        i++;
    }

// Add remaining points from rightFront
    while (j < rightFront.size()) {
        if (merged.empty() || merged.back().f2 > rightFront[j].f2) {
            merged.push_back(rightFront[j]);
        }
        j++;
    }

    return merged;
}
