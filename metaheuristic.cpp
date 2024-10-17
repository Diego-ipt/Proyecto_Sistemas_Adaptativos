#include <iostream>
#include <vector>
#include <string>
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
vector<string> generateNeighborSolution(const string& current_solution,unordered_map<string, int> substring_to_index, unordered_map<int, string> index_to_substring){
    vector<string> neighbor_solutions;
    vector<int> solution = compressString(current_solution, substring_to_index);
    int num_columns = solution.size();
    // Generar soluciones vecinas
    for (int j = 1; j < 8; ++j) {
        vector<int> new_solution = solution;
        for (int i = 0; i < num_columns; ++i) {
            new_solution[i] = (solution[j % num_columns] + j * 8) % 64;
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



void cooling_system(const string& metaheuristic_name, const vector<string>& dataset, int max_time_seconds, int threshold) {
    unordered_map<string, int> substring_to_index;
    unordered_map<int, string> index_to_substring;
    generateSubstrings(substring_to_index, index_to_substring);
    // Initialize solution
    string current_solution = generateInitialSolution(dataset, threshold, substring_to_index, index_to_substring);
    string best_solution = current_solution;
    double best_quality = calidad_solucion(dataset, threshold, best_solution);

    // Simulated Annealing parameters
    double temperature = 1000.0;
    double cooling_rate = 0.99;
    clock_t start_time = clock();

    // Simulated Annealing loop
    while ((clock() - start_time) / CLOCKS_PER_SEC < max_time_seconds) {
        // Calculate the size of the parts to replace
        int part_size = static_cast<int>(temperature / dataset.size()) * 3; // Ensure part_size is a multiple of 3
        int random_position = rand() % (current_solution.size() - part_size + 1);
        // Replace the parts with new random substrings
        // Extract the substring
        string sub_solution = current_solution.substr(random_position, part_size);

        // Generate neighbor solutions for the substring
        vector<string> neighbor_solutions = generateNeighborSolution(sub_solution, substring_to_index, index_to_substring);

        for (const string& neighbor_solution : neighbor_solutions) {
            // Replace the original substring with the neighbor solution
            string new_solution = current_solution;
            new_solution.replace(random_position, part_size, neighbor_solution);

            double neighbor_quality = calidad_solucion(dataset, threshold, new_solution);
            if (neighbor_quality > best_quality) {
                best_solution = new_solution;
                best_quality = neighbor_quality;
                cout << "Best quality: " << best_quality << " found at time: " << (clock() - start_time) / CLOCKS_PER_SEC << " seconds" << endl;
                current_solution = new_solution;
            }
        }  

        temperature *= cooling_rate;

        // Incrementar el cooling rate proporcionalmente al tiempo transcurrido
        double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
        cooling_rate = 0.99 + (0.01 * (elapsed_time / max_time_seconds));
    }


    // while ((clock() - start_time) / CLOCKS_PER_SEC < max_time_seconds) {
    //     vector<string> neighbor_solutions = generateNeighborSolution(current_solution, substring_to_index, index_to_substring);
    //     for (const string& neighbor_solution : neighbor_solutions) {
    //         double neighbor_quality = calidad_solucion(dataset, threshold, neighbor_solution);
    //         if (neighbor_quality > best_quality) {
    //             best_solution = neighbor_solution;
    //             best_quality = neighbor_quality;
    //             cout << "Best quality: " << best_quality << " found at time: " << (clock() - start_time) / CLOCKS_PER_SEC << " seconds" << endl;
    //         }

    //         double acceptance_probability = exp((neighbor_quality - best_quality) / temperature);
    //         if (neighbor_quality > best_quality || ((double) rand() / RAND_MAX) < acceptance_probability) {
    //             current_solution = neighbor_solution;
    //         }
    //     }
    //     // Calculate the size of the parts to replace
    //     int part_size = static_cast<int>(temperature / dataset.size()) * 3; // Ensure part_size is a multiple of 3
    //     if (part_size > 0 && part_size <= current_solution.size()) {
    //         // Select a random position in the solution
    //         int random_position = rand() % (current_solution.size() - part_size + 1);
    //             // Replace the parts with new random substrings
    //         for (int i = 0; i < part_size; i += 3) {
    //             string new_substring = generateRandomSubstring(3); // Function to generate a random substring of length 3
    //             current_solution.replace(random_position + i, 3, new_substring);
    //         }
    //     }

    //     temperature *= cooling_rate;

    //     // Incrementar el cooling rate proporcionalmente al tiempo transcurrido
    //     double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
    //     cooling_rate = 0.99 + (0.01 * (elapsed_time / max_time_seconds));
    // }
}

int main(int argc, char* argv[]) {
    if (argc < 4 || string(argv[1]) != "-i" || string(argv[3]) != "-th") {
        cerr << "Usar: " << argv[0] << " -i <instancia-problema> -th threshold" << endl;
        return 1;
    }
    string inputFileName = argv[2];

    int N, M, I; //N cadenas de longitud M
    tie(N, M, I) = parseFileInfo(inputFileName);
    vector<string> input_data = loadInputData(inputFileName);
    double threshold = stod(argv[4])*M; // porcentaje de longitud M
    srand(I + 26999); //random seed 
    int max_time_seconds = 60; // 60 segundos como tiempo máximo
    cooling_system("cooling_system", input_data, max_time_seconds, threshold); // max_time_seconds segundos como tiempo máximo

    return 0;
}