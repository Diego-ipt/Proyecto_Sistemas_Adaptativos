#include <iostream>
#include <algorithm>
#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"
#include "brkgaAPI/Decoder.h"
#include "loadinputdata.h"
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    // Verificar que se han proporcionado todos los argumentos
    if (argc < 7 || string(argv[2]) != "-i" || string(argv[4]) != "-t" || string(argv[6]) != "-th") {
        cerr << "Uso: " << argv[0] << " <nombremetaheuristica> -i <instancia-problema> -t <tiempo-max-segundos> -th <threshold>" << endl;
        return 1;
    }

    string metaheuristic_name = argv[1];          // Nombre de la metaheurística
    string inputFileName = argv[3];               // Nombre de la instancia del problema
    int max_time_seconds = stoi(argv[5]);         // Tiempo máximo en segundos
    double threshold = stod(argv[7]);             // Umbral (threshold)

    // Procesar archivo de entrada
    int N, M, I; // N cadenas de longitud M
    tie(N, M, I) = parseFileInfo(inputFileName);
    vector<string> input_data = loadInputData(inputFileName);

    // Ajustar el threshold en función de la longitud M
    threshold *= M; // Convertir el umbral en función de M

    // Semilla aleatoria
    srand(I + 26999);

	const unsigned n = M;		// size of chromosomes
	const unsigned p = 100;		// size of population -- puede variar
	const double pe = 0.10;		// fraction of population to be the elite-set -- puede variar
	const double pm = 0.10;		// fraction of population to be replaced by mutants -- puede variar
	const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent --puede variar
	const unsigned K = 1;		// number of independent populations
	const unsigned MAXT = 1;	// number of threads for parallel decoding
	
	Decoder decoder;				// initialize the decoder
	
	const long unsigned rngSeed = 0;	// seed to the random number generator
	MTRand rng(rngSeed);				// initialize the random number generator
	
	// initialize the BRKGA-based heuristic
	BRKGA< Decoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, threshold, input_data, K, MAXT);
	
	unsigned generation = 0;		// current generation
	const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
	const unsigned X_NUMBER = 2;	// exchange top 2 best
	const unsigned MAX_GENS = 1000;	// run for 1000 gens
    clock_t start_time = clock();

	int fitness_act = 0;
	string best_chromosome = "";
	int time = 0;
	
	do {
		algorithm.evolve();	// evolve the population for one generation
		
		if(fitness_act < trunc(-1*algorithm.getBestFitness())) {
			fitness_act = trunc(-1*algorithm.getBestFitness());
			time = (clock() - start_time) / CLOCKS_PER_SEC;
			//calidad y tiempo en que la encontró
			cout << fitness_act << " " << time << endl;
		}

		if((++generation) % X_INTVL == 0) {
			algorithm.exchangeElite(X_NUMBER);	// exchange top individuals
		}
	} while ((clock() - start_time) / CLOCKS_PER_SEC < max_time_seconds);
	
	best_chromosome = decoder.traduccion(algorithm.getBestChromosome());

	cout << best_chromosome << endl;
	//se repite nuevamente la calidad y el tiempo en que fue encontrada
	cout << fitness_act << " " << time << endl;

	return 0;
}