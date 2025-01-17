
#ifndef GLOBALS_H
#define GLOBALS_H

constexpr int populationSize = 500;
constexpr int numGenerations = 1000;
const int numVariables = 2; //must be 2 for ZDT1 (it's m)
const double mutationRate = 0.8;
const double mutationStrength = 0.7;
const int archiveSize = 500;
const int tournamentSize = 5;
const int densityNeighbours = 5;
const double crossoverStrength = 0.5;
#endif //GLOBALS_H
