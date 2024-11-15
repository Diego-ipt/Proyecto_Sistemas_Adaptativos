#include <iostream>
#include <algorithm>
#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"
#include "brkgaAPI/Decoder.h"
#include "loadinputdata.h"
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

	//valores predeterminados. Hay que aplicar tuning 
	unsigned p = 100;		// size of population -- puede variar
	double pe = 0.10;		// fraction of population to be the elite-set -- puede variar
	double pm = 0.10;		// fraction of population to be replaced by mutants -- puede variar
	double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent --puede variar
	unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations --puede variar
	unsigned X_NUMBER = 2;	// exchange top 2 best --puede variar
	int max_time_seconds = 10;	


    // Verificar que se han proporcionado los argumentos mínimos obligatorios
    if (argc < 5 || string(argv[2]) != "-i" || string(argv[4]) != "-th") {
        cerr << "Uso: " << argv[0] << " <nombremetaheuristica> -i <instancia-problema> -th <threshold> [opciones]" << endl;
        cerr << "Opciones:\n"
			 << "  -t <tiempo-max-segundos>\n"
             << "  -p <size of population>\n"
             << "  -pe <elite-set fraction>\n"
             << "  -pm <mutant fraction>\n"
             << "  -rhoe <elite allele inheritance prob>\n"
             << "  -X_INTVL <exchange best individuals every X gens>\n"
             << "  -X_NUMBER <number of top to exchange>" << endl;
        return 1;
    }

    // Parámetros obligatorios
    string metaheuristic_name = argv[1];
    string inputFileName = argv[3];
    double threshold = stod(argv[5]);

    // Procesar argumentos opcionales
    for (int i = 6; i < argc; i += 2) {
        string opt = argv[i];
        if (opt == "-t") max_time_seconds = stoi(argv[i + 1]);
        else if (opt == "-p") p = stoi(argv[i + 1]);
        else if (opt == "-pe") pe = stod(argv[i + 1]);
        else if (opt == "-pm") pm = stod(argv[i + 1]);
        else if (opt == "-rhoe") rhoe = stod(argv[i + 1]);
        else if (opt == "-X_INTVL") X_INTVL = stoi(argv[i + 1]);
        else if (opt == "-X_NUMBER") X_NUMBER = stoi(argv[i + 1]);
    }

    // Procesar archivo de entrada
    int N, M, I; // N cadenas de longitud M
    tie(N, M, I) = parseFileInfo(inputFileName);
    vector<string> input_data = loadInputData(inputFileName);

    // Ajustar el threshold en función de la longitud M
    threshold *= M; // Convertir el umbral en función de M

    // Semilla aleatoria
    srand(I + 26999);

	const unsigned n = M;		// size of chromosomes
	const unsigned K = 1;		// number of independent populations
	const unsigned MAXT = 1;	// number of threads for parallel decoding
	
	Decoder decoder;				// initialize the decoder
	
	const long unsigned rngSeed = 0;	// seed to the random number generator
	MTRand rng(rngSeed);				// initialize the random number generator
	
	// initialize the BRKGA-based heuristic
	BRKGA< Decoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, threshold, input_data, K, MAXT);
	
	unsigned generation = 0;		// current generation
	
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