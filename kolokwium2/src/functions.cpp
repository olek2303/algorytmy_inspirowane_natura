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
std::vector<Individual> findNotDominated(const std::vector<Individual>& individuals) {
    std::vector<Individual> paretoFront;
    for (const auto& ind : individuals) {
        if(ind.strength == 0)
            paretoFront.push_back(ind);
    }
    return paretoFront;
}

void writeParetoFront(const std::vector<Individual>& archive, const std::string& filename) {
    auto paretoFront = findNotDominated(archive);
    std::ofstream file(filename);
    for (int i = 0; i < paretoFront.size(); ++i) {
        file << paretoFront[i].objectives[0] << " " << paretoFront[i].objectives[1] << "\n";
    }
}