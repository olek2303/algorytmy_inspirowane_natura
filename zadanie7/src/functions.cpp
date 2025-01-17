#include "../include/functions.h"

double clamp(double value, double min, double max) {
    return std::max(min, std::min(value, max));
}

void writeParetoFront(const std::vector<Individual>& population, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& ind : population) {
        file << ind.objectives[0] << " " << ind.objectives[1] << "\n";
    }
}