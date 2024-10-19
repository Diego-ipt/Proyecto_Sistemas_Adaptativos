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
#include "funciones_metaheuristic.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Verificar que se han proporcionado todos los argumentos
    if (argc < 7 || string(argv[2]) != "-i" || string(argv[4]) != "-t" || string(argv[6]) != "-th") {
        cerr << "Uso: " << argv[0] << " <nombremetaheuristica> -i <instancia-problema> -t <tiempo-max-segundos> -th <threshold>" << endl;
        return 1;
    }

    string metaheuristic_name = argv[1];          // Nombre de la metaheurística
    string inputFileName = argv[3];               // Nombre de la instancia del problema
    int max_time_seconds = stoi(argv[5]);         // Tiempo máximo en segundos
    double threshold = stod(argv[7]);             // Umbral (threshold)

    // Procesar archivo de entrada
    int N, M, I; // N cadenas de longitud M
    tie(N, M, I) = parseFileInfo(inputFileName);
    vector<string> input_data = loadInputData(inputFileName);

    // Ajustar el threshold en función de la longitud M
    threshold *= M; // Convertir el umbral en función de M

    // Semilla aleatoria
    srand(I + 26999);
    // Llamada a la función de enfriamiento simulado
    cooling_system(metaheuristic_name, input_data, max_time_seconds, threshold);

    return 0;
}
