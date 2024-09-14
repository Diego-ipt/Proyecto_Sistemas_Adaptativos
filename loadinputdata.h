#ifndef LOADINPUTDATA_H
#define LOADINPUTDATA_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <regex>
using namespace std;

vector<string> loadInputData(const string& filename) {
    vector<string> input_data;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        input_data.push_back(line);
    }

    file.close();

    return input_data;
}

//info vector has the following structure: [N, M, I] 
//where N is the number of input strings, M is the length of each of the N input strings,
// and I+26999 is the random seed used to generate the instance.
tuple<int, int, int> parseFileInfo(const string& filename) {
    int N, M, I;
    
    // Define a regex pattern to match the filename format N-M-I.txt
    std::regex pattern(R"((\d+)-(\d+)-(\d+)\.txt)");
    std::smatch matches;
    
    // Apply the regex pattern to the filename
    if (std::regex_search(filename, matches, pattern) && matches.size() == 4) {
        N = std::stoi(matches[1].str());
        M = std::stoi(matches[2].str());
        I = std::stoi(matches[3].str());
    } else {
        throw std::invalid_argument("Formate de archivo dataset no valido");
    }
    return make_tuple(N, M, I);
}
#endif