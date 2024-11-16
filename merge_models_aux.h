#ifndef MERGE_MODELS_AUX_H
#define MERGE_MODELS_AUX_H

#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include "metaheuristic_functions.h"
#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"

using namespace std;

//Funcion para aceptar o rechazar una solucion en el simulated annealing
bool accept_rate_plus(double best_quality, double neighbor_quality, double temperature_porcentual, int dataset_size) { 
    // Calcula la diferencia en calidad (Delta E)
    double delta_quality = neighbor_quality - best_quality;

    // Si la solución candidata es mejor, se acepta directamente
    if (delta_quality < 0) {
        return true;
    }

    // Calcula la probabilidad de aceptación
    double acceptance_probability = exp(-delta_quality / (temperature_porcentual * dataset_size));

    // Genera un número aleatorio en el rango [0, 1]
    double random_value = static_cast<double>(rand()) / RAND_MAX;

    // Acepta la solución si el número aleatorio es menor o igual a la probabilidad de aceptación
    return random_value <= acceptance_probability;
}


//Función para generar soluciones vecinas
vector<string> generateNeighborSolution_plus(const string& current_solution,unordered_map<string, int> substring_to_index, unordered_map<int, string> index_to_substring, double temperature){
    vector<string> neighbor_solutions;
    vector<int> solution = compressString(current_solution, substring_to_index);
    int num_columns = solution.size();

    // Generar soluciones vecinas
    for (int j = 1; j < 8; ++j) {
        vector<int> new_solution = solution;
        for (int i = 0; i < num_columns; ++i) {
            new_solution[i] = (solution[j % num_columns] + static_cast<int>(j * 8));
            
            //perturbacion 
            int random_position = (rand() % 8)*temperature;
            if (rand() % 2) {
                new_solution[i] += random_position;
            }else{
                new_solution[i] -= random_position;
                new_solution[i] = abs(new_solution[i]);
            }
            new_solution[i] %= 64;
        }
        // Convert new_solution to string before pushing
        string new_solution_str;
        for (int pos : new_solution) {
            new_solution_str += getSubstringByPosition(index_to_substring, pos);
        }
        neighbor_solutions.push_back(new_solution_str);
    }

    return neighbor_solutions;
}

