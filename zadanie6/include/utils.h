//
// Created by admin on 20.12.2024.
//

#ifndef MYPROJECT_UTILS_H
#define MYPROJECT_UTILS_H

#include "representation.h"
#include <vector>
#include <random>
#include <iostream>
#include <fstream>

using namespace std;

class DataGenerator {
public:
    static std::vector<Point> generateSampleData(size_t nPoints) {
        std::vector<Point> data;
        std::mt19937 gen(42);
        std::uniform_real_distribution<> dis(0.0, 1.0);

        for (size_t i = 0; i < nPoints; ++i) {
            double x1 = dis(gen);
            double f1 = x1;
            double f2 = 1 - std::sqrt(f1 / (1 + 9 * (0 / (2 - 1)))); // m=2, sum from i=2 to m xi/(m-1) is 0
            data.push_back({f1, f2});
        }

        return data;
    }
};

class Visualizer {
public:
    static void visualizeFronts(const std::vector<std::vector<Point>> &fronts) {
        for (size_t i = 0; i < std::min(fronts.size(), size_t(5)); ++i) {
            for (const auto &point : fronts[i]) {
                std::cout << std::fixed << "(" << point.f1 << ", " << point.f2 << ")\n";
            }
            std::cout << "\n";
        }
    }

    static void saveFrontsToFile(const std::vector<std::vector<Point>> &fronts, const std::string &filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << "\n";
            return;
        }

        for (size_t i = 0; i < fronts.size(); ++i) {
            for (const auto &point : fronts[i]) {
                file << std::fixed  << point.f1 << " " << point.f2;
            }
            file << "\n";
        }

        file.close();
    }
};

#endif //MYPROJECT_UTILS_H
