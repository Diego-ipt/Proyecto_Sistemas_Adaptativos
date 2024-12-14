#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "merge_model.h"
#include "loadinputdata.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Default values
    int population_size = 30;
    double pm = 0.1;
    int elite_count = 5;
    double threshold = 0.85;
    int max_time_genetic = 10; // in seconds
    int max_error = 30;
    double temperature_pert = 400;
    double temperature_leap = 800;
    double cooling_rate = 0.95;
    double heat_rate = 1.5;
    bool tuningMode = false;
    int iteraciones_max = 300;
    int N_parents = 2; // Default value for N_parents
    int n_particiones = 150;

    // Verify that the minimum required arguments are provided
    if (argc < 9) {
        cerr << "Usage: " << argv[0] << " -alg <algorithm_name> -input <inputFileName> -th <threshold> -time <time> [optional parameters]" << endl;
        return 1;
    }

    // Parse arguments
    unordered_map<string, string> args;
    for (int i = 1; i < argc; i += 2) {
        if (i + 1 < argc) {
            args[argv[i]] = argv[i + 1];
        }
    }

    // Mandatory parameters
    string algorithm_name = args["-alg"];
    string inputFileName = args["-input"];
    threshold = stod(args["-th"]);
    max_time_genetic = stoi(args["-time"]);

    // Process input file
    int N, M, I;
    tie(N, M, I) = parseFileInfo(inputFileName);
    vector<string> dataset = loadInputData(inputFileName);

    // Get the optional arguments
    if (args.find("-pop_size") != args.end()) population_size = stoi(args["-pop_size"]);
    if (args.find("-pm") != args.end()) pm = stod(args["-pm"]);
    if (args.find("-elite") != args.end()) elite_count = stoi(args["-elite"]);
    if (args.find("-max_error") != args.end()) max_error = stoi(args["-max_error"]);
    if (args.find("-temp_pert") != args.end()) temperature_pert = stod(args["-temp_pert"]);
    if (args.find("-temp_leap") != args.end()) temperature_leap = stod(args["-temp_leap"]);
    if (args.find("-cool_rate") != args.end()) cooling_rate = stod(args["-cool_rate"]);
    if (args.find("-heat_rate") != args.end()) heat_rate = stod(args["-heat_rate"]);
    if (args.find("-tuning") != args.end()) tuningMode = (args["-tuning"] == "true");
    if (args.find("-iter_max") != args.end()) iteraciones_max = stoi(args["-iter_max"]);
    if (args.find("-N_parents") != args.end()) N_parents = stoi(args["-N_parents"]);
    if (args.find("-n_part") != args.end()) n_particiones = stoi(args["-n_part"]);

    // Adjust threshold based on M
    threshold *= M;

    // Seed random number generator
    srand(I + 26999);

    // Run the genetic algorithm
    geneticAlgorithm_merge(population_size, pm, 
    elite_count, dataset, threshold, max_time_genetic, max_error, temperature_pert, 
    temperature_leap, cooling_rate, heat_rate, tuningMode, iteraciones_max, N_parents, n_particiones);

    return 0;
}