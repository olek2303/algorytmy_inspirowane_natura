#include <iostream>
#include <algorithm>
#include "../include/representation.h"
#include "../include/utils.h"
#include "../include/functions.h"

int main() {
    size_t nPoints = 1000;
    auto data = DataGenerator::readSampleData("../data.txt");
     // auto data = DataGenerator::generateSampleData(nPoints);

    std::cout << "Finding fronts using the naive algorithm...\n";
    auto naiveFronts = findAllFronts(data, "naive");
    std::cout << "Number of fronts (naive): " << naiveFronts.size() << "\n\n";
//    Visualizer::visualizeFronts(naiveFronts);
    // Save fronts to a text file
    Visualizer::saveFrontsToFile(naiveFronts, "pareto_fronts_naive.txt");

     std::cout << "Finding fronts using the Kung algorithm...\n";
     auto kungFronts = findAllFronts(data, "kung");
     std::cout << "Number of fronts (Kung): " << kungFronts.size() << "\n\n";
//     Visualizer::visualizeFronts(kungFronts);
     Visualizer::saveFrontsToFile(kungFronts, "pareto_fronts_kung.txt");

    return 0;
}