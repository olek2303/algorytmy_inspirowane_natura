#include "../include/distributions.h"
#include "../include/circle.h"
#include "../include/local_search.h"
#include "../include/simulated_annealing.h"
#include "../include/functions.h"
#include "../include/evolutionary_algorithm.h"
#include <iostream>
#include <fstream>
#include "evolutionary_test_int.h"


void exercise_1() {
    // rozkład równomierny
    std::vector<double> uniform_dist_numbers = generate_uniform_distribution(-100, 100, 1000);
    std::ofstream uniform_file("uniform_dist_numbers.txt");
    for (double num : uniform_dist_numbers) {
        uniform_file << num << "\n";
    }
    uniform_file.close();

    // rozkład normalny
    std::vector<double> normal_dist_numbers = generate_normal_distribution(0, 50, 1000);
    std::ofstream normal_file("normal_dist_numbers.txt");
    for (double num : normal_dist_numbers) {
        normal_file << num << "\n";
    }
    normal_file.close();

    // koło
    show_circle_shots(10000, 3, 4);
}

void exercise_local_search() {
    auto n1_data = exercise2(N_1);
    std::cout << "N1 done\n";
    auto n2_data = exercise2(N_2);
    std::cout << "N2 done\n";
    auto n3_data = exercise2(N_3);
    std::cout << "N3 done\n";

    save_series_to_csv(n1_data, "n1_data.csv");
    save_series_to_csv(n2_data, "n2_data.csv");
    save_series_to_csv(n3_data, "n3_data.csv");

}

void exercise_simulated_annealing() {
    auto binary_fun_1 = exercise3(1, "binary");
    auto binary_fun_2 = exercise3(2, "binary");
    auto double_fun_1 = exercise3(1, "double");
    auto double_fun_2 = exercise3(2, "double");

    save_series_to_csv(binary_fun_1, "binary_fun_1.csv");
    save_series_to_csv(binary_fun_2, "binary_fun_2.csv");
    save_series_to_csv(double_fun_1, "double_fun_1.csv");
    save_series_to_csv(double_fun_2, "double_fun_2.csv");
}

void exercise_evolutionary_algorithm() {
    auto binary_fun_1 = exercise4(1, "binary");
    auto binary_fun_2 = exercise4(2, "binary");
    //auto double_fun_1 = exercise4(1, "double");
    //auto double_fun_2 = exercise4(2, "double");

    save_series_to_csv(binary_fun_1, "binary_fun_1.csv");
    save_series_to_csv(binary_fun_2, "binary_fun_2.csv");
    //save_series_to_csv(double_fun_1, "double_fun_1.csv");
    //save_series_to_csv(double_fun_2, "double_fun_2.csv");
}

int main() {
    //exercise_1();

   // exercise_local_search();

   //exercise_simulated_annealing();
    exercise_evolutionary_algorithm();
    // runExperiments();
    return 0;
}