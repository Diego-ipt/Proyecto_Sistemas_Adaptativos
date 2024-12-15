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
    if (argc != 18) {
        cerr << "Usage: " << argv[0] << " <dataset_file> <num_subdatasets> <threshold> <max_error> <temperature_pert> <temperature_leap> <cooling_rate> <heat_rate> <iteraciones_max> <time_max> <p> <pe> <pm> <rhoe> <X_INTVL> <X_NUMBER> <tuningMode>" << endl;
        return 1;
    }

    string dataset_file = argv[1];
    int num_subdatasets = atoi(argv[2]);
    int threshold = atoi(argv[3]);
    int max_error = atoi(argv[4]);
    double temperature_pert = atof(argv[5]);
    double temperature_leap = atof(argv[6]);
    double cooling_rate = atof(argv[7]);
    double heat_rate = atof(argv[8]);
    int iteraciones_max = atoi(argv[9]);
    int time_max = atoi(argv[10]);
    unsigned p = atoi(argv[11]);
    double pe = atof(argv[12]);
    double pm = atof(argv[13]);
    double rhoe = atof(argv[14]);
    unsigned X_INTVL = atoi(argv[15]);
    unsigned X_NUMBER = atoi(argv[16]);
    bool tuningMode = atoi(argv[17]);

    vector<string> dataset = loadInputData(dataset_file);

    auto start = chrono::high_resolution_clock::now();
    solutionss result = recombinacion_subdataset(dataset, num_subdatasets, threshold, max_error, temperature_pert, temperature_leap, cooling_rate, heat_rate, iteraciones_max, time_max, p, pe, pm, rhoe, X_INTVL, X_NUMBER, tuningMode);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "Best solution: " << result.solucion << endl;
    cout << "Fitness: " << result.calidad << endl;
    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;

    return 0;
}
