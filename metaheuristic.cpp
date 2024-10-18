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
#include <random>

using namespace std;

random_device rd;  // Entropía para inicializar el motor aleatorio
mt19937 generator(rd()); // Motor Mersenne Twister para generar números
uniform_real_distribution<> distribution(0.0, 1.0);  // Distribución en [0, 1)

//Función para generar la solución inicial
string generateInitialSolution(const vector<string>& dataset, double threshold, unordered_map<string, int> substring_to_index, unordered_map<int, string> index_to_substring) {
    int M = dataset[4].size(); // Longitud de las cadenas
    double alpha = 1.0; // Valor de alpha
    string solution = greedyHeuristicFFMS(dataset, M, substring_to_index, index_to_substring, threshold, alpha);
    return solution;
}

//Función para generar soluciones vecinas para búsqueda local
vector<string> generateNeighborSolution(const string& current_solution, unordered_map<string, int> substring_to_index, unordered_map<int, string> index_to_substring, double temperature) {
    vector<string> neighbor_solutions;
    vector<int> solution = compressString(current_solution, substring_to_index);
    int num_columns = solution.size();

    // Generar soluciones vecinas
    for (int j = 1; j < 8; ++j) {
        vector<int> new_solution = solution;
        for (int i = 0; i < num_columns; ++i) {
            new_solution[i] = (solution[j % num_columns] + static_cast<int>(j * 8));

            //perturbación
            int random_position = (rand() % 64) * temperature;
            if (rand() % 2) {
                new_solution[i] += random_position;
            } else {
                new_solution[i] -= random_position;
                new_solution[i] = abs(new_solution[i]);
            }
            new_solution[i] %= 64;
        }
        // Convertir new_solution a string
        string new_solution_str;
        for (int pos : new_solution) {
            new_solution_str += getSubstringByPosition(index_to_substring, pos);
        }
        neighbor_solutions.push_back(new_solution_str);
    }

    return neighbor_solutions;
}

//Generar vecino aleatorio
string generateNeighborSolutionRandom(int size, unordered_map<int, string> index_to_substring) {
    string neighbor_solution;
    for (int j = 0; j < size; j += 3) {
        neighbor_solution += getSubstringByPosition(index_to_substring, rand() % 64);
    }
    return neighbor_solution;
}

// Función para el criterio de aceptación
bool acceptance_criterion(double temperatura, double diferencias_calidad) {
    if (diferencias_calidad < 0) {
        return true;
    } else {
        double r = distribution(generator);
        double probabilidad_aceptacion = exp(-diferencias_calidad / temperatura);
        cout << "r: " << r << " probabilidad_aceptacion: " << probabilidad_aceptacion << endl;
        if (r < probabilidad_aceptacion) {
            return true;
        } else {
            cout << "r >= exp: false" << endl;
            return false;
        }
    }
}

// Enfriamiento simulado combinado con búsqueda local
void cooling_system(const string& metaheuristic_name, const vector<string>& dataset, int max_time_seconds, int threshold) {
    unordered_map<string, int> substring_to_index;
    unordered_map<int, string> index_to_substring;
    generateSubstrings(substring_to_index, index_to_substring);

    string current_solution = generateInitialSolution(dataset, threshold, substring_to_index, index_to_substring);
    string best_solution = current_solution;
    double best_quality = calidad_solucion(dataset, threshold, best_solution);
    int dataset_size = dataset.size();
    int best_solution_size = best_solution.size();

    // Parámetros de enfriamiento simulado
    double temperature = 1000.0;
    double min_temp = 0.00001;
    double cooling_rate = 0.9;

    int part_size;
    int random_position;
    string sub_solution;
    vector<string> neighbor_solutions;
    string neighbor_solution_random;
    string new_solution;
    double elapsed_time;

    clock_t start_time = clock();
    while ((clock() - start_time) / CLOCKS_PER_SEC < max_time_seconds) {
        // Tamaño de las partes a reemplazar
        part_size = static_cast<int>(ceil((temperature / 1000) * (dataset_size * 0.04)) * 3);  // Múltiplo de 3
        random_position = rand() % (best_solution_size - part_size + 1);

        // Generar soluciones vecinas para búsqueda local
        sub_solution = current_solution.substr(random_position, part_size);
        neighbor_solutions = generateNeighborSolution(sub_solution, substring_to_index, index_to_substring, temperature / 1000);

        // Búsqueda Local
        for (const string& neighbor_solution : neighbor_solutions) {
            new_solution = current_solution;
            new_solution.replace(random_position, part_size, neighbor_solution);
            double neighbor_quality = calidad_solucion(dataset, threshold, new_solution);
            if (neighbor_quality > best_quality) {
                best_solution = new_solution;
                best_quality = neighbor_quality;
                cout << "Best quality: " << best_quality << ", quality estandar(treshold aceptada): " << (trunc(best_quality)) / dataset_size << " found at time: " << (clock() - start_time) / CLOCKS_PER_SEC << " seconds" << endl;
                current_solution = new_solution;
            }
        }

        // Generar solución aleatoria y aceptar/rechazar
        random_position = rand() % (best_solution_size - part_size + 1);
        neighbor_solution_random = generateNeighborSolutionRandom(sub_solution.size(), index_to_substring);
        new_solution = current_solution;
        new_solution.replace(random_position, part_size, neighbor_solution_random);
        double neighbor_random_quality = calidad_solucion(dataset, threshold, new_solution);
        double diferencias_calidad = (neighbor_random_quality - best_quality) * 100;
        if (acceptance_criterion(temperature, diferencias_calidad)) {
            current_solution = new_solution;
        }

        // Reducir la temperatura
        temperature *= cooling_rate;
        elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
    }
    printf("Best solution: %s\n", best_solution.c_str());
}

int main(int argc, char* argv[]) {
    if (argc < 4 || string(argv[1]) != "-i" || string(argv[3]) != "-th") {
        cerr << "Usar: " << argv[0] << " -i <instancia-problema> -th threshold" << endl;
        return 1;
    }
    string inputFileName = argv[2];

    int N, M, I;
    tie(N, M, I) = parseFileInfo(inputFileName);
    vector<string> input_data = loadInputData(inputFileName);
    double threshold = stod(argv[4]) * M;
    srand(I + 26999); // Random seed
    int max_time_seconds = 60; // 60 segundos como tiempo máximo
    cooling_system("cooling_system", input_data, max_time_seconds, threshold);
    printf("Fin del programa\n");
    return 0;
}
