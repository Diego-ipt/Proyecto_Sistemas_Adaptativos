#include "loadinputdata.h"
#include "funciones_greedy.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <iomanip>

using namespace std;

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
    double alpha = 1.0;

    // Leer parametro opcional -alpha
    for (int i = 5; i < argc; ++i) {
        if (string(argv[i]) == "-alpha" && i + 1 < argc) {
            alpha = stod(argv[i + 1]);
            if (alpha < 0.0 || alpha > 1.0) {
                cerr << "El valor de alpha debe estar entre 0 y 1." << endl;
                return 1;
            }
        }
    }

    unordered_map<string, int> substring_to_index;
    unordered_map<int, string> index_to_substring;
    generateSubstrings(substring_to_index, index_to_substring);

    auto start = chrono::high_resolution_clock::now();
    string solution = greedyHeuristicFFMS(input_data, M, substring_to_index, index_to_substring, threshold, alpha);
    double calidad = calidad_solucion(input_data, threshold, solution);
    auto end = chrono::high_resolution_clock::now();
    double tiempo_ejecucion = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout<< tiempo_ejecucion << endl; // tiempo de ejecucion en ms
    cout << calidad << endl;

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