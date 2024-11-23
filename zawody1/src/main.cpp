#include <iostream>
#include "../include/evolutionary_algorithm.h"
#include "../include/evaluation_functions.h"
#include "../include/Float_representation.h"
#include <fstream>
#include "../include/eval_experiments.h"
#include "../include/ecdf_summary.h"

void run_experiments() {
    run_rosenbrock();
    run_salomon();
    run_whitney();
}

void save_test_to_csv(const std::vector<Float_representation>& population, const std::vector<double>& evaluation_values, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "x1;x2;Evaluation\n";
        for (size_t i = 0; i < population.size(); ++i) {
            for (int j = 0; j < population[i].GetDimensions(); ++j) {
                file << population[i].GetDimensionValue(j);
                if (j < population[i].GetDimensions() - 1) {
                    file << ";";
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
   // test_evaluation_functions();
//    run_experiments();
    count_ECDF("../data/5_dim.txt", "../data/5_dim_ECDF.txt", 5);

    return 0;
}