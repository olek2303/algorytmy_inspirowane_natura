
#ifndef GLOBALS_H
#define GLOBALS_H

constexpr int numGenerations = 500;
constexpr int populationSize = 150;
const int archiveSize = 150;
const int numVariables = 10;

// const int numVariables = 30;
// const int numVariables = 50;



const double mutationRate = 0.8;
const double mutationStrength = 0.7;

const int tournamentSize = 5;
const int densityNeighbours = 5;

const double crossoverStrength = 0.5;
#endif //GLOBALS_H
