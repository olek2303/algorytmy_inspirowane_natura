
#include "../include/eval_experiments.h"
#include <iostream>
#include <vector>
#include "evolutionary_algorithm.h"
#include <fstream>

int function_call_count = 0;

int n_of_experimetns = 100;
int n_of_dim_1 = 5;
int n_of_dim_2 = 15;
int n_of_dim_3 = 30;

void save_to_txt(const std::vector<std::vector<double>>& data, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for(int i = 0; i < data[0].size(); i++) {
            for(int j = 0; j < data.size(); j++) {
                file << data[j][i] << " ";
            }
            file << "\n";
        }
        file.close();
    } else {
        std::cerr << "Unable to open file";
    }
}


void run_rosenbrock() {
    std::vector<std::vector<double>> rosenbrock_eval_5_dim_all;
    std::vector<std::vector<double>> rosenbrock_eval_15_dim_all;
    std::vector<std::vector<double>> rosenbrock_eval_30_dim_all;

    for(int i =0; i < n_of_experimetns; i++) {
        std::vector<double> rosenbrock_eval_5_dim = evolutionary_algorithm(0.5, 1, -30, 30, 20, n_of_dim_1,5);
        std::cout << "R5 eksperyment nr: "<< i << " Liczba rekordów : " << rosenbrock_eval_5_dim.size() << "Best fitness: "
                  << rosenbrock_eval_5_dim.back() << std::endl;
        rosenbrock_eval_5_dim_all.push_back(rosenbrock_eval_5_dim);
        function_call_count = 0;

        std::vector<double> rosenbrock_eval_15_dim = evolutionary_algorithm(0.5, 1, -30, 30, 50, n_of_dim_2, 25);
        std::cout << "R15 eksperyment nr: "<< i << " Liczba rekordów: " << rosenbrock_eval_15_dim.size() << "Best fitness: "
                  << rosenbrock_eval_15_dim.back() << std::endl;
        rosenbrock_eval_15_dim_all.push_back(rosenbrock_eval_15_dim);
        function_call_count = 0;

        std::vector<double> rosenbrock_eval_30_dim = evolutionary_algorithm(0.5, 1, -30, 30, 150, n_of_dim_3, 85);
         std::cout << "R30 eksperyment nr: "<< i <<" Liczba rekordów: " << rosenbrock_eval_30_dim.size() << "Best fitness: "
                  << rosenbrock_eval_30_dim.back() << std::endl;
        rosenbrock_eval_30_dim_all.push_back(rosenbrock_eval_30_dim);
        function_call_count = 0;
    }
    save_to_txt(rosenbrock_eval_5_dim_all, "rosenbrock_5_dim.txt");
    save_to_txt(rosenbrock_eval_15_dim_all, "rosenbrock_15_dim.txt");
    save_to_txt(rosenbrock_eval_30_dim_all, "rosenbrock_30_dim.txt");
}

void run_salomon() {
    std::vector<std::vector<double>> salomon_eval_5_dim_all;
    std::vector<std::vector<double>> salomon_eval_15_dim_all;
    std::vector<std::vector<double>> salomon_eval_30_dim_all;

    for(int i =0; i < n_of_experimetns; i++) {
        std::vector<double> salomon_eval_5_dim = evolutionary_algorithm(0.5, 2, -100, 100, 10, n_of_dim_1,3);
        std::cout << "S5 eksperyment nr: "<< i << " Liczba rekordów: " << salomon_eval_5_dim.size() << "Best fitness: "
                  << salomon_eval_5_dim.back() << std::endl;
        salomon_eval_5_dim_all.push_back(salomon_eval_5_dim);
        function_call_count = 0;

        std::vector<double> salomon_eval_15_dim = evolutionary_algorithm(0.5, 2, -100, 100, 150, n_of_dim_2,70);
        std::cout << "S15 eksperyment nr: "<< i << " Liczba rekordów: " << salomon_eval_15_dim.size() << "Best fitness: "
                  << salomon_eval_15_dim.back() << std::endl;
        salomon_eval_15_dim_all.push_back(salomon_eval_15_dim);
        function_call_count = 0;

        std::vector<double> salomon_eval_30_dim = evolutionary_algorithm(0.5, 2, -100, 100, 300, n_of_dim_3 ,160);
       std::cout << "S30 eksperyment nr: "<< i << " Liczba rekordów: " << salomon_eval_30_dim.size() << "Best fitness: "
                  << salomon_eval_30_dim.back() << std::endl;
        salomon_eval_30_dim_all.push_back(salomon_eval_30_dim);
        function_call_count = 0;
    }
    save_to_txt(salomon_eval_5_dim_all, "salomon_eval_5_dim.txt");
    save_to_txt(salomon_eval_15_dim_all, "salomon_eval_15_dim.txt");
    save_to_txt(salomon_eval_30_dim_all, "salomon_eval_30_dim.txt");
}

void run_whitney() {
    std::vector<std::vector<double>> whitney_eval_5_dim_all;
    std::vector<std::vector<double>> whitney_eval_15_dim_all;
    std::vector<std::vector<double>> whitney_eval_30_dim_all;

    for(int i =0; i < n_of_experimetns; i++) {
        std::vector<double> whitney_eval_5_dim =evolutionary_algorithm(0.5, 3, -10.24, 10.24, 10, n_of_dim_1, 3);
        std::cout << "W5 eksperyment nr: "<< i << " Liczba rekordów: " << whitney_eval_5_dim.size() << "Best fitness: "
                  << whitney_eval_5_dim.back() << std::endl;
        whitney_eval_5_dim_all.push_back(whitney_eval_5_dim);
        function_call_count = 0;

        std::vector<double> whitney_eval_15_dim = evolutionary_algorithm(0.5, 3, -10.24, 10.24, 500, n_of_dim_2, 70);
        std::cout << "W15 eksperyment nr: "<< i << " Liczba rekordów: " << whitney_eval_15_dim.size() << "Best fitness: "
                  << whitney_eval_15_dim.back() << std::endl;
        whitney_eval_15_dim_all.push_back(whitney_eval_15_dim);
        function_call_count = 0;

        std::vector<double> whitney_eval_30_dim = evolutionary_algorithm(0.5, 3, -10.24, 10.24, 500, n_of_dim_3, 70);
        std::cout << "W30 eksperyment nr: "<< i << " Liczba rekordów: " << whitney_eval_30_dim.size() << "Best fitness: "
                  << whitney_eval_30_dim.back() << std::endl;
        whitney_eval_30_dim_all.push_back(whitney_eval_30_dim);
        function_call_count = 0;
    }
    save_to_txt(whitney_eval_5_dim_all, "whitney_eval_5_dim.txt");
    save_to_txt(whitney_eval_15_dim_all, "whitney_eval_15_dim.txt");
    save_to_txt(whitney_eval_30_dim_all, "whitney_eval_30_dim.txt");
}