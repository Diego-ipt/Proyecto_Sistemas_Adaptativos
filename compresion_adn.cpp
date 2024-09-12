#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <tuple>
#include <algorithm>



// Función para generar todos los substrings de tamaño 3 con máxima distancia de Hamming entre substrings consecutivos
void generateSubstrings(std::unordered_map<std::string, int>& substring_to_index, std::unordered_map<int, std::string>& index_to_substring) {
    std::vector<char> symbols = {'a', 't', 'c', 'g'};
    std::vector<std::string> substrings(64);

    // Generar la secuencia de Gray y convertirla en substrings
    for (int i = 0; i < 64; i++) {
        int gray_code = i ^ (i >> 1);  // Obtener el código de Gray para el índice i

        // Convertir el código de Gray en un substring de tamaño 3
        std::string s = "";
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
int findSubstringPosition(const std::unordered_map<std::string, int>& substring_to_index, const std::string& substring) {
    auto it = substring_to_index.find(substring);
    if (it != substring_to_index.end()) {
        int position = it->second;
        return position;
        //return (position + 42) % 64; // Modificar el índice haciendolo saltar a una distancia hamilton maxima y aplicar módulo 64
    } else {
        return -1; // Indicador de que el substring no se encontró
    }
}

// Función para obtener el substring de la posición modificada
std::string getSubstringByPosition(const std::unordered_map<int, std::string>& index_to_substring, int position) {
    auto it = index_to_substring.find(position);
    if (it != index_to_substring.end()) {
        return it->second;
    } else {
        return "No válido"; // Indicador de que la posición no es válida
    }
}

// Función para obtener el substring del usuario
std::string getUserInput() {
    std::string input;
    std::cout << "Introduce el substring de tamaño 3 (compuesto por 'a', 't', 'c', 'g'): ";
    std::cin >> input;
    return input;
}

// Función para imprimir el contenido de las estructuras en orden de 0 a 64
void printSubstrings(const std::unordered_map<std::string, int>& substring_to_index, const std::unordered_map<int, std::string>& index_to_substring, std::ofstream& outFile) {
    outFile << "Contenido de substring_to_index:" << std::endl;

    // Imprimir en orden ascendente según los índices
    for (int i = 0; i < 64; ++i) {
        for (const auto& pair : substring_to_index) {
            if (pair.second == i) {
                outFile << "Substring: " << pair.first << " -> Índice: " << pair.second << std::endl;
                break;
            }
        }
    }

    outFile << "\nContenido de index_to_substring:" << std::endl;

    // Imprimir en orden ascendente según los índices
    for (int i = 0; i < 64; ++i) {
        auto it = index_to_substring.find(i);
        if (it != index_to_substring.end()) {
            outFile << "Índice: " << it->first << " -> Substring: " << it->second << std::endl;
        }
    }
}



// Función // Función para calcular la distancia de Hamming entre dos substrings
int hammingDistance(const std::string& s1, const std::string& s2) {
    if (s1.size() != s2.size()) {
        throw std::invalid_argument("Los substrings deben tener la misma longitud.");
    }
    int distance = 0;
    for (size_t i = 0; i < s1.size(); ++i) {
        if (s1[i] != s2[i]) {
            ++distance;
        }
    }
    return distance;
}


// Función principal
int main() {
    std::unordered_map<std::string, int> substring_to_index;
    std::unordered_map<int, std::string> index_to_substring;

    // Generar todos los substrings de tamaño 3 y almacenarlos en los mapas
    generateSubstrings(substring_to_index, index_to_substring);

    // Obtener el substring del usuario
    std::string input = getUserInput();

    //Buscar el substring y mostrar su posición modificada y el substring correspondiente
    int position = findSubstringPosition(substring_to_index, input);
    if (input.size() == 3 && position != -1) {
        std::cout << "La posicióndel substring '" << input << "' es: " << position << std::endl;
        std::string result_substring = getSubstringByPosition(index_to_substring, (position + 42) % 64);
        std::cout << "El substring en la posición modificada es: " << result_substring <<", con posicion " <<(position + 42) % 64<< std::endl;
    } else {
        std::cout << "El substring ingresado no es válido o no existe." << std::endl;
    }

    return 0;
}