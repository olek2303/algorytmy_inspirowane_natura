#include "../include/circle.h"
#include "../include/distributions.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

void save_to_file(const std::string& filename, const std::vector<std::vector<double>>& data) {
    std::ofstream file(filename);
    for (const auto& point : data) {
        file << point[0] << " " << point[1] << "\n";
    }
    file.close();
}

void show_circle_shots(int amount_of_shots, double radius, double half_side) {
    std::vector<double> x = generate_uniform_distribution(-half_side, half_side, amount_of_shots);
    std::vector<double> y = generate_uniform_distribution(-half_side, half_side, amount_of_shots);
    std::vector<std::vector<double>> inside_circle;
    std::vector<std::vector<double>> outside_circle;
    int counter = 0;

    for (int i = 0; i < amount_of_shots; ++i) {
        counter++;
        std::vector<double> shot = {x[i], y[i]};
        if (sqrt(shot[0] * shot[0] + shot[1] * shot[1]) <= radius) {
            inside_circle.push_back(shot);
        } else {
            outside_circle.push_back(shot);
        }
        if (counter == 100 || counter == 1000) {
            double res = 4 * half_side * half_side * inside_circle.size() / counter;
            std::cout << "Proporcja trafień do strzałów: " << inside_circle.size() << " : " << counter << "\n";
            std::cout << "Pole koła liczone z proporcji: " << res << "\n";
            std::cout << "Rzeczywiste pole kola: " << M_PI * radius * radius << "\n\n";
        }
    }

    std::cout << "Proporcja trafień do strzałów: " << inside_circle.size() << " : " << counter << "\n";
    std::cout << "Pole koła liczone z proporcji: " << 4 * half_side * half_side * inside_circle.size() / counter << "\n";
    std::cout << "Rzeczywiste pole kola: " << M_PI * radius * radius << "\n\n";

    save_to_file("inside_circle.txt", inside_circle);
    save_to_file("outside_circle.txt", outside_circle);
}