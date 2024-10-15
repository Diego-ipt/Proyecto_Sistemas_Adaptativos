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

std::string generateInitialSolution(const std::vector<std::string>& dataset);
std::string generateNeighborSolution(const std::string& current_solution, double temperature);

void cooling_system(const std::string& metaheuristic_name, const std::vector<std::string>& dataset, int max_time_seconds, int threshold) {
    // Initialize solution
    std::string current_solution = generateInitialSolution(dataset);
    std::string best_solution = current_solution;
    double best_quality = calidad_solucion(dataset, threshold, best_solution);

    // Simulated Annealing parameters
    double temperature = 1000.0;
    double cooling_rate = 0.99;
    std::clock_t start_time = std::clock();

    while ((std::clock() - start_time) / CLOCKS_PER_SEC < max_time_seconds) {
        std::string neighbor_solution = generateNeighborSolution(current_solution, temperature);
        double neighbor_quality = calidad_solucion(dataset, threshold, neighbor_solution);

        if (neighbor_quality > best_quality) {
            best_solution = neighbor_solution;
            best_quality = neighbor_quality;
            std::cout << "New best quality: " << best_quality << " found at time: " << (std::clock() - start_time) / CLOCKS_PER_SEC << " seconds" << std::endl;
        }

        double acceptance_probability = exp((neighbor_quality - best_quality) / temperature);
        if (neighbor_quality > best_quality || ((double) rand() / RAND_MAX) < acceptance_probability) {
            current_solution = neighbor_solution;
        }

        temperature *= cooling_rate;
        
        // Incrementar el cooling rate proporcionalmente al tiempo transcurrido
        double elapsed_time = (std::clock() - start_time) / CLOCKS_PER_SEC;
        cooling_rate = 0.99 + (0.01 * (elapsed_time / max_time_seconds));
    }

    std::cout << "Best quality: " << best_quality << " found at time: " << (std::clock() - start_time) / CLOCKS_PER_SEC << " seconds" << std::endl;
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