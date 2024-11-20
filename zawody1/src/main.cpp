#include <iostream>
#include "../include/evolutionary_algorithm.h"
#include "../include/evaluation_functions.h"
#include "../include/Float_representation.h"
#include <fstream>

int function_call_count = 0;

void run_experiment() {
    int n_of_experimetns = 1;
    int n_of_dim_1 = 5;
    int n_of_dim_2 = 10;
    int n_of_dim_3 = 15;

    for(int i =0; i < n_of_experimetns; i++) {
        std::vector<double> rosenbrock_eval_5_dim = evolutionary_algorithm(0.5, 1, -30, 30, 10, n_of_dim_1);
        std::cout << "Liczba rekordów: " << rosenbrock_eval_5_dim.size() << std::endl;
        std::cout << "Best fitness: " << rosenbrock_eval_5_dim.back() << std::endl;
        // std::vector<double> salomon_eval_5_dim = evolutionary_algorithm(0.5, 2, -100, 100, 10, n_of_dim_1);
        // std::vector<double> whitney_eval_5_dim =evolutionary_algorithm(0.5, 3, -10.24, 10.24, 10, n_of_dim_1);
        //
        // std::vector<double> rosenbrock_eval_10_dim = evolutionary_algorithm(0.5, 1, -30, 30, 10, n_of_dim_2);
        // std::vector<double> salomon_eval_10_dim = evolutionary_algorithm(0.5, 2, -100, 100, 10, n_of_dim_2);
        // std::vector<double> whitney_eval_10_dim = evolutionary_algorithm(0.5, 3, -10.24, 10.24, 10, n_of_dim_2);
        //
        // std::vector<double> rosenbrock_eval_15_dim = evolutionary_algorithm(0.5, 1, -30, 30, 10, n_of_dim_3);
        // std::vector<double> salomon_eval_15_dim = evolutionary_algorithm(0.5, 2, -100, 100, 10, n_of_dim_3);
        // std::vector<double> whitney_eval_15_dim = evolutionary_algorithm(0.5, 3, -10.24, 10.24, 10, n_of_dim_3);

    }

    //trzeba to jakoś zapisac jeszcze żeby kolumny to były eksperymenty, a wiersze to wartości z tych wektorów


}

void save_test_to_csv(const std::vector<Float_representation>& population, const std::vector<double>& evaluation_values, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "Population,Evaluation\n";
        for (size_t i = 0; i < population.size(); ++i) {
            for (int j = 0; j < population[i].GetDimensions(); ++j) {
                file << population[i].GetDimensionValue(j);
                if (j < population[i].GetDimensions() - 1) {
                    file << " ";
                }
            }
            file << ";" << evaluation_values[i] << "\n";
        }
        file.close();
    } else {
        std::cerr << "Unable to open file";
    }
}

void test_evaluation_functions() {
    //inicjalizacja początkowej populacji
    std::vector<Float_representation> population_rosenbrock(10000, Float_representation(2));
    std::vector<Float_representation> population_salomon(10000, Float_representation(2));
    std::vector<Float_representation> population_whitney(10000, Float_representation(2));
    initializePopulation(population_rosenbrock, -30, 30);
    initializePopulation(population_salomon, -100, 100);
    initializePopulation(population_whitney, -10.24, 10.24);
    std::vector<double> evaluation_values_rosebrock;
    std::vector<double> evaluation_values_salomon;
    std::vector<double> evaluation_values_whitney;

    for (int i = 0; i < 10000; ++i) {
        evaluation_values_rosebrock.push_back(evaluate(population_rosenbrock[i],1));
        evaluation_values_salomon.push_back(evaluate(population_salomon[i],2));
        evaluation_values_whitney.push_back(evaluate(population_whitney[i],3));
    }

    save_test_to_csv(population_rosenbrock, evaluation_values_rosebrock, "test_rosenbrock.csv");
    save_test_to_csv(population_salomon, evaluation_values_salomon, "test_salomon.csv");
    save_test_to_csv(population_whitney, evaluation_values_whitney, "test_whitney.csv");
}


int main() {
    //test_evaluation_functions();
    run_experiment();
    return 0;
}