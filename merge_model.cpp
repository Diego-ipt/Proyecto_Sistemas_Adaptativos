#include <iostream>
#include <vector>
#include <string>
#include "merge_model.h"
#include "loadinputdata.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Default values
    int population_size = 100;
    int random_population_size = 20;
    int generations = 100;
    double threshold = 0.5;
    int max_error = 10;
    double temperature_pert = 0.1;
    double temperature_leap = 0.1;
    double cooling_rate = 0.99;
    double heat_rate = 1.01;
    bool tuningMode = false;
    int max_time_seconds = 2;
    int iteraciones_max = 100;
    int elite_count = 10;

    // Verify that the minimum required arguments are provided
    if (argc < 5 || string(argv[2]) != "-i" || string(argv[4]) != "-th") {
        cerr << "Usage: " << argv[0] << " <algorithm_name> -i <input_file> -th <threshold> [options]" << endl;
        cerr << "Options:\n"
             << "  -t <max_time_seconds>\n"
             << "  -ps <population_size>\n"
             << "  -rps <random_population_size>\n"
             << "  -g <generations>\n"
             << "  -me <max_error>\n"
             << "  -tp <temperature_pert>\n"
             << "  -tl <temperature_leap>\n"
             << "  -cr <cooling_rate>\n"
             << "  -hr <heat_rate>\n"
             << "  -ec <elite_count>\n"
             << "  -tuning <0|1> Enable tuning mode (default: 0)" << endl;
        return 1;
    }

    // Mandatory parameters
    string algorithm_name = argv[1];
    string inputFileName = argv[3];
    threshold = stod(argv[5]);

    // Process optional arguments
    for (int i = 6; i < argc; i += 2) {
        string opt = argv[i];
        if (opt == "-t") max_time_seconds = stoi(argv[i + 1]);
        else if (opt == "-ps") population_size = stoi(argv[i + 1]);
        else if (opt == "-rps") random_population_size = stoi(argv[i + 1]);
        else if (opt == "-g") generations = stoi(argv[i + 1]);
        else if (opt == "-me") max_error = stoi(argv[i + 1]);
        else if (opt == "-tp") temperature_pert = stod(argv[i + 1]);
        else if (opt == "-tl") temperature_leap = stod(argv[i + 1]);
        else if (opt == "-cr") cooling_rate = stod(argv[i + 1]);
        else if (opt == "-hr") heat_rate = stod(argv[i + 1]);
        else if (opt == "-ec") elite_count = stoi(argv[i + 1]);
        else if (opt == "-tuning") tuningMode = (stoi(argv[i + 1]) != 0);
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
    geneticAlgorithm_merge(population_size, random_population_size, generations, dataset, threshold, max_error, temperature_pert, temperature_leap, cooling_rate, heat_rate, tuningMode, max_time_seconds, iteraciones_max, elite_count);

    return 0;
}