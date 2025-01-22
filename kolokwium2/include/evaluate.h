//
// Created by admin on 20.01.2025.
//

#ifndef KOL2_EVALUATE_H
#define KOL2_EVALUATE_H

#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <string>

class Evaluator {
public:
    int numVariables = 0;
    std::string name;
    virtual ~Evaluator() {}
    virtual std::vector<double> evaluate(const std::vector<double>& variables) const = 0;
};

class ZDT1 : public Evaluator {
public:
    ZDT1(int numVariables, std::string name) {
        this->name = std::move(name);
        this->numVariables = numVariables;
    }

    std::vector<double> evaluate(const std::vector<double>& variables) const override {
        double f1 = variables[0];
        double g = 1.0 + 9.0 * std::accumulate(variables.begin() + 1, variables.end(), 0.0) / (variables.size() - 1);
        double h = 1.0 - std::sqrt(f1 / g);
        double f2 = g * h;
        return {f1, f2};
    }
};

class ZDT2 : public Evaluator {
public:
    ZDT2(int numVariables, std::string name) {
        this->name = std::move(name);
        this->numVariables = numVariables;
    }

    std::vector<double> evaluate(const std::vector<double>& variables) const override {
        double f1 = variables[0];
        double g = 1.0 + 9.0 * std::accumulate(variables.begin() + 1, variables.end(), 0.0) / (variables.size() - 1);
        double h = 1.0 - std::pow(f1 / g, 2);
        double f2 = g * h;
        return {f1, f2};
    }
};

class ZDT3 : public Evaluator {
public:
    ZDT3(int numVariables, std::string name) {
        this->name = std::move(name);
        this->numVariables = numVariables;
    }

    std::vector<double> evaluate(const std::vector<double>& variables) const override {
        double f1 = variables[0];
        double g = 1.0 + 9.0 * std::accumulate(variables.begin() + 1, variables.end(), 0.0) / (variables.size() - 1);
        double h = 1.0 - std::sqrt(f1 / g) - (f1 / g) * std::sin(10 * M_PI * f1);
        double f2 = g * h;
        return {f1, f2};
    }
};

class ZDT4 : public Evaluator {
public:
    ZDT4(int numVariables, std::string name) {
        this->name = std::move(name);
        this->numVariables = numVariables;
    }

    std::vector<double> evaluate(const std::vector<double>& variables) const override {
        double f1 = variables[0];
        double g = 1.0 + 10.0 * (variables.size() - 1) +
                   std::accumulate(variables.begin() + 1, variables.end(), 0.0, [](double sum, double x) {
                       return sum + (x * x - 10.0 * std::cos(4.0 * M_PI * x));
                   });
        double h = 1.0 - std::sqrt(f1 / g);
        double f2 = g * h;
        return {f1, f2};
    }
};

class ZDT6 : public Evaluator {
public:
    ZDT6(int numVariables, std::string name) {
        this->name = std::move(name);
        this->numVariables = numVariables;
    }

    std::vector<double> evaluate(const std::vector<double>& variables) const override {
        double f1 = 1.0 - std::exp(-4.0 * variables[0]) * std::pow(std::sin(6.0 * M_PI * variables[0]), 6);
        double g = 1.0 + 9.0 * std::pow(std::accumulate(variables.begin() + 1, variables.end(), 0.0) / (variables.size() - 1), 0.25);
        double h = 1.0 - std::pow(f1 / g, 2);
        double f2 = g * h;
        return {f1, f2};
    }
};

#endif //KOL2_EVALUATE_H
