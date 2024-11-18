#include "../include/ecdf_summary.h"

/* ==========================
   (c) WMP.SNŚ UKSW, 2024
========================== */
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <string>


// Funkcja oblicza i zapisuje do kontenera wartości progów budżetu wywołań funkcji F
void progiF(std::vector<double>& vec, double n) {
    double w = 0;
    int counter = 1;
    for (double &p : vec) {
        p = n * pow(10, w);
        if (fabs(floor(p) - n * pow(10, w)) < 0.0000001)
            p = floor(p); // trick numeryczny*
        w += 0.1;
        std::cout << counter++ << "\t" << p << std::endl;
    }
}
// Funkcja oblicza i zapisuje do kontenera wartości progów jakości rozwiązania
void progiJ(std::vector<double>& vec) {
    double w = -8;
    for (double &p : vec) {
        p = pow(10, w);
        if (fabs(floor(p) - pow(10, w)) < 0.000000001)
            p = floor(p); // trick numeryczny*
        w += 0.2;
    }
}
// *trick numeryczny w funkcjach 'progiF' i 'progiF' chroni przed przypadkami,
// gdy liczba całkowita ma na dostatecznie odległej pozycji po przecinku niezerową
// wartość, np. 10.000000000001

// Fukcja zlicza przekroczone progi jakości rozwiązania dla zadanej wartości 'v'
int zliczPrzekroczoneProgiJ(double v, std::vector<double>& vec) {
    int counter = 0;
    // progi są sprawdzane od ostatniego zapisanego w kontenerze, do pierwszego
    for (auto i = vec.rbegin(); i != vec.rend(); ++i)
        if (v < *i) counter++;
    return counter;
}

// Obliczanie wartości do wykresu ECDF na podstawie danych surowych
// zawartych w przykładowym pliku "data.txt"
void count_ECDF(const std::string& inputFile, const std::string& outputFile) {
    double n = 10; // liczba wymiarów przestrzeni poszukiwań
    double MaxNFFC = 10000; // maksymalna liczba wywołań funkcji F
    std::vector<double> pF(41, 0); // progi budżetu wywołań funkcji F
    std::vector<double> pJ(51, 0); // progi jakości rozwiązania
    int counter = 1; // licznik odczytanych wartości z pliku
    int pFcounter = 0; // licznik sprawdzanych progów budżetu wywołań funkcji F
    double val = 0;
    try {
        std::ifstream fdata(inputFile); // plik z zapisanymi wartościami funkcji F najlepszych rozwiązań
        std::ofstream fecdf(outputFile);

        progiF(pF, n); // obliczenie i zapisanie wartości progów pF
        progiJ(pJ); // obliczenie i zapisanie wartości progów pJ
        std::string line;

        // generujemy nagłówki dla kolumn, których jest
        // tyle, ile progów jakości, czyli 51
        for (auto i = pJ.rbegin(); i != pJ.rend(); ++i)
            fecdf << *i << " ";
        fecdf << std::endl;

        // generujemy zawartość dla kolumn
        while (getline(fdata, line)) {
            // odczytana linia zawiera 100 liczb - wartoście aktualnego najlepszego znalezionego
            // rozwiązania w każdym ze 100 eksperymentów w danym wywołaniu funkcji F
            if (counter * n >= pF[pFcounter]) {
                // obliczamy ile progów jakości przekroczyła każda z wartości w odczytanej linii
                std::istringstream iss(line);
                std::vector<double> Fvec(100, 0);
                for (double& liczbaProgow : Fvec) {
                    iss >> val;
                    liczbaProgow = zliczPrzekroczoneProgiJ(val, pJ);
                }
                // obliczamy, w ilu eksperymentach udało się przekroczyć każdy z progów
                // jakości w danym momencie pracy, tj. dla danego progu wywołań F
                for (int i = 0; i < pJ.size(); i++) {
                    int ekspNum = 0;
                    for (double liczbaProgow : Fvec)
                        if (liczbaProgow > i) ekspNum++;
                    fecdf << ekspNum << " ";
                }
                fecdf << std::endl;
                std::cout << pFcounter++ << std::endl;
            }
            counter++;
        }

    } catch (const std::exception &e) {
        std::cerr << "Wystąpił błąd: " << e.what() << std::endl;
    }

}