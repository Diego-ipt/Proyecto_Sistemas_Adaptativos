#ifndef METAHEURISTIC_FUNCTIONS_H
#define METAHEURISTIC_FUNCTIONS_H

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
#include "loadinputdata.h"
#include "funciones_greedy.h"

using namespace std;

//Función para generar la solución inicial
string generateInitialSolution(const vector<string>& dataset, double threshold,unordered_map<string, int> substring_to_index, unordered_map<int, string> index_to_substring ) {
    // Asumimos que los valores de M, threshold y alpha son conocidos o se pueden calcular
    int M = dataset[4].size(); // Longitud de las cadenas
    double alpha = 1.0; // Valor de alpha
    // Generar la solución inicial usando la heurística greedy
    string solution = greedyHeuristicFFMS(dataset, M, substring_to_index, index_to_substring, threshold, alpha);
    return solution;
}


//Función para generar soluciones vecinas
vector<string> generateNeighborSolution(const string& current_solution,unordered_map<string, int> substring_to_index, unordered_map<int, string> index_to_substring, double temperature){
    vector<string> neighbor_solutions;
    vector<int> solution = compressString(current_solution, substring_to_index);
    int num_columns = solution.size();

    // Generar soluciones vecinas
    for (int j = 1; j < 8; ++j) {
        vector<int> new_solution = solution;
        for (int i = 0; i < num_columns; ++i) {
            new_solution[i] = (solution[j % num_columns] + static_cast<int>(j * 8));
            
            //perturbacion
            int random_position = (rand() % 64)*temperature;
            if (rand() % 2) {
                new_solution[i] += random_position;
            }else{
                new_solution[i] -= random_position;
                new_solution[i] = abs(new_solution[i]);
            }
            new_solution[i] %= 64;
        }
        // Convert new_solution to string before pushing
        string new_solution_str;
        for (int pos : new_solution) {
            new_solution_str += getSubstringByPosition(index_to_substring, pos);
        }
        neighbor_solutions.push_back(new_solution_str);
    }

    return neighbor_solutions;
}

bool accept_rate(double best_quality, double neighbor_quality, double temperature_porcentual, int dataset_size) {

    // Calcular la diferencia de calidad
    double delta_quality = (best_quality-neighbor_quality)/dataset_size;
    double probability = exp(delta_quality);
    probability *=temperature_porcentual;
    double aleatorio= (static_cast<double>(rand()) / RAND_MAX);
    bool accept = false;
    // Calcular la probabilidad de aceptar la nueva solución
    //cout<< "delta_quality: "<<delta_quality<<", probability: "<<probability<<", aleatorio: "<<aleatorio<<endl;

    if( delta_quality<=0){
        accept=true;
    }else if (aleatorio < probability) {
        accept= true;
    }

    // Si no se acepta la nueva solución, mantener la calidad actual
    return accept;  
}

int size_calculator(double temperature_porcentual, double solution_size){
    int size_extra = solution_size/3;
    int tercio_semi_aleatorio = temperature_porcentual*size_extra*(static_cast<double>(rand()) / RAND_MAX);
    int multiplo_tres = tercio_semi_aleatorio*3;
    if (multiplo_tres == 0){
        return 3;
    }
    return (multiplo_tres);
}

//generar neighbor random
string generateNeighborSolutionRandom(int size, unordered_map<int, string> index_to_substring){
    string neighbor_solutions;
    for (int j = 0; j < size; j+=3) {
        neighbor_solutions+=getSubstringByPosition(index_to_substring, rand() % 64);
    }
    return neighbor_solutions;
}

void cooling_system(const string& metaheuristic_name, const vector<string>& dataset, int max_time_seconds, int threshold) {
    unordered_map<string, int> substring_to_index;
    unordered_map<int, string> index_to_substring;
    generateSubstrings(substring_to_index, index_to_substring);
    // Initialize solution
    string current_solution = generateInitialSolution(dataset, threshold, substring_to_index, index_to_substring);
    string best_solution = current_solution;
    double best_quality = calidad_solucion(dataset, threshold, best_solution);
    int dataset_size= dataset.size();
    int best_solution_size = best_solution.size();
    // Simulated Annealing parameters
    double temperature = 1000.0;
    double cooling_rate = 0.99;
    clock_t start_time = clock();

    // Simulated Annealing loop
    int part_size;
    int random_position;
    string sub_solution;
    vector<string> neighbor_solutions;
    string new_solution;
    double elapsed_time;
    int iterations_without_improvement = 0;
    string solution_random;
    double neighbor_solution_quality_in;
    while ((clock() - start_time) / CLOCKS_PER_SEC < max_time_seconds) {
        // Calculate the size of the parts to replace
        part_size = size_calculator(temperature/1000, best_solution_size); // Ensure part_size is a multiple of 3
        random_position = rand() % (best_solution_size - part_size + 1);
        // Replace the parts with new random substrings
        // Extract the substring
        sub_solution = current_solution.substr(random_position, part_size);

        // Generate neighbor solutions for the substring
        neighbor_solutions = generateNeighborSolution(sub_solution, substring_to_index, index_to_substring, temperature/1000);

        for (const string& neighbor_solution : neighbor_solutions) {
            // Replace the original substring with the neighbor solution
            new_solution = current_solution;
            new_solution.replace(random_position, part_size, neighbor_solution);

            double neighbor_quality = calidad_solucion(dataset, threshold, new_solution);
            if (neighbor_quality > best_quality) {
                best_solution = new_solution;
                best_quality = neighbor_quality;
                cout << (trunc(best_quality))/dataset_size << " "<< (clock() - start_time) / CLOCKS_PER_SEC << endl;
                current_solution = new_solution;
                iterations_without_improvement=0;
            }else {
                iterations_without_improvement++;  // Aumentar si no hay mejora
            }
        }  

        temperature *= cooling_rate;
        // Si no hay mejoras durante 20 iteraciones, ajustar el cooling_rate
        if (iterations_without_improvement >= 30) {
            cooling_rate *= 1.01; // Aumentar ligeramente el cooling_rate
            iterations_without_improvement = 0; // Reiniciar el contador
            while (true) {
                // Calculate the size of the parts to replace
                part_size = size_calculator(temperature/1000, best_solution_size);
                random_position = rand() % (best_solution_size - part_size + 1);
                //printf("Part size: %d, random position: %d\n", part_size, random_position);
                // Generate neighbor solutions for the substring
                solution_random = generateNeighborSolutionRandom(part_size, index_to_substring);

                new_solution = current_solution;
                new_solution.replace(random_position, part_size, solution_random);
                neighbor_solution_quality_in = calidad_solucion(dataset, threshold, new_solution);
                current_solution = new_solution;
        
                if (accept_rate(best_quality, neighbor_solution_quality_in, temperature/1000, dataset_size)) {
                    //cout<<".";//ayuda visual de probabilidad
                    current_solution = new_solution;
                    break;
                }

            }
        }

        // Incrementar el cooling rate proporcionalmente al tiempo transcurrido
        elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
        cooling_rate = 0.99 + (0.01 * (elapsed_time / max_time_seconds));
        //printf("%f\n", temperature);
    }

    cout << (trunc(best_quality))/dataset_size << " "<< (clock() - start_time) / CLOCKS_PER_SEC;                
    printf("\n%s", best_solution.c_str());
}

#endif

