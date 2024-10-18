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

//generar neighbor random
vector<string> generateNeighborSolutionRandom(int size, unordered_map<int, string> index_to_substring){
    vector<string> neighbor_solutions;
    for (int i = 0; i < 10; ++i) {
        string new_solution_sub_str;
        for (int j = 0; j < size; j+=3) {
            new_solution_sub_str+=getSubstringByPosition(index_to_substring, rand() % 64);
        }
        neighbor_solutions.push_back(new_solution_sub_str);
    }
    return neighbor_solutions;
}

vector<int> calidad_solucion_neighbor(const string& current_solution, int random_position, 
    const vector<string>& dataset, int threshold, 
    const vector<string>& neighbor_solutions) {
    vector<int> qualities;
    int part_size = neighbor_solutions[0].size();
    int size = current_solution.size();
    string new_solution;
    for (const string& neighbor_solution : neighbor_solutions) {
        new_solution = current_solution;
        new_solution.replace(random_position, part_size, neighbor_solution);
        int quality = calidad_solucion(dataset, threshold, new_solution);
        qualities.push_back(quality);
    }
    return qualities;
}

void local_search(string& current_solution, int random_position, int part_size, const vector<string>& dataset, double threshold, double temperature, string& best_solution, double& best_quality, const unordered_map<string, int>& substring_to_index, const unordered_map<int, string>& index_to_substring, clock_t start_time, int dataset_size) {
    // Extract the substring
    string sub_solution = current_solution.substr(random_position, part_size);

    // Generate neighbor solutions for the substring
    vector<string> neighbor_solutions = generateNeighborSolution(sub_solution, substring_to_index, index_to_substring, temperature / 1000);
    for (const string& neighbor_solution : neighbor_solutions) {
        // Replace the original substring with the neighbor solution
        string new_solution = current_solution;
        new_solution.replace(random_position, part_size, neighbor_solution);

        double neighbor_quality = calidad_solucion(dataset, threshold, new_solution);
        if (neighbor_quality > best_quality) {
            best_solution = new_solution;
            best_quality = neighbor_quality;
            cout << "Best quality: " << best_quality << ", quality estandar(treshold aceptada): " << (trunc(best_quality)) / dataset_size << " found at time: " << (clock() - start_time) / CLOCKS_PER_SEC << " seconds" << endl;
            current_solution = new_solution;
        }
    }
}

bool accept_rate(double best_quality, double neighbor_quality, double temperature, int dataset_size) {

    // Calcular la diferencia de calidad
    double delta_quality = (neighbor_quality - best_quality)/dataset_size;
    bool accept = false;

    // Si la nueva solución es peor, aceptarla con una probabilidad P
    double probability = exp(-delta_quality / temperature);
    probability--;
    probability*=10000;
    //cout << "Probabilidad: " << probability << endl;

    double aleatorio= static_cast<double>(rand()) / RAND_MAX;
    //cout << "Aleatorio: " << aleatorio << endl;
    
    if (aleatorio < probability) {
        accept= true;
    }
    // Si no se acepta la nueva solución, mantener la calidad actual
    return accept;  
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
    vector<string> neighbor_solutions_random;
    string new_solution;
    double elapsed_time;
    // Calculate the size of the parts to replace
    part_size = trunc(temperature/1000) /(dataset_size) * 3;
    random_position = rand() % (best_solution_size - part_size + 1);
    // Call local_search function
    local_search(current_solution, random_position, part_size, dataset, threshold, 
    temperature, best_solution, best_quality, substring_to_index, 
    index_to_substring, start_time, dataset_size);

    while ((clock() - start_time) / CLOCKS_PER_SEC < max_time_seconds) {
        // Calculate the size of the parts to replace
        part_size = trunc((temperature/1000) * (dataset_size)) * 3; // Ensure part_size is a multiple of 3
        random_position = rand() % (best_solution_size - part_size + 1);
        local_search(current_solution, random_position, part_size, dataset, threshold, 
    temperature, best_solution, best_quality, substring_to_index, 
    index_to_substring, start_time, dataset_size);
        random_position = rand() % (best_solution_size - part_size + 1);

        // Replace the parts with new random substrings
        // Extract the substring
        sub_solution = current_solution.substr(random_position, part_size);

        // Generate neighbor solutions for the substring
        neighbor_solutions_random = generateNeighborSolutionRandom(sub_solution.size(), index_to_substring);
        for (const string& neighbor_solution : neighbor_solutions_random) {
            // Replace the original substring with the neighbor solution
            new_solution = current_solution;
            new_solution.replace(random_position, part_size, neighbor_solution);
            double neighbor_solution_quality_in = calidad_solucion(dataset, threshold, new_solution);

            if (accept_rate(best_quality, neighbor_solution_quality_in, temperature, dataset_size)) {
                // best_solution = new_solution;
                // best_quality = neighbor_quality;
                cout<<".";//ayuda visual de probabilidad
                // cout << "Leap quality: " << neighbor_quality<< ", quality estandar(treshold aceptada): " << (trunc(neighbor_quality))/dataset_size << " found at time: " << (clock() - start_time) / CLOCKS_PER_SEC << " seconds" << endl;
                current_solution = new_solution;
                local_search(current_solution, random_position, part_size, dataset, threshold, temperature, best_solution, best_quality, substring_to_index, index_to_substring, start_time, dataset_size);
                //temperature /= cooling_rate;
            }else{
                cout<<"-";//ayuda visual de probabilidad
            }
        }

        // Reduce the temperature
        temperature *= cooling_rate;

        // Incrementar el cooling rate proporcionalmente al tiempo transcurrido
        elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
        cooling_rate = 0.99 + (0.01 * (elapsed_time / max_time_seconds));
        //printf("%f\n", temperature);
    }
    cout << " \nBest solution: " << best_solution << "\nBest quality: " << best_quality << endl;
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
    int max_time_seconds = 20; // 60 segundos como tiempo máximo
    cooling_system("cooling_system", input_data, max_time_seconds, threshold); // max_time_seconds segundos como tiempo máximo
    cout<<"time: "<<max_time_seconds<<("\nFin del programa\n");
    return 0;
}