//Funcion (mutacion direccionada por temperatura)
// variables de tunning: max_error, temperature_pert, temperature_leap, cooling_rate, heat_rate
pair<string, double> cooling_system_plus(const vector<string>& dataset,  int threshold,int max_error, double temperature_pert,double temperature_leap , double cooling_rate, double heat_rate, bool tuningMode, int max_time_seconds) {
    unordered_map<string, int> substring_to_index;
    unordered_map<int, string> index_to_substring;
    generateSubstrings(substring_to_index, index_to_substring);
    // Initialize solution
    string current_solution = generateInitialSolution(dataset, threshold, substring_to_index, index_to_substring);
    string best_solution = current_solution;
    double best_quality = calidad_solucion(dataset, threshold, best_solution);
    int dataset_size= dataset.size();
    int best_solution_size = best_solution.size();
    
    clock_t start_time = clock();
    int time;

    double temperature_pert_max=temperature_pert;
    double temperature_leap_max=temperature_leap;


    // Simulated Annealing loop
    int part_size;
    int random_position;
    string sub_solution;
    vector<string> neighbor_solutions;
    string new_solution;
    double elapsed_time;
    int iterations_without_improvement = 0;
    string solution_random;
    double neighbor_solution_quality_in;
    while ((clock() - start_time) / CLOCKS_PER_SEC < max_time_seconds) {
        // Calculate the size of the parts to replace
        part_size = size_calculator(temperature_pert/temperature_pert_max, best_solution_size); // Ensure part_size is a multiple of 3
        random_position = rand() % (best_solution_size - part_size + 1);
        // Replace the parts with new random substrings
        // Extract the substring
        sub_solution = current_solution.substr(random_position, part_size);

        // Generate neighbor solutions for the substring
        neighbor_solutions = generateNeighborSolution_plus(sub_solution, substring_to_index, index_to_substring, temperature_pert/temperature_pert_max);

        for (const string& neighbor_solution : neighbor_solutions) {
            // Replace the original substring with the neighbor solution
            new_solution = current_solution;
            new_solution.replace(random_position, part_size, neighbor_solution);

            double neighbor_quality = calidad_solucion(dataset, threshold, new_solution);
            if (neighbor_quality > best_quality) {
                int aux_best = trunc(best_quality);
                best_solution = new_solution;
                best_quality = neighbor_quality;
                time=(clock() - start_time) / CLOCKS_PER_SEC;
                if(tuningMode==false && (trunc(best_quality) > aux_best)){
                    cout << (trunc(best_quality)) << " "<< time << endl;
                }
                current_solution = new_solution;
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
                temperature_leap_max=temperature_leap_max*heat_rate;
                // Calculate the size of the parts to replace
                part_size = size_calculator(temperature_leap/temperature_leap_max, best_solution_size);
                random_position = rand() % (best_solution_size - part_size + 1);

                // Generate neighbor solutions for the substring
                solution_random = generateNeighborSolutionRandom(part_size, index_to_substring);

                new_solution = current_solution;
                new_solution.replace(random_position, part_size, solution_random);
                neighbor_solution_quality_in = calidad_solucion(dataset, threshold, new_solution);
                current_solution = new_solution;
        
                if (accept_rate_plus(best_quality, neighbor_solution_quality_in, temperature_leap/temperature_leap_max, dataset_size)) {
                    //cout<<".";//ayuda visual de probabilidad
                    current_solution = new_solution;
                    break;
                }

            }
        }

        // Incrementar el cooling rate proporcionalmente al tiempo transcurrido
        elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
        cooling_rate = 0.99 + (0.01 * (elapsed_time / max_time_seconds));
        //printf("%f\n", temperature);
    }
    if(tuningMode){
        double tuning_quality =trunc(best_quality)*-1;
        cout << tuning_quality<< endl;
    }
    else{
        cout << best_solution << endl;
        cout << (trunc(best_quality)) << " "<< time;                
    }
    return make_pair(best_solution, best_quality);
}

//Funcion de cruzamiento
/*usar cmsa y cplex*/

//Poblaciones restringidas
class DecoderATCG {
public:
    // La seleccion debe ser entre 1 y 6
    DecoderATCG(int seleccion) : seleccion(seleccion) {}
    ~DecoderATCG(){};

private:
    int seleccion;

    string traduccion(const vector<double>& chromosome) const {

        vector<char> alphabet = {'X', 'Y'};
        string solucion = "";
        int size = chromosome.size();

        if(seleccion <= 3) {
            if(seleccion == 1) alphabet = {'A', 'T'};
            else if(seleccion == 2) alphabet = {'A', 'C'};
            else alphabet = {'A', 'G'};
        } else {
            if(seleccion == 4) alphabet = {'T', 'G'};
            else if(seleccion == 5) alphabet = {'T', 'C'};
            else alphabet = {'C', 'G'};
        }

        //traduccion a caracteres
        for(unsigned i = 0; i < size; i++) {
            if(chromosome[i] < 0.5) {
                solucion += alphabet[0];
            } else{
                solucion += alphabet[1];
            }
        }
        return solucion;
    }

    // Decode a chromosome, returning its fitness as a double-precision floating point:
    double decode(const std::vector< double >& chromosome, const double treshold, const std::vector<std::string>& dataset) const{
        double myFitness = 0.0;

        std::string solucion = traduccion(chromosome);

        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);

        myFitness = calidad_solucion(dataset, treshold, solucion);

        myFitness = myFitness * -1;

        // Return the fitness:
        return myFitness;
    }
};

#endif

