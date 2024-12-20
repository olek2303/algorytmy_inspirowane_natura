#include <iostream>
#include <algorithm>
#include "../include/representation.h"
#include "../include/utils.h"
#include "../include/functions.h"

int main() {
    size_t nPoints = 1000;
    auto data = DataGenerator::generateSampleData(nPoints);
    // ParetoAlgorithm kungAlgorithm(true, true);

    std::cout << "Finding fronts using the naive algorithm...\n";
    auto naiveFronts = findAllFronts(data, "naive");
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