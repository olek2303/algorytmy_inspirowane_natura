#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include "../include/individual.h"
#include "../include/functions.h"
#include "../include/spea2.h"
#include "../include/globals.h"




int main() {
    std::random_device rd;
    std::mt19937 rng(rd());

    auto population = initializePopulation(populationSize, numVariables, rng);

    auto archive = spea2(population, rng);

    // writeParetoFront(archive, "pareto_front.txt");
    std::cout << "Optimization completed. Pareto front written to pareto_front.txt" << std::endl;

    return 0;
}
