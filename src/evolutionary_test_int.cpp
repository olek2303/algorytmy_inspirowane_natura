#include<chrono>
#include<vector>
#include<random>
#include<iostream>
#include"Point.h"
#include "evaluation_functions.h"

const int BITS_PER_DIMENSION = 16;
const int MAX_ITER_EXPERIMENT = 10;
const int DIMENSIONS = 10;
const int MAX_ITER = 10000;
const int POP_SIZE = 100;
const int GENERATIONS = 100;

std::mt19937 rng(std::random_device{}());

// Reprezentacja binarna i double
// Operator sasiedztwa pokazany na slajdach

// 1. Inicjalizacja populacji
std::vector<IntPoint> initializePopulation() {
    std::vector<IntPoint> population(POP_SIZE);
    std::uniform_int_distribution<int> dist(0, 1);
    for (auto& individual : population) {
        int point_coordinates = 0;
        for (int i = 0; i < BITS_PER_DIMENSION * DIMENSIONS; ++i) {
            point_coordinates |= (dist(rng) << i);
        }
        individual.SetPoint(point_coordinates);
    }
    return population;
}

// 2. Algorytm selekcji z wykladu
IntPoint tournamentSelection(const std::vector<IntPoint>& population) {
    std::uniform_int_distribution<int> dist(0, POP_SIZE - 1);
    int a = dist(rng);
    int b = dist(rng);
    return (evaluation_function_1(population[a]) > evaluation_function_1(population[b])) ? population[a] : population[b];
}

int fitness(const IntPoint& individual) {
    int sum = 0;
    int coordinates = individual.GetPoint();
    int total_bits = individual.GetBitsPerDim() * individual.dimensions;
    for (int i = 0; i < total_bits; ++i) {
        sum += (coordinates >> i) & 1;
    }
    return sum;
}

// 3. Krzyzowanie jednopunktowe
IntPoint crossover(const IntPoint& parent1, const IntPoint& parent2) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    if (dist(rng) > 0.7) return parent1;

    int total_bits = parent1.GetBitsPerDim() * parent1.dimensions;
    std::uniform_int_distribution<int> crossover_point_dist(0, total_bits - 1);
    int crossover_point = crossover_point_dist(rng);

    int coordinates1 = parent1.GetPoint();
    int coordinates2 = parent2.GetPoint();

    for (int i = crossover_point; i < total_bits; ++i) {
        bool bit1 = (coordinates1 >> i) & 1;
        bool bit2 = (coordinates2 >> i) & 1;

        if (bit1 != bit2) {
            coordinates1 ^= (1 << i);
            coordinates2 ^= (1 << i);
        }
    }

    return IntPoint(parent1.dimensions, parent1.GetBitsPerDim(), coordinates1);
}

void mutate(IntPoint& individual) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    int total_bits = individual.GetBitsPerDim() * individual.dimensions;
    int coordinates = individual.GetPoint();
    for (int i = 0; i < total_bits; ++i) {
        if (dist(rng) < 0.5) {
            coordinates ^= (1 << i);  // Flip the bit at position i
        }
    }
    individual.SetPoint(coordinates);
}

// 4. Algorytm ewolucyjny z elementami sukcesji
double evolutionaryAlgorithm() {
    auto population = initializePopulation();
    double best_fitness = 0;
    int evaluations = 0;

    for (int generation = 0; generation < GENERATIONS && evaluations < MAX_ITER; ++generation) {
        std::vector<IntPoint> new_population;

        // Tworzenie nowej populacji przez selekcję, krzyżowanie i mutację
        while (new_population.size() < POP_SIZE) {
            auto parent1 = tournamentSelection(population);
            auto parent2 = tournamentSelection(population);

            //TO TRZEBA PRZEROBIC

            auto offspring = crossover(parent1, parent2);
            mutate(offspring);

            // Ocena przystosowania i aktualizacja najlepszego wyniku
            int offspring_fitness = fitness(offspring);
            evaluations++;
            // best_fitness = std::max(best_fitness, offspring_fitness);
            best_fitness = std::max(best_fitness, evaluation_function_1(parent1));
            new_population.push_back(offspring);
            new_population.push_back(parent1);
            new_population.push_back(parent2);
        }

        // Aktualizacja populacji
        population = new_population;
    }

    return best_fitness;
}

// Funkcja do przeprowadzenia 100 eksperymentów i zmierzenia czasu ich trwania
void runExperiments() {

    for (int i = 0; i < MAX_ITER_EXPERIMENT; ++i) {
        auto start = std::chrono::high_resolution_clock::now();

        // Uruchomienie pojedynczego eksperymentu
        double best_fitness = evolutionaryAlgorithm();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;


        std::cout << "Eksperyment " << i + 1 << ": Czas trwania = " << elapsed.count() << "s, Najlepsza przystosowanie = " << best_fitness << "\n";
    }

}


