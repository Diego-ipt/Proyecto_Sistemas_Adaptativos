#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <tuple>
#include <algorithm>
#include <cstdlib> // Para srand() y rand()
#include <ctime>   // Para time()


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
// int distancia_aritmetica(int s1, int s2) {
//     int s1a = int(s1 / 16);
//     int s2a = int(s2 / 16);
//     int primer = ((s1a - s2a) != 0);

//     printf("s1a: %d, s2a: %d, primer: %d\n", s1a, s2a, primer);

//     // par o impar?
//     int s1P = (s1a % 2); // si es 0 es par (0 o 2)
//     int s2P = (s2a % 2); // si es 0 es par (0 o 2)
//     int X = ((s1P + s2P) == 1); // es 0 si s1a=1 xor s2a=1 

//     printf("s1P: %d, s2P: %d, X: %d\n", s1P, s2P, X);

//     int s1b = (s1%16)/4;
//     int s2b = (s2%16)/4;
//     int in = (s1b + s2b * ( (2 * (X % 2))-1) - (X*3));
//     int segundo = (in != 0);

//     printf("s1b: %d, s2b: %d, segundo: %d, in second %d \n", s1b, s2b, segundo, in);

//     int s1c = (s1 % 16) % 4;
//     int s2c = (s2 % 16) % 4;
//     s1P = (s1b % 2); // si es 0 es par (0 o 2)
//     s2P = (s2b % 2); // si es 0 es par (0 o 2)
//     X = ((s1P + s2P) == 1);
//     printf("s1P: %d, s2P: %d, X: %d, \n", s1P, s2P, X);

//     printf("s1c: %d, s2c: %d, ", s1c, s2c);
//     in = (s1c + s2c * ((2 * (X % 2))-1) - (X*3));

//     int tercero = (in != 0);

//     printf("tercero: %d, in third %d\n", tercero,in);

//     printf("Resultados: primer = %d, segundo = %d, tercero = %d\n", primer, segundo, tercero);

//     return (primer + segundo + tercero);
// }


// Generar un substring aleatorio de tamaño 3
std::string generateRandomSubstring() {
    std::string bases = "atcg";
    std::string result;
    for (int i = 0; i < 3; ++i) {
        result += bases[rand() % 4];
    }
    return result;
}

// Función principal
int main() {
    srand(time(0)); // Inicializar la semilla para la generación de números aleatorios

    std::unordered_map<std::string, int> substring_to_index;
    std::unordered_map<int, std::string> index_to_substring;

    // Generar todos los substrings de tamaño 3 y almacenarlos en los mapas
    generateSubstrings(substring_to_index, index_to_substring);

    // Realizar múltiples pruebas aleatorias para comparar distancia aritmética y distancia de Hamming
    for (int i = 0; i < 1000; ++i) { // Por ejemplo, 1000 pruebas
        std::string s1 = generateRandomSubstring();
        std::string s2 = generateRandomSubstring();
        std::cout <<s1<<" y "<<s2<<"\n";

        int hamming_dist = hammingDistance(s1, s2);
        int position1 = findSubstringPosition(substring_to_index,s1);
        int position2 = findSubstringPosition(substring_to_index,s2);
        int aritmetica_dist = distancia_aritmetica(position1, position2);

        if (hamming_dist != aritmetica_dist) {
            std::cout << "Diferencia encontrada en iteracion " << i + 1 << ":\n";
            std::cout << "Substring 1: " << s1 << " | Substring 2: " << s2 << "\n";
            std::cout << "Distancia de Hamming: " << hamming_dist << " | Distancia Aritmetica: " << aritmetica_dist << "\n";
            return 1; // Terminar el programa si hay una diferencia
        }
    }

    std::cout << "Todas las pruebas pasaron, las distancias son equivalentes.\n";
    return 0;
}