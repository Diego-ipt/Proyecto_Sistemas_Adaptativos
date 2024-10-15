#ifndef FUNCIONES_GREEDY_H
#define FUNCIONES_GREEDY_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <tuple>
#include <algorithm>
#include <cstdlib> // Para srand() y rand()
#include <ctime>   // Para time()

using namespace std;


//Función para generar substrings de tamaño 3
void generateSubstrings(unordered_map<string, int>& substring_to_index, unordered_map<int, string>& index_to_substring) {
    vector<char> symbols = {'A', 'T', 'C', 'G'};
    vector<string> substrings(64);

    // Generar la secuencia de Gray y convertirla en substrings
    for (int i = 0; i < 64; i++) {
        int gray_code = i ^ (i >> 1);  // Obtener el código de Gray para el índice i

        // Convertir el código de Gray en un substring de tamaño 3
        string s = "";
        s += symbols[(gray_code >> 4) & 0x03];
        s += symbols[(gray_code >> 2) & 0x03];
        s += symbols[gray_code & 0x03];

        substrings[i] = s;
    }

    // Llenar los mapas con los substrings generados
    for (int i = 0; i < 64; i++) {
        substring_to_index[substrings[i]] = i;
        index_to_substring[i] = substrings[i];
    }
}

// Función para buscar la posición de un substring en el mapa y modificar el índice
int findSubstringPosition(const unordered_map<string, int>& substring_to_index, const string& substring) {
    auto it = substring_to_index.find(substring);
    if (it != substring_to_index.end()) {
        int position = it->second;
        return position;
        //return (position + 42) % 64; // Modificar el índice haciendolo saltar a una distancia hamilton maxima y aplicar módulo 64
    } else {
        return -1; // Indicador de que el substring no se encontró
    }
}

//Función para buscar el substring dado una posición
string getSubstringByPosition(const unordered_map<int, string>& index_to_substring, int position) {
    auto it = index_to_substring.find(position);
    if (it != index_to_substring.end()) {
        return it->second;
    } else {
        return "No válido"; // Indicador de que la posición no es válida
    }
}


//Función para calcular la distancia hamming dados dos substrings
int hammingDistance(const string& s1, const string& s2) {
    if (s1.size() != s2.size()) {
        cout << "s1: " << s1 << " s2: " << s2 << endl; // Depuración
        throw invalid_argument("Los substrings deben tener la misma longitud.");
    }
    int distance = 0;
    for (int i = 0; i < s1.size(); ++i) {
        if (s1[i] != s2[i]) {
            ++distance;
        }
    }
    return distance;
}


//Función para calcular la distancia Hamming de forma aritmética entre 2 índices
int distancia_aritmetica(int s1, int s2) {
    int s1a = (s1 / 16);
    int s2a = (s2 / 16);
    int X = (((s1a % 2) + (s2a % 2)) == 1); // es 0 si s1a=1 xor s2a=1 
    int s1b = (s1%16)/4;
    int s2b = (s2%16)/4;
    int segundo = ((s1b + s2b * ( (2 * (X % 2))-1) - (X*3)) != 0);
    X = (((s1b % 2) + (s2b % 2)) == 1);
    return (((s1a - s2a) != 0) + segundo + ((((s1 % 16) % 4) + ((s2 % 16) % 4) * ((2 * (X % 2))-1) - (X*3)) != 0));
}



vector<int> compressString(const string& input, const unordered_map<string, int>& substring_to_index) {
    vector<int> compressed;
    int size = input.size();
    for (int i = 0; i+3 <= size; i += 3) {
        string substring = input.substr(i, 3);
        int position = findSubstringPosition(substring_to_index, substring);
        if (position != -1) {
            compressed.push_back(position);
        } else {
            cerr << "Error: Substring no encontrado en el mapa: " << substring << endl; // Depuración
            throw invalid_argument("Substring no encontrado en el mapa.");
        }
    }
    return compressed;
}


string greedyHeuristicFFMS(const vector<string>& dataset, int long_cadenas, const unordered_map<string,int>& substring_to_index, 
const unordered_map<int, string>& index_to_substring, int threshold, double alpha = 1.0) {

    // matriz de posiciones de los substrings
    vector<vector<int>> positions;

    // Dividir los strings en substrings y mapearlos a posiciones
    for(const auto& str: dataset){
        positions.push_back(compressString(str, substring_to_index));
    }

    //Función para calcular la media de las columnas
    int num_columns = positions[0].size();
    vector<int> column_means(num_columns, 0);
    for (int j = 0; j < num_columns; ++j) {
        // Generar numero aleatorio entre 0 y 1
        double random_value = static_cast<double>(rand()) / RAND_MAX;
        if(random_value < alpha) { // determinista si alpha es 1 
            int sum = 0;
            for (const auto& row : positions) {
                sum += row[j];
            }
            int mean = sum / positions.size();
            column_means[j] = (mean + 32) % 64;
        } else {
            column_means[j] = rand() % 64;
        }
}

    string solution_string;
    for(int pos: column_means){
        solution_string += getSubstringByPosition(index_to_substring, pos);
    }

    //caso de que 800%3 = 2
    if(long_cadenas % 3 == 1) {
        solution_string += "A";
    } else if(long_cadenas % 3 == 2) {
        solution_string += "AA";
    } else {
        printf("Error en la longitud de las cadenas\n");
    }

    return solution_string;
}


//Función para evaluar la calidad de la solución. Entrega la proporción de filas del dataset que cumplen con el threshold
double calidad_solucion(const vector<string>& dataset, int threshold, string solution_string) {
    // Evaluar la calidad de la solución
    int size = solution_string.size();
    double suma_filas;
    double calidad = 0;
    for(const auto& str : dataset){
        int total_distance = 0;
        for (int i = 0; i < size; i += 3) {
            string substring_solution = solution_string.substr(i, 3);
            string substring_str_dataset = str.substr(i, 3);
            total_distance += hammingDistance(substring_solution, substring_str_dataset);
        }
        if (total_distance >= threshold){
            suma_filas++;
        }
    }

    calidad = suma_filas / dataset.size();

    return calidad;
}

#endif