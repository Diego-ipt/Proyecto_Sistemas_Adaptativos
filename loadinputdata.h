#ifndef LOADINPUTDATA_H
#define LOADINPUTDATA_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

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
    char dash1, dash2;
    
    // Create a string stream from the filename and parse the integers
    stringstream ss(filename);
    ss >> N >> dash1 >> M >> dash2 >> I;

    //cout << "N: " << N << " M: " << M << " I: " << I << endl; 
    
    return make_tuple(N, M, I);
}

#endif