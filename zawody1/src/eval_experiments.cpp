//
// Created by admin on 20.11.2024.
//

#include "../include/eval_experiments.h"
#include <iostream>
#include <vector>
#include "evolutionary_algorithm.h"

int function_call_count = 0;

int n_of_experimetns = 1;
int n_of_dim_1 = 5;
int n_of_dim_2 = 10;
int n_of_dim_3 = 15;

void run_rosenbrock() {
    std::vector<std::vector<double>> rosenbrock_eval_5_dim_all;
    std::vector<std::vector<double>> rosenbrock_eval_10_dim_all;
    std::vector<std::vector<double>> rosenbrock_eval_15_dim_all;

    for(int i =0; i < n_of_experimetns; i++) {
        std::vector<double> rosenbrock_eval_5_dim = evolutionary_algorithm(0.5, 1, -30, 30, 10, n_of_dim_1);
        std::cout << "Liczba rekordów: " << rosenbrock_eval_5_dim.size() << "Best fitness: "
                  << rosenbrock_eval_5_dim.back() << std::endl;
        rosenbrock_eval_5_dim_all.push_back(rosenbrock_eval_5_dim);
        function_call_count = 0;

        std::vector<double> rosenbrock_eval_10_dim = evolutionary_algorithm(0.5, 1, -30, 30, 10, n_of_dim_1);
        std::cout << "Liczba rekordów: " << rosenbrock_eval_10_dim.size() << "Best fitness: "
                  << rosenbrock_eval_10_dim.back() << std::endl;
        rosenbrock_eval_10_dim_all.push_back(rosenbrock_eval_10_dim);
        function_call_count = 0;

        std::vector<double> rosenbrock_eval_15_dim = evolutionary_algorithm(0.5, 1, -30, 30, 10, n_of_dim_1);
        std::cout << "Liczba rekordów: " << rosenbrock_eval_15_dim.size() << "Best fitness: "
                  << rosenbrock_eval_15_dim.back() << std::endl;
        rosenbrock_eval_15_dim_all.push_back(rosenbrock_eval_15_dim);
        function_call_count = 0;
    }
}

void run_salomon() {
    std::vector<std::vector<double>> salomon_eval_5_dim_all;
    std::vector<std::vector<double>> salomon_eval_10_dim_all;
    std::vector<std::vector<double>> salomon_eval_15_dim_all;

    for(int i =0; i < n_of_experimetns; i++) {
        std::vector<double> salomon_eval_5_dim = evolutionary_algorithm(0.5, 2, -100, 100, 10, n_of_dim_1);
        std::cout << "Liczba rekordów: " << salomon_eval_5_dim.size() << "Best fitness: "
                  << salomon_eval_5_dim.back() << std::endl;
        salomon_eval_5_dim_all.push_back(salomon_eval_5_dim);
        function_call_count = 0;

        std::vector<double> salomon_eval_10_dim = evolutionary_algorithm(0.5, 2, -100, 100, 10, n_of_dim_1);
        std::cout << "Liczba rekordów: " << salomon_eval_10_dim.size() << "Best fitness: "
                  << salomon_eval_10_dim.back() << std::endl;
        salomon_eval_5_dim_all.push_back(salomon_eval_10_dim);
        function_call_count = 0;

        std::vector<double> salomon_eval_15_dim = evolutionary_algorithm(0.5, 2, -100, 100, 10, n_of_dim_1);
        std::cout << "Liczba rekordów: " << salomon_eval_15_dim.size() << "Best fitness: "
                  << salomon_eval_15_dim.back() << std::endl;
        salomon_eval_5_dim_all.push_back(salomon_eval_15_dim);
        function_call_count = 0;
    }
}

void run_whitney() {
    std::vector<std::vector<double>> whitney_eval_5_dim_all;
    std::vector<std::vector<double>> whitney_eval_10_dim_all;
    std::vector<std::vector<double>> whitney_eval_15_dim_all;

    for(int i =0; i < n_of_experimetns; i++) {
        std::vector<double> whitney_eval_5_dim =evolutionary_algorithm(0.5, 3, -10.24, 10.24, 10, n_of_dim_1);
        std::cout << "Liczba rekordów: " << whitney_eval_5_dim.size() << "Best fitness: "
                  << whitney_eval_5_dim.back() << std::endl;
        whitney_eval_5_dim_all.push_back(whitney_eval_5_dim);
        function_call_count = 0;

        std::vector<double> whitney_eval_10_dim = evolutionary_algorithm(0.5, 3, -10.24, 10.24, 10, n_of_dim_2);
        std::cout << "Liczba rekordów: " << whitney_eval_10_dim.size() << "Best fitness: "
                  << whitney_eval_10_dim.back() << std::endl;
        whitney_eval_10_dim_all.push_back(whitney_eval_10_dim);
        function_call_count = 0;

        std::vector<double> whitney_eval_15_dim = evolutionary_algorithm(0.5, 3, -10.24, 10.24, 10, n_of_dim_3);
        std::cout << "Liczba rekordów: " << whitney_eval_15_dim.size() << "Best fitness: "
                  << whitney_eval_15_dim.back() << std::endl;
        whitney_eval_15_dim_all.push_back(whitney_eval_15_dim);
        function_call_count = 0;
    }
}