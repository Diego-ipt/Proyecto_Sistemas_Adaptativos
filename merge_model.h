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

// Función para generar la población inicial usando DecoderATCG
vector<Individual> initializePopulation(int population_size, int seleccion, const vector<string>& dataset, double threshold) {
    DecoderATCG decoder(seleccion);
    vector<Individual> population;

    for (int i = 0; i < population_size; ++i) {
        vector<double> chromosome(dataset[0].size(), 0.0);
        for (double& gene : chromosome) {
            gene = static_cast<double>(rand()) / RAND_MAX;
        }
        string solution = decoder.traduccion(chromosome);
        double fitness = calidad_solucion(dataset, threshold, solution);
        population.push_back({solution, fitness});
    }

    // Ordenar la población por fitness
    sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
        return a.fitness > b.fitness;
    });

    // Seleccionar los X mejores individuos
    population.resize(population_size);

    return population;
}

// Función para realizar el cruce usando crossover_using_cplex
Individual crossover(const Individual& parent1, const Individual& parent2, int threshold, const vector<string>& dataset) {
    string child_solution = crossover_using_cplex(parent1.solution, parent2.solution, threshold, dataset);
    double child_fitness = calidad_solucion(dataset, threshold, child_solution);
    return {child_solution, child_fitness};
}

// Función para realizar la mutación usando cooling_system_plus
Individual mutate(const Individual& individual, const vector<string>& dataset, int threshold, int max_error, double temperature_pert, double temperature_leap, double cooling_rate, double heat_rate, bool tuningMode, int max_time_seconds, int iteraciones_max) {
    auto result = cooling_system_plus(dataset, threshold, max_error, temperature_pert, temperature_leap, cooling_rate, heat_rate, tuningMode, max_time_seconds, iteraciones_max);
    return {result.first, result.second};
}

// Función principal del algoritmo genético
void geneticAlgorithm_merge(int population_size, int random_population_size, const vector<string>& dataset, double threshold, int max_time_genetic, int max_error, double temperature_pert, double temperature_leap, double cooling_rate, double heat_rate, bool tuningMode, int max_time_seconds_mutation, int iteraciones_max, int elite_count) {
    // Inicializar la población
    vector<Individual> population;
    int seleccion;
    for(int i = 1; i < 7; i++){
        seleccion = i;
        vector<Individual> new_population = initializePopulation(population_size/6, seleccion, dataset, threshold);
        population.insert(population.end(), new_population.begin(), new_population.end());
    }

    // variable para la creacion de sujetos randoms
    unordered_map<int, string> index_to_substring;
    generateSubstrings(index_to_substring);

    auto start_time = chrono::steady_clock::now();
    int generations = 0;

    while (chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start_time).count() < max_time_genetic) {
        vector<Individual> new_population;

        // Selección y cruce
        for (int i = 0; i < population_size / 2; ++i) {
            int parent1_index = rand() % population.size();
            int parent2_index = rand() % population.size();
            const Individual& parent1 = population[parent1_index];
            const Individual& parent2 = population[parent2_index];
            Individual child = crossover(parent1, parent2, threshold, dataset);
            new_population.push_back(child);
        }

        // Mutación
        for (Individual& individual : new_population) {
            individual = mutate(individual, dataset, threshold, max_error, temperature_pert, temperature_leap, cooling_rate, heat_rate, tuningMode, max_time_seconds_mutation, iteraciones_max);
        }

        // Agregar parte de la población usando generateNeighborSolutionRandom
        for (int i = 0; i < random_population_size; ++i) {
            string solution = generateNeighborSolutionRandom(dataset[0].size(), index_to_substring);
            double fitness = calidad_solucion(dataset, threshold, solution);
            new_population.push_back({solution, fitness});
        }

        // Ordenar la población por fitness
        sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
            return a.fitness > b.fitness;
        });

        // Imprimir el mejor individuo de la generación actual
        cout << "Generation " << generations << ": Best fitness = " << population[0].fitness << endl;

        // Agregar los N individuos con mejor calidad a la nueva población
        for (int i = 0; i < elite_count; ++i) {
            new_population.push_back(population[i]);
        }

        // Reemplazo de la población
        population = new_population;

        generations++;
    }

    // Imprimir la mejor solución encontrada
    cout << "Best solution: " << population[0].solution << " with fitness = " << population[0].fitness << endl;
}

#endif
