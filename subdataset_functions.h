#ifndef SUBDATASET_FUNCTIONS_H
#define SUBDATASET_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "merge_models_aux.h"
#include "loadinputdata.h"
#include "funciones_greedy.h"

using namespace std;

//guarda la solucion y su calidad
struct solutionss{
    string solucion;
    double calidad;
};

//guarda el subdataset y sus soluciones
struct Subdataset{
    //vector<int> identificadores_subdatasets_contenidos;
    vector<string> dataset;
    //vector<solutionss> soluciones;
};

//sudividir el dataset en subdatasets
vector<Subdataset> divide_subdataset(const vector<string>& dataset, int num_subdatasets){
    vector<Subdataset> subdatasets(num_subdatasets);
    int size = dataset.size();
    int subdataset_size = size/num_subdatasets;
    for(int i = 0; i < num_subdatasets; i++){
        int start = i*subdataset_size;
        int end = (i+1)*subdataset_size;
        if(i == num_subdatasets-1){
            end = size;
        }
        subdatasets[i].dataset = vector<string>(dataset.begin()+start, dataset.begin()+end);
    }
    // Assign an ID to each subdataset
    // for (int i = 0; i < num_subdatasets; i++) {
    //     subdatasets[i].identificadores_subdatasets_contenidos.push_back(i);
    // }
    return subdatasets;
}


//union subdataset
//se puede usar en cualquier momento
vector<Subdataset> union_subdataset(const vector<Subdataset>& subdatasets){
    vector<Subdataset> subdatasets_union;
    Subdataset subdataset_union;
    for(const Subdataset& subdataset : subdatasets){
        subdataset_union.dataset.insert(subdataset_union.dataset.end(), subdataset.dataset.begin(), subdataset.dataset.end());
        //subdataset_union.identificadores_subdatasets_contenidos.insert(subdataset_union.identificadores_subdatasets_contenidos.end(), subdataset.identificadores_subdatasets_contenidos.begin(), subdataset.identificadores_subdatasets_contenidos.end());
    }
    subdatasets_union.push_back(subdataset_union);
    return subdatasets_union;
}


//calculador de solucion inicial para un subdataset
solutionss init_solucion_SuDa(const vector<string>& dataset,int threshold,int max_error, 
double temperature_pert,double temperature_leap , 
double cooling_rate, double heat_rate, int iteraciones_max, 
unordered_map<string, int> substring_to_index,
unordered_map<int, string> index_to_substring){

    string best_solution=generateInitialSolution(dataset, threshold, substring_to_index, index_to_substring);
    double best_quality=calidad_solucion(dataset, threshold, best_solution);
    int dataset_size= dataset.size();
    int best_solution_size = best_solution.size();
    double temperature_pert_max=1000;
    double temperature_leap_max=1000;

    string current_solution = best_solution;
    double current_quality = best_quality;


    // Simulated Annealing loop variables
    int part_size;
    int random_position;
    string sub_solution;
    vector<string> neighbor_solutions;
    string new_solution;
    int iterations_without_improvement = 0;
    string solution_random;
    double neighbor_solution_quality_in;
    int iteracion_actual=0;

    while (iteraciones_max>iteracion_actual || dataset_size>best_quality) {
        // Calculate the size of the parts to replace
        part_size = size_calculator_plus(temperature_pert/temperature_pert_max, best_solution_size); // Ensure part_size is a multiple of 3
        random_position = rand() % (best_solution_size - part_size + 1);
        // Replace the parts with new random substrings


        // Extract the substring
        sub_solution = current_solution.substr(random_position, part_size);

        // Generate neighbor solutions for the substring
        neighbor_solutions = generateNeighborSolution_plus(sub_solution, substring_to_index, 
        index_to_substring, temperature_pert/temperature_pert_max);


        // Replace the original substring with the neighbor solution
        new_solution = current_solution;
        for (const string& neighbor_solution : neighbor_solutions) {

            new_solution.replace(random_position, part_size, neighbor_solution);

            double neighbor_quality = calidad_solucion(dataset, threshold, new_solution);
            if (neighbor_quality > best_quality) {
                best_solution = new_solution;
                best_quality = neighbor_quality;
                best_solution = new_solution;
                current_solution = new_solution;
                current_quality = neighbor_quality;
                iterations_without_improvement=0; //reset si se mejoro
            }else {
                iterations_without_improvement++;  // Aumentar si no hay mejora
            }
        }  

        temperature_pert *= cooling_rate;
        // Si no hay mejoras en iteraciones, ajustar el cooling_rate
        if (iterations_without_improvement >= max_error) {
            temperature_leap=temperature_leap_max; //reset search temperature
            iterations_without_improvement = 0; // Reiniciar el contador
            while (true) {
                temperature_leap=temperature_leap*heat_rate;
                // Calculate the size of the parts to replace
                part_size = size_calculator_plus(temperature_leap/temperature_leap_max, best_solution_size);
                random_position = rand() % (best_solution_size - part_size + 1);

                // Generate neighbor solutions for the substring
                solution_random = generateNeighborSolutionRandom(part_size, index_to_substring);

                new_solution = best_solution;
                new_solution.replace(random_position, part_size, solution_random);
                neighbor_solution_quality_in = calidad_solucion(dataset, threshold, new_solution);
        
                if (accept_rate_plus(best_quality, neighbor_solution_quality_in, temperature_leap/temperature_leap_max, dataset_size)) {
                    //cout<<".";//ayuda visual de probabilidad
                    current_solution = new_solution;
                    current_quality  = neighbor_solution_quality_in; 
                    break;
                }

            }
        }
        iteracion_actual++;
    }
    return {best_solution, best_quality};
}




#endif