#include "../include/distributions.h"
#include "../include/circle.h"
#include "../include/local_search.h"
#include "../include/simulated_annealing.h"
#include <iostream>
#include <fstream>


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
    auto n1_data = excercise2(N_1);
    std::cout << "N1 done\n";
    auto n2_data = excercise2(N_2);
    std::cout << "N2 done\n";
    auto n3_data = excercise2(N_3);
    std::cout << "N3 done\n";

    save_to_csv(n1_data, "n1_data.csv");
    save_to_csv(n2_data, "n2_data.csv");
    save_to_csv(n3_data, "n3_data.csv");

}

void exercise_simulated_annealing() {
    auto binary_fun_1 = excercise3(1, "double");

    for (const auto& series : binary_fun_1) {
        for (size_t i = 0; i < series.size(); ++i) {
            std::cout << series[i];
            if (i < series.size() - 1) {
                std::cout << ";";
            }
        }
        std::cout << "\n";
    }
}

int main() {
//    exercise_1();

    //exercise_local_search();
    exercise_simulated_annealing();
    //Sprawdziłem działanie dla reprezentacji binarnej dla funkcji oceny 1. Sprawdziłem wyłącznie na małej liczbie eksperymentów.
    //TODO finish implement of double version of evaluation fun 1, implement evaluation fun 2 for both representations, save results to csv, make excel file.
    return 0;
}