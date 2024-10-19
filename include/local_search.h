// lab2

#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include <vector>
#include <string>

// Constants
extern const int BITS_PER_DIMENSION;
extern const double MIN_VALUE;
extern const double MAX_VALUE;
extern const int MAX_ITER;
extern const int MAX_ITER_EXPERIMENT;
extern const int N_1;
extern const int N_2;
extern const int N_3;

// Function declarations
double mapping_value(int decimal, int bits_per_dimension);
int neighborhood_operator(double m, int solution, int dimensions);
double evaluation_function(int dimensions, int solution);
std::pair<int, std::vector<double>> first_improvement_local_search(int x, double m, int dimensions);
std::vector<std::vector<double>> excercise2(int dimensions);
std::vector<std::vector<double>> pad_evaluation_series(const std::vector<std::vector<double>>& all_evaluation_series);
std::vector<double> average_evaluation_series(const std::vector<std::vector<double>>& all_evaluation_series);
void save_to_txt(const std::vector<std::vector<double>>& data, const std::string& filename);
void save_avg_to_txt(const std::vector<double>& avg_data, const std::string& filename);
void print_current_working_directory();

#endif // LOCAL_SEARCH_H