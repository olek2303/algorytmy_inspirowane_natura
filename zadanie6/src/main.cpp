#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <random>
#include <set>
#include <string>
#include <iomanip>

struct Point {
    double f1, f2;

    bool operator==(const Point &other) const {
        return std::fabs(f1 - other.f1) < 1e-6 && std::fabs(f2 - other.f2) < 1e-6;
    }
};

class ParetoAlgorithm {
public:
    ParetoAlgorithm(bool f1Minimize, bool f2Maximize)
            : f1Minimize(f1Minimize), f2Maximize(f2Maximize) {}

    std::vector<Point> naiveParetoFront(const std::vector<Point> &points) {
        std::vector<Point> front;
        for (const auto &point : points) {
            if (!isDominated(point, points)) {
                front.push_back(point);
            }
        }
        return front;
    }

    std::vector<Point> kungParetoFront(std::vector<Point> points) {
        if (points.size() <= 1) {
            return points;
        }

        auto comp = f1Minimize ? [](const Point &a, const Point &b) { return a.f1 < b.f1; }
                               : [](const Point &a, const Point &b) { return a.f1 > b.f1; };
        std::sort(points.begin(), points.end(), comp);

        size_t mid = points.size() / 2;
        std::vector<Point> left(points.begin(), points.begin() + mid);
        std::vector<Point> right(points.begin() + mid, points.end());

        std::vector<Point> leftFront = kungParetoFront(left);
        std::vector<Point> rightFront = kungParetoFront(right);

        std::vector<Point> merged;
        for (const auto &point : leftFront) {
            if (!isDominated(point, rightFront)) {
                merged.push_back(point);
            }
        }
        merged.insert(merged.end(), rightFront.begin(), rightFront.end());

        return merged;
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

private:
    bool f1Minimize;
    bool f2Maximize;

    bool isDominated(const Point &point, const std::vector<Point> &others) {
        for (const auto &other : others) {
//            if ((f1Minimize ? other.f1 <= point.f1 : other.f1 >= point.f1) &&
//                (f2Maximize ? other.f2 >= point.f2 : other.f2 <= point.f2)) {
//                return true;
//            }
            if ((other.f1 < point.f1 && other.f2 >= point.f2) || (other.f1 <= point.f1 && other.f2 > point.f2)) {
                return true;
            }
        }
        return false;
    }
};

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
                std::cout << std::fixed << std::setprecision(3) << "(" << point.f1 << ", " << point.f2 << ")\n";
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
                file << std::fixed << std::setprecision(3)  << point.f1 << " " << point.f2;
            }
            file << "\n";
        }

        file.close();
    }
};

int main() {
    size_t nPoints = 1000;
    auto data = DataGenerator::generateSampleData(nPoints);

    ParetoAlgorithm naiveAlgorithm(true, true);
    // ParetoAlgorithm kungAlgorithm(true, true);

    std::cout << "Finding fronts using the naive algorithm...\n";
    auto naiveFronts = naiveAlgorithm.findAllFronts(data, "naive");
    std::cout << "Number of fronts (naive): " << naiveFronts.size() << "\n\n";
    Visualizer::visualizeFronts(naiveFronts);
    // Save fronts to a text file
    Visualizer::saveFrontsToFile(naiveFronts, "pareto_fronts.txt");

    // std::cout << "Finding fronts using the Kung algorithm...\n";
    // auto kungFronts = kungAlgorithm.findAllFronts(data, "kung");
    // std::cout << "Number of fronts (Kung): " << kungFronts.size() << "\n\n";
    // Visualizer::visualizeFronts(kungFronts);

    return 0;
}