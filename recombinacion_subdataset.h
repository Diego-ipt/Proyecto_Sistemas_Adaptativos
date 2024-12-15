#ifndef RECOMBINACION_SUBDATASET_H
#define RECOMBINACION_SUBDATASET_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "subdataset_functions.h"

using namespace std;

//inicializador de subdataset junto con sus respectivas solucion/es inicial
//usar divide_subdataset, luego por cada subdataset usar init_solucion_SuDa
vector<Subdataset> init_subdatasets(const vector<string>& dataset, int num_subdatasets, int threshold,int max_error, 
double temperature_pert,double temperature_leap , 
double cooling_rate, double heat_rate, int iteraciones_max, 
unordered_map<string, int> substring_to_index,
unordered_map<int, string> index_to_substring){
    vector<Subdataset> subdataset;
    subdataset = divide_subdataset(dataset, num_subdatasets);
    for(int i = 0; i < num_subdatasets; i++){
        subdataset[i].soluciones.push_back(init_solucion_SuDa(subdataset[i].dataset, threshold, max_error, temperature_pert, temperature_leap, cooling_rate, heat_rate, iteraciones_max, substring_to_index, index_to_substring));
    }
    return subdataset;
}

//recombinacion de soluciones de subdatasets (ingreso de soluciones..
//con la union de los subdatasets correspondientes..
//para ir uniendo los subdasets de a poco hasta lograr el dataset completo)
//       ;se debe usar brkga;
//extraer soluciones de subdatasets->unir subdatasets(con solutionss vacio)
//->recombinar las soluciones extraidas con brkga
//->agregar las soluciones al subdataset unido->repetir


#endif