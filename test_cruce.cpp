#include <vector>
#include <map>
#include <string>
#include <random>
#include <iostream>
#include "cruce_cplex.h"
#include "metaheuristic_functions.h"

using namespace std;
int main() {
    std::vector<std::string> parents = {"ACGT", "TGCA"};
    std::vector<std::string> dataset = {"AAAA", "CCCC", "GGGG", "TTTT"};
    int threshold = 3;



    std::vector<std::string> offspring = crossover_using_cplex(parents, threshold, dataset);

    vector<double> parent_quality;
    for (size_t i = 0; i < parents.size(); ++i) {
        vector<double> parent_quality;
        parent_quality[i] = calidad_solucion(dataset, threshold, parents[i]);
    }
    vector<double> offspring_quality;
    for (size_t i = 0; i < offspring.size(); ++i) {
        vector<double> offspring_quality;
        offspring_quality[i] = calidad_solucion(dataset, threshold, offspring[i]);
    }

    bool offspring_better = *max_element(offspring_quality.begin(), offspring_quality.end()) > *max_element(parent_quality.begin(), parent_quality.end());
    cout << (offspring_better ? "True" : "False") << " si los hijos tienen mayor calidad que el mejor de los padres" << endl;

    return 0;
}
