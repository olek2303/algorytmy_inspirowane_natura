#include "../include/evaluation_functions.h"
#include "../include/Float_representation.h"
#include "../include/operators_and_selection.h"
#include "../include/evolutionary_algorithm.h"
#include <vector>
#include <iostream>
#include <numeric>
#include <cfloat>
#include <random>

std::mt19937 rng2(std::random_device{}());

void initializePopulation(std::vector<Float_representation>& population, double min_value, double max_value) {
    std::uniform_real_distribution<> dis(min_value, max_value);
    for (auto& individual : population) {
        for (int i = 0; i < individual.GetDimensions(); ++i) {
            individual.SetPoint(i, dis(rng2));
        }
    }
}
//TODO czekam na odpowiedz Trojanowskiego, nie ruszam na razie bo nie wiem jak to ma działać tak szczerze
// std::vector<double> evolutionary_algorithm(double m, int evaluation_function, double min_value, double max_value, int POP_SIZE, int dimensions) {
//     //inicjalizacja początkowej populacji
     //std::vector<Float_representation> population(POP_SIZE, Float_representation(dimensions);
//     initializePopulation(population, min_value, max_value);
//
//     double best_fitness = DBL_MAX;
//     std::vector<double> evaluation_values;
//
//     //vector for evaluation values of single population
//     std::vector<double> evaluation_values_population(POP_SIZE);
//     //indexes of best and second best individual in the single population
//     int best_individual_index = 0;
//     int second_best_individual_index = 0;
//
//     for (int iter = 0; iter < MAX_ITER; ++iter) {
//         //calculate evaluation values for each individual in the population
//         for (int i = 0; i < POP_SIZE; ++i) {
//             double evaluation_value = evaluate(population[i], evaluation_function);
//             evaluation_values_population[i] = evaluation_value;
//             if (evaluation_value < best_fitness) {
//                 best_fitness = evaluation_value;
//                 second_best_individual_index = best_individual_index;
//                 best_individual_index = i;
//             } else if (evaluation_value < evaluation_values_population[second_best_individual_index] || best_individual_index == second_best_individual_index) {
//                 second_best_individual_index = i;
//             }
//         }
//         //save the best fitness value
//         evaluation_values.push_back(best_fitness);
//         std::vector<T> p_prime;
//
//         //to the EMPTY p_prime vector add the best and second-best individual from the previous population
//         p_prime.push_back(population[best_individual_index]);
//         p_prime.push_back(population[second_best_individual_index]);
//
//         while(p_prime.size() < POP_SIZE) {
//             //tutaj też można zoptymalizować i zamiast tworzyć nowe punkty to zwracać indkesy istneijących ale to mała zmiana chyba
//             T parent_1 = tournamentSelection(population, evaluation_function);
//             T parent_2 = tournamentSelection(population, evaluation_function);
//
//             //wydaje mi się że to można jeszcze zoptymalizować i zamiast tworzyć nowe punkty to zmeiniać istniejące tak jak w mutate
//             auto offspring = crossover(parent_1, parent_2);
//
//             mutate(offspring[0], min_value, max_value);
//             mutate(offspring[1], min_value, max_value);
//
//             p_prime.push_back(offspring[0]);
//             p_prime.push_back(offspring[1]);
//         }
//
//         population = p_prime;
//     }
//
//     return evaluation_values;
// }
