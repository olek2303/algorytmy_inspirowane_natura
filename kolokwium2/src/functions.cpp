#include "../include/functions.h"

double clamp(double value, double min, double max) {
    return std::max(min, std::min(value, max));
}

void writeArchive(const std::vector<Individual>& archive, const std::string& filename) {
    std::ofstream file(filename);
    for (int i = 0; i < archive.size(); ++i) {
        file << archive[i].objectives[0] << " " << archive[i].objectives[1] << "\n";
    }
}
std::vector<Individual> findParetoFront(const std::vector<Individual>& individuals) {
    std::vector<Individual> paretoFront;
    for (const auto& ind : individuals) {
        bool dominated = false;
        for (const auto& other : individuals) {
            if (other.objectives[0] <= ind.objectives[0] &&
                other.objectives[1] <= ind.objectives[1] &&
                (other.objectives[0] != ind.objectives[0] || other.objectives[1] != ind.objectives[1])) {
                dominated = true;
                break;
                }
        }
        if (!dominated) {
            paretoFront.push_back(ind);
        }
    }
    return paretoFront;
}

void writeParetoFront(const std::vector<Individual>& archive, const std::string& filename) {
    auto paretoFront = findParetoFront(archive);
    std::ofstream file(filename);
    for (int i = 0; i < paretoFront.size(); ++i) {
        file << paretoFront[i].objectives[0] << " " << paretoFront[i].objectives[1] << "\n";
    }
}