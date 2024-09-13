#include "loadinputdata.h"
#include <fstream>
#include <iostream>

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