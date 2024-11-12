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
#include "metaheuristic_functions.h"



int main(int argc, char* argv[]) {
    // Verificar que se han proporcionado todos los argumentos
    if (argc < 7 || string(argv[2]) != "-i" || string(argv[4]) != "-t" || string(argv[6]) != "-th") {
        cerr << "Uso: " << argv[0] << " <nombremetaheuristica> -i <instancia-problema> -t <tiempo-max-segundos> -th <threshold> [-temperature <temp> -cooling_rate <cool> -tuning <0|1>]" << endl;
        return 1;
    }
    
    string metaheuristic_name = argv[1];          // Nombre de la metaheurística
    string inputFileName = argv[3];               // Nombre de la instancia del problema
    int max_time_seconds = stoi(argv[5]);         // Tiempo máximo en segundos
    double threshold = stod(argv[7]);             // Umbral (threshold)

    // Parámetros opcionales
    double temperature = 1000.0;                      // Default temperature
    double cooling_rate = 0.99;                      // Default cooling rate
    bool tuningMode = false;                         // Flag for tuning mode

    // Parse 
    for (int i = 8; i < argc; ++i) {
        if (string(argv[i]) == "-tuning" && i + 1 < argc && string(argv[i + 1]) == "1") {
            tuningMode = true;
            i++;
        } else if (string(argv[i]) == "-temperature" && i + 1 < argc) {
            temperature = stod(argv[i + 1]);
            i++;
        } else if (string(argv[i]) == "-cooling_rate" && i + 1 < argc) {
            cooling_rate = stod(argv[i + 1]);
            i++;
        }
    }

    // Procesar archivo de entrada
    int N, M, I; // N cadenas de longitud M
    tie(N, M, I) = parseFileInfo(inputFileName);
    vector<string> input_data = loadInputData(inputFileName);

    // Ajustar el threshold en función de la longitud M
    threshold *= M; // Convertir el umbral en función de M

    // Semilla aleatoria
    srand(I + 26999);
    // Llamada a la función de enfriamiento simulado
    cooling_system(metaheuristic_name, input_data, threshold, temperature, cooling_rate, tuningMode, max_time_seconds);

    return 0;
}