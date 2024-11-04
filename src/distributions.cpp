#include "../include/distributions.h"
#include <random>

std::vector<double> generate_normal_distribution(double mean, double std_dev, int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(mean, std_dev);
    std::vector<double> numbers(size);
    for (int i = 0; i < size; ++i) {
        numbers[i] = d(gen);
    }
    return numbers;
}

std::vector<double> generate_uniform_distribution(double low, double high, int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> d(low, high);
    std::vector<double> numbers(size);
    for (int i = 0; i < size; ++i) {
        numbers[i] = d(gen);
    }
    return numbers;
}

std::vector<int> generate_uniform_int_distribution(double low, double high, int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> d(low, high);
    std::vector<int> numbers(size);
    for (int i = 0; i < size; ++i) {
        numbers[i] = d(gen);
    }
    return numbers;
}

