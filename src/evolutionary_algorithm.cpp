
#include "evolutionary_algorithm.h"
#include "Point.h"
#include "evaluation_functions.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <functions.h>
#include <numeric>
#include <cfloat>

// Constants
const int BITS_PER_DIMENSION = 16;
const int MAX_ITER_EXPERIMENT = 100;
const int DIMENSIONS = 10;
const int MAX_ITER = 10000;
const int POP_SIZE_DBL = 10;
const int POP_SIZE_INT = 3;

std::mt19937 rng(std::random_device{}());

template<typename T>
std::vector<T> initializePopulation(double min_value, double max_value) {
    constexpr const int POP_SIZE = std::is_same_v<T, IntPoint> ? POP_SIZE_INT : POP_SIZE_DBL;
    std::vector<T> population(POP_SIZE);
    std::uniform_int_distribution<int> dist(0, 1);

    if constexpr (std::is_same_v<T, IntPoint>) {
        for (auto& individual : population) {
            int point_coordinates = 0;
            for (int i = 0; i < BITS_PER_DIMENSION * DIMENSIONS; ++i) {
                point_coordinates |= (dist(rng) << i);
            }
            individual.SetPoint(point_coordinates);
        }
    } else if constexpr (std::is_same_v<T, VectorPoint>) {
        std::uniform_real_distribution<> dis(min_value, max_value);
        for (auto& individual : population) {
            std::vector<double> point_coordinates(DIMENSIONS);
            for (int i = 0; i < DIMENSIONS; ++i) {
                point_coordinates[i] = dis(rng);
            }
            individual.SetPoint(point_coordinates);
        }
    } else {
        static_assert("Unsupported type for initializePopulation.");
        return 0.0;
    }

    return population;
}

// Specialized one-point crossover function for IntPoint
std::vector<IntPoint> crossover(const IntPoint& p1, const IntPoint& p2, double r_cross=0.5) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> dis_int(1, p1.GetBitsPerDim() * p1.dimensions - 2);

    if (dis(rng) < r_cross) {
        int pt = dis_int(rng);

        int mask = (1 << pt) - 1;
        int new_c1 = (p1.GetPoint() & mask) | (p2.GetPoint() & ~mask);
        int new_c2 = (p2.GetPoint() & mask) | (p1.GetPoint() & ~mask);

        auto c1 = IntPoint(p1.dimensions, p1.GetBitsPerDim(), new_c1);
        auto c2 = IntPoint(p2.dimensions, p2.GetBitsPerDim(), new_c2);

        return {c1, c2};
    }

    return {p1, p2};
}

// Specialized crossover function for VectorPoint
std::vector<VectorPoint> crossover(const VectorPoint& p1, const VectorPoint& p2, double r_cross=0.5) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    // std::normal_distribution<> normal_dis(0.0, 1.0);
    std::uniform_int_distribution<> dis_int(1, p1.GetPoint().size() - 2);

    VectorPoint c1 = p1;
    VectorPoint c2 = p2;

    if (dis(rng) < r_cross) {
        int pt = dis_int(rng);


        for (int i = pt; i < p1.GetPoint().size(); ++i) {
            c1.SetPoint(i,p2.GetPoint()[i]);
            c2.SetPoint(i, p1.GetPoint()[i]);
        }
        //IMO DZIAŁA GORZEJ
        // for(int i = 0; i < p1.GetPoint().size(); ++i) {
        //         double x_a = p1.GetPoint(i);
        //         double x_b = p2.GetPoint(i);
        //         if(x_a > x_b) {
        //             double center = (x_a - x_b) / 2.0;
        //             c1.SetPoint(i, x_a + center * dis(rng));
        //             c2.SetPoint(i, x_a + center * dis(rng));
        //         }
        //         else {
        //             double center = (x_b - x_a) / 2.0;
        //             c1.SetPoint(i, x_b + center * dis(rng));
        //             c2.SetPoint(i, x_a + center * dis(rng));
        //         }
        //
        // }
    }

    return {c1, c2};
}

template<typename T>
T tournamentSelection(const std::vector<T>& population, int eval_function, int tournament_size = 5) {
    constexpr const int POP_SIZE = std::is_same_v<T, IntPoint> ? POP_SIZE_INT : POP_SIZE_DBL;
    if constexpr (std::is_same_v<T, IntPoint>) {
        tournament_size = 3;
    }
    std::uniform_int_distribution<int> dist(0, POP_SIZE - 1);
    T best_individual = population[dist(rng)];
    double best_fitness = evaluate(best_individual, eval_function);

    for (int i = 1; i < tournament_size; ++i) {
        T individual = population[dist(rng)];
        double fitness = evaluate(individual, eval_function);
        if (fitness < best_fitness) {
            best_individual = individual;
            best_fitness = fitness;
        }
    }

    return best_individual;
}

