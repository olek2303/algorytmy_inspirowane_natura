#include "../include/functions.h"
#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>

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

// Funkcja zamienia przecinki na kropki w danym ciągu znaków
std::string replaceCommaWithDot(const std::string &line) {
    std::string result = line;
    std::replace(result.begin(), result.end(), ',', '.');
    return result;
}

// Funkcja wczytuje dane z pliku CSV i zwraca jako macierz liczb (wektor wektorów)
std::vector<std::vector<double>> readCSV(const std::string &filename) {
    std::ifstream file(filename);
    std::vector<std::vector<double>> data;
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("Nie można otworzyć pliku!");
    }

    while (std::getline(file, line)) {
        line = replaceCommaWithDot(line);
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> row;

        while (std::getline(ss, cell, ';')) {
            if (!cell.empty()) {
                row.push_back(std::stod(cell));
            }
        }
        data.push_back(row);
    }

    file.close();
    return data;
}

// Funkcja zapisuje dane do pliku CSV
void writeCSV(const std::string &filename, const std::vector<std::vector<double>> &data) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Nie można otworzyć pliku do zapisu!");
    }

    for (const auto &row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << std::fixed << std::setprecision(6) << row[i];
            if (i != row.size() - 1) {
                file << " ";
            }
        }
        file << "\n";
    }

    file.close();
}

// Funkcja transponuje macierz
std::vector<std::vector<double>> transpose(const std::vector<std::vector<double>> &data) {
    size_t rows = data.size();
    size_t cols = data[0].size();
    std::vector<std::vector<double>> transposed(cols, std::vector<double>(rows));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            transposed[j][i] = data[i][j];
        }
    }

    return transposed;
}

void save_transposed_values(const std::string& inputFile, const std::string& outputFile) {
    try {
        // Wczytaj dane z pliku CSV
        auto data = readCSV(inputFile);

        // Transponuj dane
        auto transposedData = transpose(data);

        // Zapisz dane do pliku CSV
        writeCSV(outputFile, transposedData);

        std::cout << "Dane zostały pomyślnie przetworzone i zapisane do " << outputFile << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Wystąpił błąd: " << e.what() << std::endl;
    }
}