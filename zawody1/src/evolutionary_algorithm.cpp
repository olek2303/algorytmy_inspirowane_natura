#include "../include/evaluation_functions.h"
#include "../include/Float_representation.h"
#include "../include/operators_and_selection.h"
#include "../include/evolutionary_algorithm.h"
#include <vector>
#include <iostream>
#include <numeric>
#include <cfloat>
#include <random>
#include <algorithm>

std::mt19937 rng2(std::random_device{}());

void initializePopulation(std::vector<Float_representation>& population, double min_value, double max_value) {
    std::uniform_real_distribution<> dis(min_value, max_value);
    for (auto& individual : population) {
        for (int i = 0; i < individual.GetDimensions(); ++i) {
            individual.SetPoint(i, dis(rng2));
        }
    }
}

 std::vector<double> evolutionary_algorithm(double m, int evaluation_function, double min_value, double max_value, int pop_size, int dimensions, int tournament_size) {
     //inicjalizacja początkowej populacji
     std::vector<Float_representation> population(pop_size, Float_representation(dimensions));
     initializePopulation(population, min_value, max_value);

     double best_fitness = DBL_MAX;
     std::vector<double> evaluation_values;

     //vector for evaluation values of single population
     std::vector<double> evaluation_values_population(pop_size);
     //indexes of best and second best individual in the single population
     int best_individual_index = 0;
     int second_best_individual_index = 0;


     while(function_call_count < 10000 * dimensions) {

         //calculate evaluation values for each individual in the population
         for (int i = 0; i < pop_size; ++i) {
             double evaluation_value = 0.0;
             if(function_call_count >= 10000 * dimensions)
                 return evaluation_values;
             evaluation_value = evaluate(population[i], evaluation_function);

             //każde wywowałnie funkcji ewaluacji jest zapisywane. Jeżeli dopasowanie jest lepsze niż poprzednio to je zapisuejmy, jak nie to kopiujemy ostatni wynik
             if(evaluation_values.empty())
                 evaluation_values.push_back(evaluation_value);
             else if(evaluation_value < evaluation_values.back())
                 evaluation_values.push_back(evaluation_value);
             else
                 evaluation_values.push_back(evaluation_values.back());

             evaluation_values_population[i] = evaluation_value;
         }

         const double SELECTION_RATE = 0.20; // 25% do zachowania

         // Zmodyfikowany kod:
         std::vector<Float_representation> p_prime;

         // Para ocena i indeks
         std::vector<std::pair<double, int>> evaluation_index_pairs;

         for (int i = 0; i < population.size(); ++i) {
             evaluation_index_pairs.push_back({evaluation_values_population[i], i});
         }

         // Posortuj po wartości oceny rosnąco
         std::sort(evaluation_index_pairs.begin(), evaluation_index_pairs.end());

         // Oblicz liczbę osobników do zachowania
         int num_to_keep = std::max(1, static_cast<int>(SELECTION_RATE * pop_size));

         // Zachowaj najlepsze 20%
         for (int i = 0; i < num_to_keep; ++i) {
             int individual_index = evaluation_index_pairs[i].second;
             p_prime.push_back(population[individual_index]);
         }



        // std::cout << "PRZED: " << function_call_count << std::endl;
         while(p_prime.size() < pop_size) {
             Float_representation parent_1 = tournamentSelection(population, tournament_size, evaluation_values_population);
             Float_representation parent_2 = tournamentSelection(population, tournament_size, evaluation_values_population);

             auto offspring = crossover(parent_1, parent_2);

             double adaptive_mutation_rate = std::max(0.01, 1.0 - (double)function_call_count / (10000 * dimensions));
             mutate(offspring[0], min_value, max_value, adaptive_mutation_rate);
             mutate(offspring[1], min_value, max_value, adaptive_mutation_rate);

             p_prime.push_back(offspring[0]);
             p_prime.push_back(offspring[1]);
         }

         population = p_prime;
     }

     return evaluation_values;
 }
