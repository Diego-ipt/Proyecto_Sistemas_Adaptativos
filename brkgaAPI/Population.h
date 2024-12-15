/**
 * Population.h
 *
 * Encapsulates a population of chromosomes represented by a vector of doubles. We don't decode
 * nor deal with random numbers here; instead, we provide private support methods to set the
 * fitness of a specific chromosome as well as access methods to each allele. Note that the BRKGA
 * class must have access to such methods, thus begin a friend. In terms of design, this class is
 * obviously tightly coupled with BRKGA, and was implemented just to remove unnecessary complexity
 * from the design of BRKGA.
 *
 * All public methods in this API *require* the fitness array to be sorted, and thus a call to
 * sortFitness() beforehand. Since this class is tightly coupled with BRKGA, rest assured: 
 * everything will work just fine with a Population obtained from BRKGA.
 *
 * Created on : Jun 21, 2010 by rtoso
 * Last update: Nov 15, 2010 by rtoso
 * Authors    : Rodrigo Franco Toso <rtoso@cs.rutgers.edu>
 *              Mauricio G.C. Resende <mgcr@research.att.com>
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018
 * Rodrigo Franco Toso (rfrancotoso@gmail.com) and
 * Mauricio G.C. Resende
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <algorithm>
#include <exception>
#include <stdexcept>

class Population {
	template< class Decoder, class MTRand >
	friend class BRKGA;

public:
	unsigned getN() const {
		return population[0].size();
	}	// Size of each chromosome

	unsigned getP() const {
		return population.size();
	}	// Size of population

	// Returns the best fitness in this population:
	double getBestFitness() const {
		return getFitness(0);
	}
	
	// Returns the fitness of chromosome i \in {0, ..., getP() - 1}
	double getFitness(unsigned i) const {
		#ifdef RANGECHECK
			if(i >= getP()) { throw std::range_error("Invalid individual identifier."); }
		#endif
		return fitness[i].first;
	}
	
	// Returns (i+1)-th best chromosome, where i = 0 is the best and i = getP() - 1 is the worst:
	const std::vector<double>& getChromosome(unsigned i) const {
		#ifdef RANGECHECK
			if(i >= getP()) { throw std::range_error("Invalid individual identifier."); }
		#endif
		
		return population[fitness[i].second];
	}
	void pushChromosome(const std::vector<double>& chromosome, double fitness) {
		population.push_back(chromosome);
		this->fitness.push_back(std::make_pair(fitness, population.size() - 1));
	}

private:
	// Constructor por copia
	Population(const Population& pop) :
		population(pop.population),
		fitness(pop.fitness) {}

	// Constructor principal con lista de inicialización
	Population(unsigned n, unsigned p) :
		population(p, std::vector<double>(n, 0.0)), fitness(p) {
		if(p == 0) { throw std::range_error("Population size p cannot be zero."); }
		if(n == 0) { throw std::range_error("Chromosome size n cannot be zero."); }
	}

	~Population() {}

	std::vector<std::vector<double>> population;    // Population as vectors of prob.
	std::vector<std::pair<double, unsigned>> fitness; // Fitness (double) of each chromosome

	void sortFitness() {
		std::sort(fitness.begin(), fitness.end());
	}									// Sorts 'fitness' by its first parameter

	void setFitness(unsigned i, double f) {
		fitness[i].first = f;
		fitness[i].second = i;
	}				// Sets the fitness of chromosome i

	std::vector<double>& getChromosome(unsigned i) {
		#ifdef RANGECHECK
			if(i >= getP()) { throw std::range_error("Invalid individual identifier."); }
		#endif
		
		return population[fitness[i].second];
	}	// Returns a chromosome

	double& operator()(unsigned chromosome, unsigned allele) {
		return population[chromosome][allele];		// Direct access to allele j of chromosome i
	}

	std::vector<double>& operator()(unsigned chromosome) {
		return population[chromosome];
	}	// Direct access to chromosome i
};

#endif
