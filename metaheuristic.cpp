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
#include "test_greedy.cpp"

using namespace std;

vector<int> calidad_solucion_simple(const vector<string>& dataset, string solution_string) {
    // Evaluar la calidad de la solución
    int size = solution_string.size();
    double calidad = 0;
    vector<int> calidad_filas;
    for(const auto& str : dataset){
        int total_distance = 0;
        for (int i = 0; i < size; i += 3) {
            string substring_solution = solution_string.substr(i, 3);
            string substring_str_dataset = str.substr(i, 3);
            total_distance += hammingDistance(substring_solution, substring_str_dataset);
        }
        calidad_filas.push_back(total_distance);
    }

    return calidad_filas;
}


string generateInitialSolution(const vector<string>& dataset, int threshold ) {
    // Asumimos que los valores de M, threshold y alpha son conocidos o se pueden calcular
    int M = dataset[0].size(); // Longitud de las cadenas
    double threshold = threshold;
    double alpha = 1.0; // Valor de alpha

    unordered_map<string, int> substring_to_index;
    unordered_map<int, string> index_to_substring;
    generateSubstrings(substring_to_index, index_to_substring);

    // Generar la solución inicial usando la heurística greedy
    string solution = greedyHeuristicFFMS(dataset, M, substring_to_index, index_to_substring, threshold, alpha);
    return solution;
}


vector<string> generateNeighborSolution(const string& current_solution, double temperature){
    unordered_map<string, int> substring_to_index;
    unordered_map<int, string> index_to_substring;
    vector<string> neighbor_solutions;
    vector<int> solution = compressString(current_solution, substring_to_index);
    int num_columns = solution.size();
    // Generar soluciones vecinas
    for (int j = 1; j < 4; ++j) {
        vector<int> new_solution = solution;
        for (int i = 0; i < num_columns; ++i) {
            new_solution[i] = (solution[j % num_columns] + j * 16) % 64;
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

bool analyzeQualityImprovement(const vector<int>& calidad_filas, int threshold) {
    int count = 0;
    int sum = 0;
    for (int quality : calidad_filas) {
        if (quality < threshold) {
            sum += quality;
            count++;
        }
    }
    if (count == 0) return false;
    double average_quality = static_cast<double>(sum) / count;
    return average_quality < threshold;
}

void cooling_system(const string& metaheuristic_name, const vector<string>& dataset, int max_time_seconds, int threshold) {
    // Initialize solution
    string current_solution = generateInitialSolution(dataset, threshold);
    string best_solution = current_solution;
    double best_quality = calidad_solucion(dataset, threshold, best_solution);

    // Simulated Annealing parameters
    double temperature = 1000.0;
    double cooling_rate = 0.99;
    clock_t start_time = clock();

    while ((clock() - start_time) / CLOCKS_PER_SEC < max_time_seconds) {
        vector<string> neighbor_solutions = generateNeighborSolution(current_solution, temperature);
        for (const string& neighbor_solution : neighbor_solutions) {
            vector<int> calidad_filas = calidad_solucion_simple(dataset, neighbor_solution);
            if (analyzeQualityImprovement(calidad_filas, threshold)) {
                double neighbor_quality = calidad_solucion(dataset, threshold, neighbor_solution);

                if (neighbor_quality > best_quality) {
                    best_solution = neighbor_solution;
                    best_quality = neighbor_quality;
                    cout << "New best quality: " << best_quality << " found at time: " << (clock() - start_time) / CLOCKS_PER_SEC << " seconds" << endl;
                }

                double acceptance_probability = exp((neighbor_quality - best_quality) / temperature);
                if (neighbor_quality > best_quality || ((double) rand() / RAND_MAX) < acceptance_probability) {
                    current_solution = neighbor_solution;
                }
            }
        }

        temperature *= cooling_rate;
        
        // Incrementar el cooling rate proporcionalmente al tiempo transcurrido
        double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
        cooling_rate = 0.99 + (0.01 * (elapsed_time / max_time_seconds));
    }

    cout << "Best quality: " << best_quality << " found at time: " << (clock() - start_time) / CLOCKS_PER_SEC << " seconds" << endl;
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

    cooling_system("cooling_system", input_data, 60, threshold); // 60 segundos como tiempo máximo

    return 0;
}