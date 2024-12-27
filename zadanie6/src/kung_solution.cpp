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
    for (const auto &point : leftFront) {
//        if (!isDominated(point, rightFront)) {
//            merged.push_back(point);
//        }
        bool isDominated = false;
        for (const auto &f : rightFront) {
            if (f.f2 > point.f2) {
                isDominated = true;
                break;
            }
        }
        if (!isDominated) {
            merged.push_back(point);
        }

    }
    merged.insert(merged.end(), rightFront.begin(), rightFront.end());

    return merged;
}
