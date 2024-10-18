#include "../include/distributions.h"
#include "../include/circle.h"
#include <chrono>
#include <iostream>
#include <fstream>

void exercise_1() {
    // rozkład równomierny
    std::vector<double> uniform_dist_numbers = generate_uniform_distribution(-100, 100, 1000);
    std::ofstream uniform_file("lab1_files/uniform_dist_numbers.txt");
    for (double num : uniform_dist_numbers) {
        uniform_file << num << "\n";
    }
    uniform_file.close();

    // rozkład normalny
    std::vector<double> normal_dist_numbers = generate_normal_distribution(0, 50, 1000);
    std::ofstream normal_file("lab1_files/normal_dist_numbers.txt");
    for (double num : normal_dist_numbers) {
        normal_file << num << "\n";
    }
    normal_file.close();

    // koło
    show_circle_shots(10000, 3, 4);
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    exercise_1();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "EXECUTION: --- " << execution_time << " milliseconds ---\n";
    return 0;
}