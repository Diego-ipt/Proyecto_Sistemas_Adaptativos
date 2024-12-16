#include "loadinputdata.h"
#include "recombinacion_subdataset.h"
#include <cmath>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[]) {

    // Default values
    int num_subdatasets = 1;
    double threshold = 0.80;
    int max_error = 30;
    double temperature_pert = 400;
    double temperature_leap = 800;
    double cooling_rate = 0.95;
    double heat_rate = 1.5;
    int iteraciones_max = 300;
    int time_max = 60;
    unsigned p = 10;
    double pe = 0.7;
    double pm = 0.1;
    double rhoe = 0.5;
    unsigned X_INTVL = 10;
    unsigned X_NUMBER = 5;
    bool tuningMode = false;
    string dataset_file;

    // Verify that the minimum required arguments are provided
    if (argc < 9) {
        cerr << "Usage: " << argv[0] << "-alg <algorithm_name> -input <dataset_file> -th <threshold> -time <time_in_seconds> [optional parameters]" << endl;
        cerr << "\nOptional parameters:" << endl;
        cerr << "  -numsub <num>      Number of subdatasets" << endl;
        cerr << "  -maxerr <num>      Maximum error" << endl;
        cerr << "  -temppert <val>    Temperature perturbation" << endl;
        cerr << "  -templeap <val>    Temperature leap" << endl;
        cerr << "  -cool <val>        Cooling rate" << endl;
        cerr << "  -heat <val>        Heat rate" << endl;
        cerr << "  -maxiter <num>     Maximum iterations" << endl;
        cerr << "  -p <num>           Parameter p" << endl;
        cerr << "  -pe <val>          Parameter pe" << endl;
        cerr << "  -pm <val>          Parameter pm" << endl;
        cerr << "  -rhoe <val>        Parameter rhoe" << endl;
        cerr << "  -xintvl <num>      X interval" << endl;
        cerr << "  -xnum <num>        X number" << endl;
        cerr << "  -tuning            Enable tuning mode" << endl;
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
    dataset_file = args["-input"];
    threshold = stod(args["-th"]);
    time_max = stoi(args["-time"]);
    vector<string> dataset = loadInputData(dataset_file);

    // Get the optional arguments
    if (args.find("-numsub") != args.end()) num_subdatasets = stoi(args["-numsub"]);
    if (args.find("-maxerr") != args.end()) max_error = stoi(args["-maxerr"]);
    if (args.find("-temppert") != args.end()) temperature_pert = stod(args["-temppert"]);
    if (args.find("-templeap") != args.end()) temperature_leap = stod(args["-templeap"]);
    if (args.find("-cool") != args.end()) cooling_rate = stod(args["-cool"]);
    if (args.find("-heat") != args.end()) heat_rate = stod(args["-heat"]);
    if (args.find("-maxiter") != args.end()) iteraciones_max = stoi(args["-maxiter"]);
    if (args.find("-p") != args.end()) p = stoi(args["-p"]);
    if (args.find("-pe") != args.end()) pe = stod(args["-pe"]);
    if (args.find("-pm") != args.end()) pm = stod(args["-pm"]);
    if (args.find("-rhoe") != args.end()) rhoe = stod(args["-rhoe"]);
    if (args.find("-xintvl") != args.end()) X_INTVL = stoi(args["-xintvl"]);
    if (args.find("-xnum") != args.end()) X_NUMBER = stoi(args["-xnum"]);
    if (args.find("-tuning") != args.end()) tuningMode = (args["-tuning"] == "true");

    
    auto start = chrono::high_resolution_clock::now();
    int threshold_entero = threshold * dataset[0].size();
    solutionss result = recombinacion_subdataset(dataset, num_subdatasets, threshold_entero, max_error, temperature_pert, temperature_leap, cooling_rate, heat_rate, iteraciones_max, time_max, p, pe, pm, rhoe, X_INTVL, X_NUMBER, tuningMode);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    // if(!tuningMode){
    //     cout << "Best solution: " << result.solucion << endl;
    //     cout << "Fitness: " << result.calidad << endl;
    //     cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
    // }
    return 0;
}