template<typename T>
void mutate(T& individual, double min_value, double max_value) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    if constexpr (std::is_same_v<T, IntPoint>) {
        int total_bits = individual.GetBitsPerDim() * individual.dimensions;
        int coordinates = individual.GetPoint();
        for (int i = 0; i < total_bits; ++i) {
            if (dist(rng) < 0.5) {
                coordinates ^= (1 << i);  // Flip the bit at position i
            }
        }
        individual.SetPoint(coordinates);
    } else if constexpr (std::is_same_v<T, VectorPoint>) {
        std::normal_distribution<double> normal_dis(0,1);
        for (int i = 0; i < DIMENSIONS; ++i) {
            if (dist(rng) < 0.5) {
                //new single coordinate value (add normal distribution(0,1)
                double new_single_coordinate = individual.GetPoint(i) + normal_dis(rng);
                //check if new value is in range
                if (new_single_coordinate < min_value) {
                    new_single_coordinate = min_value;
                } else if (new_single_coordinate > max_value) {
                    new_single_coordinate = max_value;
                }
                //set new value
                individual.SetPoint(i, new_single_coordinate);
            }
        }
    } else {
        static_assert("Unsupported type for mutate.");
    }
}


template<typename T>
std::vector<double> evolutionary_algorithm_real_valued(double m, int evaluation_function, double min_value, double max_value) {
    constexpr const int POP_SIZE = std::is_same_v<T, IntPoint> ? POP_SIZE_INT : POP_SIZE_DBL;
    std::vector<T> population = initializePopulation<T>(min_value, max_value);
    double best_fitness = DBL_MAX;
    std::vector<double> evaluation_values;

    //vector for evaluation values of single population
    std::vector<double> evaluation_values_population(POP_SIZE);
    //indexes of best and second best individual in the single population
    int best_individual_index = 0;
    int second_best_individual_index = 0;

    for (int iter = 0; iter < MAX_ITER; ++iter) {
        //calculate evaluation values for each individual in the population
        for (int i = 0; i < POP_SIZE; ++i) {
            double evaluation_value = evaluate(population[i], evaluation_function);
            evaluation_values_population[i] = evaluation_value;
            if (evaluation_value < best_fitness) {
                best_fitness = evaluation_value;
                second_best_individual_index = best_individual_index;
                best_individual_index = i;
            } else if (evaluation_value < evaluation_values_population[second_best_individual_index] || best_individual_index == second_best_individual_index) {
                second_best_individual_index = i;
            }
        }
        //save the best fitness value
        evaluation_values.push_back(best_fitness);
        std::vector<T> p_prime;

        //to the EMPTY p_prime vector add the best and second-best individual from the previous population
        p_prime.push_back(population[best_individual_index]);
        p_prime.push_back(population[second_best_individual_index]);

        while(p_prime.size() < POP_SIZE) {
            //tutaj też można zoptymalizować i zamiast tworzyć nowe punkty to zwracać indkesy istneijących ale to mała zmiana chyba
            T parent_1 = tournamentSelection(population, evaluation_function);
            T parent_2 = tournamentSelection(population, evaluation_function);

            //wydaje mi się że to można jeszcze zoptymalizować i zamiast tworzyć nowe punkty to zmeiniać istniejące tak jak w mutate
            auto offspring = crossover(parent_1, parent_2);

            mutate(offspring[0], min_value, max_value);
            mutate(offspring[1], min_value, max_value);

            p_prime.push_back(offspring[0]);
            p_prime.push_back(offspring[1]);
        }

        population = p_prime;
    }

    return evaluation_values;
}

template<typename T>
std::vector<std::vector<double>> run_simulation(double m, int evaluation_function, std::string numbers_representation) {
    std::vector<std::vector<double>> all_evaluation_series;
    std::vector<long> execution_times;

    double min_value = 0, max_value = 0;

    if(evaluation_function == 1) {
        min_value = -3;
        max_value = 3;
    }
    else if(evaluation_function == 2) {
        min_value = -32.768;
        max_value = 32.768;
    }

    for (int i = 0; i < MAX_ITER_EXPERIMENT; ++i) {
        std::cout << "start iter:  " << i << std::endl;
        auto start_time = std::chrono::high_resolution_clock::now();

        if (numbers_representation == "binary") {
            auto evaluation_values = evolutionary_algorithm_real_valued<IntPoint>(m, evaluation_function, min_value, max_value);
            all_evaluation_series.push_back(evaluation_values);
        } else if (numbers_representation == "double") {
            auto evaluation_values = evolutionary_algorithm_real_valued<VectorPoint>(m, evaluation_function, min_value, max_value);
            all_evaluation_series.push_back(evaluation_values);
        } else {
            std::cerr << "Unsupported numbers representation" << std::endl;
        }

        auto end_time = std::chrono::high_resolution_clock::now();

        long execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        std::cout << "clock: " << execution_time <<std::endl;
        execution_times.push_back(execution_time);
    }
    save_execution_times_to_csv(execution_times, "execution_times_evolutionary_fun_" + std::to_string(evaluation_function) + "_" + numbers_representation + ".csv");
    return all_evaluation_series;
}

std::vector<std::vector<double>> exercise4(int evaluation_function, std::string numbers_representation) {
    //mutation probability
    double m = 4;
    std::vector<std::vector<double>> all_evaluation_series;

    if(numbers_representation == "binary") {
        all_evaluation_series = run_simulation<IntPoint>(m, evaluation_function, numbers_representation);
    }
    else if (numbers_representation == "double") {
        all_evaluation_series = run_simulation<VectorPoint>(m, evaluation_function, numbers_representation);
    }

    return all_evaluation_series;
}