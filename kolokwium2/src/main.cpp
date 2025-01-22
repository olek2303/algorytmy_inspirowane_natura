#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include "../include/individual.h"
#include "../include/functions.h"
#include "../include/spea2.h"
#include "../include/globals.h"
#include "../include/evaluate.h"


int main() {
    std::random_device rd;
    std::mt19937 rng(rd());

    Evaluator* evaluateZDT1 = new ZDT1(10, "ZDT1");
    Evaluator* evaluateZDT2 = new ZDT2(10, "ZDT2");
    Evaluator* evaluateZDT3 = new ZDT3(10, "ZDT3");
    Evaluator* evaluateZDT4 = new ZDT4(10, "ZDT4");
    Evaluator* evaluateZDT6 = new ZDT6(10, "ZDT6");

    std::vector<Evaluator*> evaluators = {evaluateZDT1, evaluateZDT2, evaluateZDT3, evaluateZDT4, evaluateZDT6};

    for (int i = 10; i <= 50; i += 20) {
        for (auto& evaluator : evaluators) {
            std::cout << "Evaluating " << evaluator->name << " with " << i << " variables" << std::endl;
            evaluator->numVariables = i;
            spea2(rng, evaluator);
        }
    }

    return 0;
}
