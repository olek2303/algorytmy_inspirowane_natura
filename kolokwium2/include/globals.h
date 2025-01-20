
#ifndef GLOBALS_H
#define GLOBALS_H

constexpr int numGenerations = 500;
constexpr int populationSize = 150;
const int archiveSize = 150;
const int numVariables = 50;

// const int numVariables = 30;
// const int numVariables = 50;



const double mutationRate = 0.8;
const double mutationStrength = 0.1;

const int tournamentSize = 15;
const int densityNeighbours = 15;

const double crossoverStrength = 0.1;
#endif //GLOBALS_H
