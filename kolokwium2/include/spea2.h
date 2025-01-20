#ifndef SPEA2_H
#define SPEA2_H
#include <vector>
#include  <numeric>
#include <cmath>
#include <algorithm>
#include <random>
#include "individual.h"
#include "functions.h"
#include "globals.h"

std::vector<double> evaluateZDT1(const std::vector<double>& variables);
std::vector<Individual> initializePopulation(int populationSize, int numVariables, std::mt19937& rng);
void calculateStrength(std::vector<Individual>& population);
void calculateDensity(std::vector<Individual>& population, int n_neighbours);
void calculateFitness(std::vector<Individual>& population);
std::vector<Individual> tournamentSelection(const std::vector<Individual>& population, int tournamentSize, int selectionSize, std::mt19937& rng);
Individual crossover(const Individual& parent1, const Individual& parent2, std::mt19937& rng, double alpha);
void mutate(Individual& individual, double mutationRate, double mutationStrength, std::mt19937& rng);
std::vector<Individual> spea2 (std::vector<Individual> population, std::mt19937 rng);
#endif //SPEA2_H
