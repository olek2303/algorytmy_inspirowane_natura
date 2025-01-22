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
#include "evaluate.h"

std::vector<Individual> initializePopulation(int populationSize, std::mt19937& rng, Evaluator* evaluate);
void calculateStrength(std::vector<Individual>& population);
void calculateDensity(std::vector<Individual>& population, int n_neighbours);
void calculateFitness(std::vector<Individual>& population);
std::vector<Individual> tournamentSelection(const std::vector<Individual>& population, int tournamentSize, int selectionSize, std::mt19937& rng);
Individual crossover(const Individual& parent1, const Individual& parent2, std::mt19937& rng, Evaluator* evaluate);
void mutate(Individual& individual, std::mt19937& rng, Evaluator* evaluate);
void spea2(std::mt19937 rng, Evaluator* evaluate);
#endif //SPEA2_H
