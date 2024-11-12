#include "loadinputdata.h"
#include "funciones_greedy.h"
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
    
    if (argc < 4 || string(argv[1]) != "-i" || string(argv[3]) != "-th") {
        cerr << "Usar: " << argv[0] << " -i <instancia-problema> -th threshold [-t maxsecs] [-alpha alpha]" << endl;
        return 1;
    }
    string inputFileName = argv[2];

    int N, M, I; //N cadenas de longitud M
    tie(N, M, I) = parseFileInfo(inputFileName);
    vector<string> input_data = loadInputData(inputFileName);
    double threshold = stod(argv[4])*M; // porcentaje de longitud M
    srand(I + 26999); //random seed 
    //Default best value for alpha from irace tuning
    double alpha= 0.99; // Default value for alpha for threshold 0.75
    //double alpha = 0.73 // Default value for alpha for threshold 0.8
    //double alpha = 0.87 // Default value for alpha for threshold 0.85
    int maxsecs = -1;

    // Verificar si se usa el modo tuning y alfa
    int tuningMode = false;
    for (int i = 5; i < argc; ++i) {
        if (string(argv[i]) == "-alpha" && i + 1 < argc) {
            alpha = stod(argv[i + 1]);
            if (alpha < 0.0 || alpha > 1.0) {
                cerr << "El valor de alpha debe estar entre 0 y 1." << endl;
                return 1;
            }
            i++; // Skip the next argument because it's part of -alpha
        }
        else if (string(argv[i]) == "-tuning" && i + 1 < argc && string(argv[i + 1]) == "1") {
            tuningMode = true;  // Set tuning mode to true
            i++; // Skip the next argument because it's part of -tuning
        }
        else if (string(argv[i]) == "-t" && i + 1 < argc) {
            maxsecs = stoi(argv[i + 1]);
            i++; // Skip the next argument because it's part of -t
        }
    }

    unordered_map<string, int> substring_to_index;
    unordered_map<int, string> index_to_substring;
    generateSubstrings(substring_to_index, index_to_substring);

    auto start = chrono::high_resolution_clock::now();
    string solution = greedyHeuristicFFMS(input_data, M, substring_to_index, index_to_substring, threshold, alpha);
    double calidad_sin_norm = trunc(calidad_solucion(input_data, threshold, solution));
    double calidad = calidad_sin_norm  /input_data.size();
    double calidad_tuning = calidad_sin_norm * -1.0; // Se invierte la calidad para el modo tuning
    auto end = chrono::high_resolution_clock::now();
    double tiempo_ejecucion = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    if (tuningMode) {
        cout << calidad_tuning;
    } else {
        cout << "Calidad: " << calidad << endl;
        cout << "Tiempo de ejecución: " << tiempo_ejecucion << " ms" << endl;
    }

    /*
    ofstream outputFile("results_greedy.csv", ios::app);
    if (!outputFile.is_open()) {
        cerr << "Error abriendo el archivo: results_greedy.csv" << endl;
        return 1;
    }
    outputFile << N << ";" << M << ";" << tiempo_ejecucion << ";" << calidad << endl;
    outputFile.close();
    */
    return 0;
}