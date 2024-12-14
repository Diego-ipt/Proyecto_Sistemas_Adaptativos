#include <vector>
#include <map>
#include <string>
#include <random>
#include <iostream>
#include "cruce_cplex.h"
#include "metaheuristic_functions.h"

using namespace std;
int main() {
    // Padres iniciales
    vector<string> padres = {"TGGCA", "CGTAG", "TTGGC"};

    // Dataset ejemplo
    vector<string> dataset = {
            "TGGCA", "CGTAT", "TTGGC", "TGACT", "GCTAG", "TACGT", "GATCA", "CTGAT",
            "ATGCG", "CGTGA", "TGCAT", "GACTG", "CTAGT", "GTCAG", "TAGCT", "GATCG",
            "CTGAC", "ATCGT", "GCTGA", "TACGA", "GATCT", "CTGTA", "ATGCT", "CGTAG",
            "TGCGA", "GACTA", "CTAGC", "GTCGA", "TAGCA", "GATGC", "CTGCA", "ATGTA",
            "CGTAC", "TGCAA", "GACTC", "CTAGA", "GTCAT", "TAGCG", "GATGA", "CTGGA",
            "ATGAA", "CGTCA", "TGCTA", "GACTG", "CTAGT", "GTCAG", "TAGCT", "GATCG",
        };

    // Umbral
    int threshold = 3;

    try {
        // Resolver con CPLEX
        string hijo = crossover_using_cplex(padres, dataset, threshold);
        // Imprimir la calidad de los padres
        for (const auto& padre : padres) {
            int calidad_padre = calidad_solucion(dataset, threshold, padre);
            cout << "Calidad del padre " << padre << ": " << calidad_padre << endl;
        }

        // Imprimir la calidad del hijo
        int calidad_hijo = calidad_solucion(dataset, threshold, hijo);
        cout << "Calidad del hijo " << hijo << ": " << calidad_hijo << endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}