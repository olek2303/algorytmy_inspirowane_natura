
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <random>
#include <vector>

//functions for general use, like saving to csv, generating random numbers etc.

double random_uniform();

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec);

void save_series_to_csv(const std::vector<std::vector<double>>& data, const std::string& filename);
void save_execution_times_to_csv(const std::vector<long>& execution_times, const std::string& filename);
void save_best_values_to_csv(const std::vector<double>& best_values, const std::string& filename);
std::string replaceCommaWithDot(const std::string &line);
std::vector<std::vector<double>> readCSV(const std::string &filename);
void writeCSV(const std::string &filename, const std::vector<std::vector<double>> &data);
std::vector<std::vector<double>> transpose(const std::vector<std::vector<double>> &data);
void save_transposed_values(const std::string& inputFile, const std::string& outputFile);
#endif //FUNCTIONS_H
