#ifndef CRUCE_CPLEX_H
#define CRUCE_CPLEX_H
//#include <ilcplex/ilocplex.h>
#include <vector>
#include <map>
#include <string>
#include <random>
#include <iostream>
#include <algorithm>

using namespace std;
//calidad particionada
//esta funcion debe usar substring de tamano minimo de 2, aunque se recomiendan tamanos grandes
int calidad_particion(const vector<string>& dataset, const string& substring, int position_start) {
    double suma_filas_reject=0;
    // Evaluar la calidad de la solución
    double size = substring.size();
    double suma_filas;
    double calidad = 0;
    double cantidad_reject = 0;
    double data_size = dataset.size();
    for(const auto& str : dataset){
        int total_distance = 0;
        for (int i = 0; i < size; ++i) {
            char char_solution = substring[i];
            char char_str_dataset = str[i+position_start];
            if (char_solution != char_str_dataset) {
                total_distance++;
            }
        }
        if (total_distance >= size-1){
            suma_filas++;//parte real de la calidad
        } else {
            cantidad_reject++;
            suma_filas_reject += total_distance/size;
        }
    }
    if(cantidad_reject==0){
        cantidad_reject=1;
    }
    double porcentaje_hamilton_reject = suma_filas_reject/cantidad_reject;
    calidad = suma_filas+porcentaje_hamilton_reject;

    return calidad;
}
// Función para resolver con CPLEX
/* std::string crossover_using_cplex(const std::vector<std::string>& padres, const std::vector<std::string>& dataset, int threshold) {
    size_t longitud = padres[0].size();
    size_t num_padres = padres.size();

    IloEnv env;
    try {
        IloModel model(env);
        IloArray<IloNumVarArray> x(env, longitud); // Variables binarias para elegir letras

        // Crear variables
        for (size_t i = 0; i < longitud; ++i) {
            x[i] = IloNumVarArray(env, num_padres, 0, 1, ILOBOOL);
        }

        // Restricción: Cada posición debe tener exactamente una letra elegida
        for (size_t i = 0; i < longitud; ++i) {
            IloExpr sum(env);
            for (size_t j = 0; j < num_padres; ++j) {
                sum += x[i][j];
            }
            model.add(sum == 1);
            sum.end();
        }

        // Función objetivo: Maximizar la calidad del hijo
        IloExpr objective(env);
        int size_substring = 8;
        for (size_t i = 0; i < longitud; i += size_substring) {
            for (size_t j = 0; j < num_padres; ++j) {
                std::string substring = padres[j].substr(i, size_substring);
                objective += calidad_particion(dataset, substring, i) * x[i][j];
            }
        }
        model.add(IloMaximize(env, objective));
        objective.end();

        // Resolver el modelo
        IloCplex cplex(model);
        cplex.solve();

        // Construir la solución
        std::string hijo(longitud, ' ');
        for (size_t i = 0; i < longitud; ++i) {
            double max_value = -1.0;
            size_t best_j = 0;
            for (size_t j = 0; j < num_padres; ++j) {
            double value = cplex.getValue(x[i][j]);
            if (value > max_value) {
                max_value = value;
                best_j = j;
            }
            }
            hijo[i] = padres[best_j][i];
        }

        return hijo;
    } catch (IloException& e) {
        std::cerr << "Error de CPLEX: " << e.getMessage() << std::endl;
        return "";
    } catch (...) {
        std::cerr << "Error desconocido." << std::endl;
        return "";
    }
    env.end();
}
 */


string crossover_subdivision(const vector<string>& padres, const vector<string>& dataset, int num_particiones) {
    string solucion_final = "";

    // Calcular el tamaño de cada partición
    int tam_substring = padres[0].size() / num_particiones;
    int resto = padres[0].size() % num_particiones;

    for (int particion = 0; particion < num_particiones; ++particion) {
        string mejor_subdivision;
        int mejor_calidad = -1;

        int pos_inicio = particion * tam_substring;

        // Evaluar cada padre para esta partición
        for (const auto& padre : padres) {
            string subdivision = padre.substr(pos_inicio, tam_substring);

            // Evaluar calidad de la partición actual
            int calidad = calidad_particion(dataset, subdivision, pos_inicio);

            // Actualizar la mejor subdivisión si la calidad es superior
            if (calidad > mejor_calidad) {
                mejor_calidad = calidad;
                mejor_subdivision = subdivision;
            }
        }

        // Agregar la mejor subdivisión a la solución final
        solucion_final += mejor_subdivision;
    }

    if (resto > 0) {
        string mejor_subdivision;
        int mejor_calidad = -1;
        int pos_inicio = num_particiones * tam_substring;

        for (const auto& padre : padres) {
            string substring = padre.substr(pos_inicio, resto);

            int calidad = calidad_particion(dataset, substring, pos_inicio);

            if (calidad > mejor_calidad) {
                mejor_calidad = calidad;
                mejor_subdivision = substring;
            }
        }

        solucion_final += mejor_subdivision;
    }

    return solucion_final;
}

#endif