//KOD Z CZATU
// #include <iostream>
// #include <vector>
// #include <random>
// #include <bitset>
// #include <algorithm>
// #include <chrono>
// #include <ctime>
//
// constexpr int N = 10;            // Wymiary
// constexpr int POP_SIZE = 100;     // Rozmiar populacji
// constexpr int GENERATIONS = 100;
// constexpr double CROSSOVER_RATE = 0.7;
// constexpr double MUTATION_RATE = 0.01;
// constexpr int EXPERIMENTS = 100;  // Liczba eksperymentów
// constexpr int EVALUATIONS = 10000; // Liczba wywołań funkcji oceny na eksperyment
//
// std::mt19937 rng(std::random_device{}());
//
// // Funkcja przystosowania: Suma wartości wszystkich wymiarów.
// int fitness(const std::vector<int>& individual) {
//     int sum = 0;
//     for (int value : individual) {
//         sum += value;
//     }
//     return sum;
// }
//
// // Funkcja selekcji turniejowej
// std::vector<int> tournamentSelection(const std::vector<std::vector<int>>& population) {
//     std::uniform_int_distribution<int> dist(0, POP_SIZE - 1);
//     int a = dist(rng);
//     int b = dist(rng);
//     return (fitness(population[a]) > fitness(population[b])) ? population[a] : population[b];
// }
//
// // Funkcja krzyżowania jednokropkowego
// std::vector<int> crossover(const std::vector<int>& parent1, const std::vector<int>& parent2) {
//     std::uniform_real_distribution<double> dist(0.0, 1.0);
//     if (dist(rng) > CROSSOVER_RATE) return parent1;
//
//     std::vector<int> offspring(N);
//     std::uniform_int_distribution<int> crossover_point_dist(0, 15); // Punkt krzyżowania na bitach
//     int crossover_point = crossover_point_dist(rng);
//
//     for (int i = 0; i < N; ++i) {
//         std::bitset<16> bits1(parent1[i]);
//         std::bitset<16> bits2(parent2[i]);
//
//         // Krzyżowanie w punkcie
//         for (int j = 0; j < 16; ++j) {
//             if (j > crossover_point) {
//                 bool temp = bits1[j];
//                 bits1[j] = bits2[j];
//                 bits2[j] = temp;
//
//             }
//         }
//         offspring[i] = static_cast<int>(bits1.to_ulong());
//     }
//     return offspring;
// }
//
// // Funkcja mutacji
// void mutate(std::vector<int>& individual) {
//     std::uniform_real_distribution<double> dist(0.0, 1.0);
//     for (int i = 0; i < N; ++i) {
//         std::bitset<16> bits(individual[i]);
//         for (int j = 0; j < 16; ++j) {
//             if (dist(rng) < MUTATION_RATE) {
//                 bits.flip(j);  // Zamiana bitu
//             }
//         }
//         individual[i] = static_cast<int>(bits.to_ulong());
//     }
// }
//
// // Inicjalizacja populacji
// std::vector<std::vector<int>> initializePopulation() {
//     std::vector<std::vector<int>> population(POP_SIZE, std::vector<int>(N));
//     std::uniform_int_distribution<int> dist(0, 65535); // 16-bitowe liczby
//     for (auto& individual : population) {
//         for (auto& value : individual) {
//             value = dist(rng);
//         }
//     }
//     return population;
// }
//
// // Algorytm ewolucyjny - pojedynczy eksperyment
// int evolutionaryAlgorithm() {
//     auto population = initializePopulation();
//     int best_fitness = 0;
//     int evaluations = 0;
//
//     for (int generation = 0; generation < GENERATIONS && evaluations < EVALUATIONS; ++generation) {
//         std::vector<std::vector<int>> new_population;
//
//         // Tworzenie nowej populacji przez selekcję, krzyżowanie i mutację
//         while (new_population.size() < POP_SIZE && evaluations < EVALUATIONS) {
//             auto parent1 = tournamentSelection(population);
//             auto parent2 = tournamentSelection(population);
//
//             auto offspring = crossover(parent1, parent2);
//             mutate(offspring);
//
//             // Ocena przystosowania i aktualizacja najlepszego wyniku
//             int offspring_fitness = fitness(offspring);
//             evaluations++;
//             best_fitness = std::max(best_fitness, offspring_fitness);
//
//             new_population.push_back(offspring);
//         }
//
//         // Aktualizacja populacji
//         population = new_population;
//     }
//
//     return best_fitness;
// }
//
// // Funkcja do przeprowadzenia 100 eksperymentów i zmierzenia czasu ich trwania
// void runExperiments() {
//     double total_time = 0.0;
//     int total_best_fitness = 0;
//
//     for (int i = 0; i < EXPERIMENTS; ++i) {
//         auto start = std::chrono::high_resolution_clock::now();
//
//         // Uruchomienie pojedynczego eksperymentu
//         int best_fitness = evolutionaryAlgorithm();
//
//         auto end = std::chrono::high_resolution_clock::now();
//         std::chrono::duration<double> elapsed = end - start;
//
//         total_time += elapsed.count();
//         total_best_fitness += best_fitness;
//
//         std::cout << "Eksperyment " << i + 1 << ": Czas trwania = " << elapsed.count() << "s, Najlepsza przystosowanie = " << best_fitness << "\n";
//     }
//
//     // Obliczenie średnich wyników
//     double avg_time = total_time / EXPERIMENTS;
//     int avg_best_fitness = total_best_fitness / EXPERIMENTS;
//
//     std::cout << "\nŚredni czas trwania eksperymentu: " << avg_time << "s\n";
//     std::cout << "Średnia najlepsza przystosowanie: " << avg_best_fitness << "\n";
// }
//
// int main() {
//     runExperiments();
//     return 0;
// }
