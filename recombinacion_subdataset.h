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
#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"
#include "brkgaAPI/Decoder.h"

using namespace std;

//inicializador de subdataset junto con sus respectivas solucion/es inicial
//usar divide_subdataset, luego por cada subdataset usar init_solucion_SuDa
vector<solutionss> init_subdatasets_solutions(const vector<string>& dataset, int num_subdatasets, int threshold,int max_error, 
double temperature_pert,double temperature_leap , 
double cooling_rate, double heat_rate, int iteraciones_max, 
unordered_map<string, int> substring_to_index,
unordered_map<int, string> index_to_substring){
    vector<Subdataset> subdataset = divide_subdataset(dataset, num_subdatasets);
    vector<solutionss> soluciones_subdatasets;
    for(int i = 0; i < num_subdatasets; i++){
        soluciones_subdatasets.push_back(init_solucion_SuDa(subdataset[i].dataset, threshold, max_error, temperature_pert, temperature_leap, cooling_rate, heat_rate, iteraciones_max, substring_to_index, index_to_substring));
    }
    return soluciones_subdatasets;
}

//recombinacion de soluciones de subdatasets
void recombinacion_subdataset(const vector<string>& dataset, 
int num_subdatasets, int threshold,int max_error, 
double temperature_pert,double temperature_leap , 
double cooling_rate, double heat_rate, int iteraciones_max, 
int time_max, unsigned p, double pe, double pm, double rhoe, 
unsigned X_INTVL, unsigned X_NUMBER, bool tuningMode){
    clock_t start_time = clock();
    int time = 0;
    //inicializar subdatasets
    unordered_map<string, int> substring_to_index;
    unordered_map<int, string> index_to_substring;
    generateSubstrings(substring_to_index, index_to_substring);
    vector<solutionss> soluciones_subdatasets = init_subdatasets_solutions(dataset, num_subdatasets, threshold, max_error, temperature_pert, temperature_leap, cooling_rate, heat_rate, iteraciones_max, substring_to_index, index_to_substring);
    // Ajustar el threshold en función de la longitud M
    const unsigned M = dataset[0].size();
    const unsigned n = M;		// size of chromosomes
	const unsigned K = 1;		// number of independent populations
	const unsigned MAXT = 1;	// number of threads for parallel decoding
    Decoder decoder;				// initialize the decoder
    const long unsigned rngSeed = 0;	// seed to the random number generator
	MTRand rng(rngSeed);				// initialize the random number generator
	// initialize the BRKGA-based heuristic
	BRKGA< Decoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, threshold, dataset, K, MAXT);
	vector<string> Sub_solutions_allinit;
	// Add the solutions of the subdatasets to the BRKGA
	for (const auto& solution : soluciones_subdatasets) {
        Sub_solutions_allinit.push_back(solution.solucion);        
	}
    algorithm.pushIndividuals_AGTC(Sub_solutions_allinit);


	double fitness_act=-1*algorithm.getBestFitness();
    string best_chromosome = decoder.traduccion(algorithm.getBestChromosome());
	unsigned generation = 0;		// current generation

        
    while ((clock() - start_time) / CLOCKS_PER_SEC < time_max){
        algorithm.evolve();	// evolve the population for one generation
            
        if(fitness_act < -1*algorithm.getBestFitness()) {
            fitness_act = -1*algorithm.getBestFitness();
            
            //calidad y tiempo en que la encontró
            if(!tuningMode) {
                time = (clock() - start_time) / CLOCKS_PER_SEC;
                cout << fitness_act << " " << time << endl;
            }
        }
    }
    best_chromosome = decoder.traduccion(algorithm.getBestChromosome());
    if(!tuningMode){
        time = (clock() - start_time) / CLOCKS_PER_SEC;
        cout << best_chromosome << endl;
        cout << fitness_act << " " << time << endl;
    }
    else{
        int irace_minmax = -fitness_act;
        cout << irace_minmax << endl;
    }
}



#endif