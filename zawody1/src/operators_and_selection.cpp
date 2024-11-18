#include "../include/operators_and_selection.h"

#include <vector>
#include <random>


std::mt19937 rng(std::random_device{}());
std::vector<Float_representation> crossover(const Float_representation& p1, const Float_representation& p2, double r_cross=0.5) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    // std::normal_distribution<> normal_dis(0.0, 1.0);
    std::uniform_int_distribution<> dis_int(1, p1.GetPoint().size() - 2);

    Float_representation c1 = p1;
    Float_representation c2 = p2;

    if (dis(rng) < r_cross) {
        int pt = dis_int(rng);

        for (int i = pt; i < p1.GetPoint().size(); ++i) {
            c1.SetPoint(i,p2.GetPoint()[i]);
            c2.SetPoint(i, p1.GetPoint()[i]);
        }
    }

    return {c1, c2};
}