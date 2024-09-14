#include "loadinputdata.h"
#include "funciones_greedy.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <cstdlib>

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
    double threshold = stod(argv[4])*M; //puede ser 0.8 o 0.85 tambien 
    srand(I + 26999); //random seed 

    unordered_map<string, int> substring_to_index;
    unordered_map<int, string> index_to_substring;
    generateSubstrings(substring_to_index, index_to_substring);
    string solution = greedyHeuristicFFMS(input_data, M, substring_to_index, index_to_substring, threshold);
    double calidad = calidad_solucion(input_data, threshold, solution);

    cout << "Solucion: " << solution << endl;
    cout << "Calidad: " << calidad << endl;
    return 0;
}