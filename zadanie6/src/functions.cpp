//
// Created by admin on 20.12.2024.
//

#include "../include/functions.h"

bool isDominated(const Point &point, const std::vector<Point> &others) {
    for (const auto &other : others) {
        if ((other.f1 < point.f1 && other.f2 >= point.f2) || (other.f1 <= point.f1 && other.f2 > point.f2)) {
            return true;
        }
    }
    return false;
}

std::vector<std::vector<Point>> findAllFronts(const std::vector<Point> &points,
                                              const std::string &method) {
    std::vector<Point> remaining = points;
    std::vector<std::vector<Point>> fronts;

    while (!remaining.empty()) {
        std::vector<Point> front;
        if (method == "naive") {
            front = naiveParetoFront(remaining);
        } else if (method == "kung") {
            front = kungParetoFront(remaining);
        }

        fronts.push_back(front);

        std::vector<Point> newRemaining;
        for (const auto &point : remaining) {
            bool inFront = false;
            for (const auto &f : front) {
                if (std::fabs(point.f1 - f.f1) < 1e-6 && std::fabs(point.f2 - f.f2) < 1e-6) {
                    inFront = true;
                    break;
                }
            }
            if (!inFront) {
                newRemaining.push_back(point);
            }
        }
        remaining = std::move(newRemaining);
    }

    return fronts;
}