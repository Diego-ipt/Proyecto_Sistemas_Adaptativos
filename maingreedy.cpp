#include "loadinputdata.cpp"
#include "greedy.cpp"
#include <chrono>
#include <iostream>
#include <sstream>
#include <cstdlib>
using namespace std;

//info vector has the following structure: [N, M, I] 
//where N is the number of input strings, M is the length of each of the N input strings,
// and I+26999 is the random seed used to generate the instance.
std::tuple<int, int, int> parseFileInfo(const std::string& filename) {
    int N, M, I;
    char dash1, dash2;
    
    // Create a string stream from the filename and parse the integers
    std::stringstream ss(filename);
    ss >> N >> dash1 >> M >> dash2 >> I;
    
    return std::make_tuple(N, M, I);
}
int main(int argc, char* argv[]) {
    
    if (argc < 4 || string(argv[1]) != "-i" || string(argv[3]) != "-th") {
        cerr << "Usar: " << argv[0] << " -i <instancia-problema> -th threshold" << endl;
        return 1;
    }
    string inputFileName = argv[2];
    std::vector<std::string> input_data = loadInputData(inputFileName);

    int N, M, I;
    std::tie(N, M, I) = parseFileInfo(inputFileName);
    double threshold = 0.75*M; //puede ser 0.8 o 0.85 tambien 
    srand(I + 26999); //random seed 

    std::unordered_map<std::string, int> substring_to_index;
    std::unordered_map<int, std::string> index_to_substring;
    generateSubstrings(substring_to_index, index_to_substring);
    tuple<string, int> solution;
    solution = greedyHeuristicFFMS(input_data, substring_to_index, index_to_substring, threshold);

    cout << "Solucion: " << get<0>(solution) << endl;
    //print distance
    cout << "Distancia: " << get<1>(solution) << endl;
    return 0;
}