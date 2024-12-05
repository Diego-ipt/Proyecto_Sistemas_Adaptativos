#include <iostream>
#include <vector>
#include <string>
#include "merge_model.h"
#include "loadinputdata.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Default values
    int population_size = 100;
    int random_population_size = 10;
    double pm = 0.1;
    int elite_count = 5;
    double threshold = 0.9;
    int max_time_genetic = 10; // in seconds
    int max_error = 10;
    double temperature_pert = 0.1;
    double temperature_leap = 0.2;
    double cooling_rate = 0.95;
    double heat_rate = 1.05;
    bool tuningMode = false;
    int iteraciones_max = 1000;

    // Verify that the minimum required arguments are provided
    // algorithm_name, inputFileName, threshold
    if (argc < 6) {
        cerr << "Usage: " << argv[0] << " <algorithm_name> <inputFileName> <threshold> [optional parameters]" << endl;
        return 1;
    }

    // Mandatory parameters
    string algorithm_name = argv[1];
    string inputFileName = argv[2];
    threshold = stod(argv[3]);

    // Process optional arguments
    // population_size, random_population_size, pm, elite_count, max_error, temperature_pert, temperature_leap, cooling_rate, heat_rate, tuningMode, iteraciones_max
    for (int i = 4; i < argc; i += 2) {
        string param = argv[i];
        if (param == "--population_size" && i + 1 < argc) {
            population_size = stoi(argv[i + 1]);
        } else if (param == "--random_population_size" && i + 1 < argc) {
            random_population_size = stoi(argv[i + 1]);
        } else if (param == "--pm" && i + 1 < argc) {
            pm = stod(argv[i + 1]);
        } else if (param == "--elite_count" && i + 1 < argc) {
            elite_count = stoi(argv[i + 1]);
        } else if (param == "--max_error" && i + 1 < argc) {
            max_error = stoi(argv[i + 1]);
        } else if (param == "--temperature_pert" && i + 1 < argc) {
            temperature_pert = stod(argv[i + 1]);
        } else if (param == "--temperature_leap" && i + 1 < argc) {
            temperature_leap = stod(argv[i + 1]);
        } else if (param == "--cooling_rate" && i + 1 < argc) {
            cooling_rate = stod(argv[i + 1]);
        } else if (param == "--heat_rate" && i + 1 < argc) {
            heat_rate = stod(argv[i + 1]);
        } else if (param == "--tuningMode" && i + 1 < argc) {
            tuningMode = (string(argv[i + 1]) == "true");
        } else if (param == "--iteraciones_max" && i + 1 < argc) {
            iteraciones_max = stoi(argv[i + 1]);
        }
    }

    // Process input file
    int N, M, I;
    tie(N, M, I) = parseFileInfo(inputFileName);
    vector<string> dataset = loadInputData(inputFileName);

    // Adjust threshold based on M
    threshold *= M;

    // Seed random number generator
    srand(I + 26999);

    // Run the genetic algorithm
    geneticAlgorithm_merge(population_size, random_population_size, 
    pm, elite_count, dataset, threshold, max_time_genetic, max_error, temperature_pert
    , temperature_leap, cooling_rate, heat_rate, tuningMode, iteraciones_max);

    return 0;
}