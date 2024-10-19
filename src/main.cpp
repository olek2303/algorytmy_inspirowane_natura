#include "../include/distributions.h"
#include "../include/circle.h"
#include "../include/local_search.h"
#include <chrono>
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
    print_current_working_directory();
    auto n1_data = excercise2(N_1);
    std::cout << "N1 done\n";
    auto n2_data = excercise2(N_2);
    std::cout << "N2 done\n";
    auto n3_data = excercise2(N_3);
    std::cout << "N3 done\n";

    save_to_txt(n1_data, "n1_data.txt");
    save_to_txt(n2_data, "n2_data.txt");
    save_to_txt(n3_data, "n3_data.txt");

    auto n1_avg = average_evaluation_series(n1_data);
    auto n2_avg = average_evaluation_series(n2_data);
    auto n3_avg = average_evaluation_series(n3_data);

    save_avg_to_txt(n1_avg, "n1_avg.txt");
    save_avg_to_txt(n2_avg, "n2_avg.txt");
    save_avg_to_txt(n3_avg, "n3_avg.txt");
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
//    exercise_1();

    exercise_local_search();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "EXECUTION: --- " << execution_time << " milliseconds ---\n";
    return 0;
}