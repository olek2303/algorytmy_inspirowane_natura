
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
#endif //FUNCTIONS_H
