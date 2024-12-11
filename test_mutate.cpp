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
#include "metaheuristic_functions.h"

using namespace std;

int main() {
    // Example dataset
    vector<string> dataset = {
        "ATCGATCGATCG",
        "GCTAGCTAGCTA",
        "CGTACGTACGTA",
        "TACGTACGTACG","ATCGATCGATCG", "GCTAGCTAGCTA", "CGTACGTACGTA", "TACGTACGTACG",
        "ATCGATCGATCG", "GCTAGCTAGCTA", "CGTACGTACGTA", "TACGTACGTACG",
        "ATCGATCGATCG", "GCTAGCTAGCTA", "CGTACGTACGTA", "TACGTACGTACG",
        "ATCGATCGATCG", "GCTAGCTAGCTA", "CGTACGTACGTA", "TACGTACGTACG",
        "ATCGATCGATCG", "GCTAGCTAGCTA", "CGTACGTACGTA", "TACGTACGTACG",
        "ATCGATCGATCG", "GCTAGCTAGCTA", "CGTACGTACGTA", "TACGTACGTACG",
        "ATCGATCGATCG", "GCTAGCTAGCTA", "CGTACGTACGTA", "TACGTACGTACG",
        "ATCGATCGATCG", "GCTAGCTAGCTA", "CGTACGTACGTA", "TACGTACGTACG",
        "ATCGATCGATCG", "GCTAGCTAGCTA", "CGTACGTACGTA", "TACGTACGTACG",
        "ATCGATCGATCG", "GCTAGCTAGCTA", "CGTACGTACGTA", "TACGTACGTACG"
    };


    // Parameters
    int threshold = 11;
    int max_error = 20;
    double temperature_pert = 1000;
    double temperature_leap = 2000;
    double cooling_rate = 0.99;
    double heat_rate = 1.01;
    int iteraciones_max = 999;

    // Initialize maps
    unordered_map<string, int> substring_to_index;
    unordered_map<int, string> index_to_substring;
    generateSubstrings(substring_to_index, index_to_substring);

    // Generate initial solution
    string best_solution = generateInitialSolution(dataset, threshold, substring_to_index, index_to_substring);
    double best_quality = calidad_solucion(dataset, threshold, best_solution);

    // Apply cooling system plus for 10 seconds
    auto start_time = chrono::high_resolution_clock::now();
    pair<string, double> result = make_pair(best_solution, best_quality);
    double best_overall_quality = best_quality;
    string best_overall_solution = best_solution;

    while (chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start_time).count() < 10) {
        result = cooling_system_plus(dataset, result.first, result.second, threshold, max_error, temperature_pert, temperature_leap, cooling_rate, heat_rate, iteraciones_max, substring_to_index, index_to_substring);
        
        // Check if the new solution is better than the best overall solution
        if (result.second > best_overall_quality) {
            best_overall_quality = result.second;
            best_overall_solution = result.first;
        }

        // Output results
        auto current_time = chrono::high_resolution_clock::now();
        auto elapsed_time = chrono::duration_cast<chrono::seconds>(current_time - start_time).count();
        cout << "Elapsed time: " << elapsed_time << " seconds" << endl;
        cout << "Current best solution: " << result.first << endl;
        cout << "Current best quality: " << result.second << endl;
    }

    // Output the best overall solution and quality
    cout << "Best overall solution: " << best_overall_solution << endl;
    cout << "Best overall quality: " << best_overall_quality << endl;

    return 0;
}