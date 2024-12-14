#ifndef MERGE_MODEL_H
#define MERGE_MODEL_H

#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include "merge_models_aux.h"

using namespace std;

// Estructura para representar un individuo en la población
struct Individual {
    string solution;
    double fitness;
};


// Función para generar la población inicial
vector<Individual> init_population(int population_size, int size_individual, double threshold, const vector<string>& dataset, unordered_map<int, string> index_to_substring) {
    vector<Individual> population;
    for (int i = 0; i < population_size; ++i) {
        string solution = generateNeighborSolutionRandom(size_individual, index_to_substring);
        population.push_back({solution, calidad_solucion(dataset, threshold, solution)});
    }
    return population;
}
// Función para realizar el cruce usando crossover_using_cplex
Individual crossover(const vector<string>& parents, int threshold, const vector<string>& dataset) {
    string child_solution = crossover_using_cplex(parents, threshold, dataset);
    double child_fitness = calidad_solucion(dataset, threshold, child_solution);
    cout << "calidad offspring: " << child_fitness << endl;
    return {child_solution, child_fitness};
}

// Función para realizar la mutación usando cooling_system_plus
Individual mutate(const Individual& individual, const vector<string>& dataset, int threshold
, int max_error, double temperature_pert, double temperature_leap, 
double cooling_rate, double heat_rate,int iteraciones_max, 
unordered_map<string, int> substring_to_index,unordered_map<int, string> index_to_substring) {
    auto result = cooling_system_plus(dataset, individual.solution, individual.fitness, threshold, 
    max_error, temperature_pert, temperature_leap, 
    cooling_rate, heat_rate, iteraciones_max, substring_to_index, index_to_substring);
    return {result.first, result.second};
}

void geneticAlgorithm_merge(int population_size, double pm, int elite_count, 
const vector<string>& dataset, double threshold, 
int max_time_genetic, int max_error, double temperature_pert, 
double temperature_leap, double cooling_rate, 
double heat_rate, bool tuningMode, int iteraciones_max, int N_parents) {


    //dataset
    int dataset_size = dataset.size(); 
    int long_cadenas = dataset[0].size();
   
    // Crear los mapas para las subcadenas
    unordered_map<string, int> substring_to_index;
    unordered_map<int, string> index_to_substring;
    generateSubstrings(substring_to_index, index_to_substring);

    // Imprimir el tamaño del dataset
   cout << "dataset size: " << dataset_size << ", " << long_cadenas << endl;

    // Inicializar la población
    vector<Individual> population = init_population(population_size, long_cadenas, threshold, dataset, index_to_substring);

    auto start_time = chrono::steady_clock::now();
    int generations = 0;

    while (chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start_time).count() < max_time_genetic) {
        vector<Individual> new_population;
        vector<Individual> population_copy = population;
        
        // Selección y cruce
        for (int i = 0; i < population_size / N_parents; ++i) {
            vector<string> parents;
            // Seleccionar N_parents padres aleatorios
            for (int j = 0; j < N_parents; ++j) {
                int parent_index = rand() % population.size();
                parents.push_back(population[parent_index].solution);
                population_copy.erase(population_copy.begin() + parent_index);
                cout << "calidad padre " << j << ": " << calidad_solucion(dataset, threshold, population[parent_index].solution) << endl;
            }

            // Crear un hijo a partir de N_parents
            Individual child = crossover(parents, threshold, dataset);
            new_population.push_back(child);
        }


        // Ordenar la población por fitness
        sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
            return a.fitness > b.fitness;
        });

        // Imprimir el mejor individuo de la generación actual
        auto current_time = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start_time).count();
        cout << "Generation " << generations << ": Best fitness = " << population[0].fitness << " in time: " << current_time << " seconds" << endl;


        // Agregar los N individuos con mejor calidad a la nueva población
        for (int i = 0; i < elite_count; ++i) {
            new_population.push_back(population[i]);
        }

        // Completa la población con soluciones aleatorias
        while (new_population.size() < population_size) {
            string solution = generateNeighborSolutionRandom(dataset[0].size(), index_to_substring);
            double fitness = calidad_solucion(dataset, threshold, solution);
            new_population.push_back({solution, fitness});
        }

        // Mutación
        for (Individual& individual : new_population) {
            if(rand() % 100 < pm * 100){
                auto result = cooling_system_plus(dataset, individual.solution, individual.fitness, threshold, 
                max_error, temperature_pert, temperature_leap, 
                cooling_rate, heat_rate, iteraciones_max, substring_to_index, index_to_substring);
                individual.solution = result.first;
                individual.fitness = result.second;
            }
        }


        // Reemplazo de la población
        population = new_population;

        generations++;
    }

    // Imprimir la mejor solución encontrada
    cout << "Best solution: " << population[0].solution << " with fitness = " << population[0].fitness << endl;
}
#endif
