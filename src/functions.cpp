#include "../include/functions.h"
#include <random>
#include <vector>
#include <iostream>
#include <fstream>

//functions for general use, like saving to csv, generating random numbers etc.


double random_uniform()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
};

// Przeciążenie operatora << dla std::vector
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i != vec.size() - 1) {
            os << ";";
        }
    }
    return os;
};

void save_series_to_csv(const std::vector<std::vector<double>>& data, const std::string& filename) {
    std::cout << "Saving to file: " << filename << std::endl;
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    for (const auto& series : data) {
        file << series << "\n";
    }

    file.close();
    std::cout << "File saved: " << filename << std::endl;
};

void save_execution_times_to_csv(const std::vector<long>& execution_times, const std::string& filename) {
    std::cout << "Saving execution times to file: " << filename << std::endl;
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    for (const auto& time : execution_times) {
        file << time << "\n";
    }
    file.close();
    std::cout << "File saved: " << filename << std::endl;
}

void save_best_values_to_csv(const std::vector<double>& best_values, const std::string& filename) {
    std::cout << "Saving best values to file: " << filename << std::endl;
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    for (const auto& value : best_values) {
        file << value << "\n";
    }
    file.close();
    std::cout << "File saved: " << filename << std::endl;
}
