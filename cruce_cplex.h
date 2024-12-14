#ifndef CRUCE_CPLEX_H
#define CRUCE_CPLEX_H
#include <ilcplex/ilocplex.h>
#include <vector>
#include <map>
#include <string>
#include <random>
#include <iostream>
#include <algorithm>

// Define el componente solución como una asignación letra-posición
struct Component {
    int position;
    char letter;
};

using Solution = std::vector<Component>;

// Genera soluciones iniciales a partir de padres
Solution probabilisticSolutionConstruction(const std::vector<std::string>& parents, int length, double randomnessRate) {
    Solution child;
    std::map<char, int> letterFrequency;
    
    for (int j = 0; j < length; ++j) {
        letterFrequency.clear();
        for (const auto& parent : parents) {
            letterFrequency[parent[j]]++;
        }

        // Selección probabilística
        double randValue = static_cast<double>(rand()) / RAND_MAX;
        char selectedLetter;

        if (randValue <= randomnessRate) {
            // Selección determinística
            selectedLetter = std::max_element(letterFrequency.begin(), letterFrequency.end(),
                                              [](const auto& a, const auto& b) {
                                                  return a.second < b.second;
                                              })->first;
        } else {
            // Selección probabilística por frecuencia
            int total = 0;
            for (const auto& [letter, freq] : letterFrequency) {
                total += freq;
            }
            
            double threshold = static_cast<double>(rand()) / RAND_MAX * total;
            total = 0;
            for (const auto& [letter, freq] : letterFrequency) {
                total += freq;
                if (total >= threshold) {
                    selectedLetter = letter;
                    break;
                }
            }
        }

        child.push_back({j, selectedLetter});
    }
    return child;
}

// Resuelve la subinstancia con CPLEX
Solution solveSubInstance(const std::vector<Component>& subInstance, int length, int timeLimit) {
    IloEnv env;
    Solution optimalSolution;
    try {
        IloModel model(env);
        IloArray<IloBoolVarArray> x(env, length);
        for (int i = 0; i < length; ++i) {
            x[i] = IloBoolVarArray(env, 4); // Para A, C, T, G
        }

        // Restricciones de subinstancia
        for (const auto& comp : subInstance) {
            int letterIndex = comp.letter - 'A'; // Asumiendo letras como A, C, T, G
            model.add(x[comp.position][letterIndex] == 1);
        }

        // Función objetivo (ejemplo: maximizar distancia de Hamming)
        IloExpr obj(env);
        for (int i = 0; i < length; ++i) {
            for (int j = 0; j < 4; ++j) {
                obj += x[i][j];
            }
        }
        model.add(IloMaximize(env, obj));

        IloCplex cplex(model);
        cplex.setParam(IloCplex::TiLim, timeLimit);

        if (cplex.solve()) {
            for (int i = 0; i < length; ++i) {
                for (int j = 0; j < 4; ++j) {
                    if (cplex.getValue(x[i][j]) > 0.5) {
                        optimalSolution.push_back({i, static_cast<char>('A' + j)});
                        break;
                    }
                }
            }
        }
        obj.end();
    } catch (const IloException& e) {
        std::cerr << "CPLEX Exception: " << e.getMessage() << std::endl;
    }
    env.end();
    return optimalSolution;
}

// Función principal para realizar crossover usando CPLEX
std::vector<std::string> crossover_using_cplex(const std::vector<std::string>& parents, int threshold, const std::vector<std::string>& dataset) {
    int length = parents[0].size();
    double randomnessRate = 0.5;
    int timeLimit = 10;

    // Generar soluciones iniciales
    Solution child = probabilisticSolutionConstruction(parents, length, randomnessRate);

    // Resolver subinstancia con CPLEX
    Solution result = solveSubInstance(child, length, timeLimit);

    // Convertir la solución óptima en cadenas de salida
    std::vector<std::string> offspring(dataset.size(), std::string(length, ' '));
    for (const auto& comp : result) {
        for (auto& offspringStr : offspring) {
            offspringStr[comp.position] = comp.letter;
        }
    }

    return offspring;
}
#endif
