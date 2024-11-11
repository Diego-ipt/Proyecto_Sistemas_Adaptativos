/**
 * SampleDecoder.h
 *
 * The chromosome's fitness is computed as the sum of its individual alleles. We also show how to
 * generate a random permutation of {0, 1, ..., n-1} using the supplied chromosome.
 *
 * Any Decoder class must implement either of the methods:
 *     1. double decode(std::vector< double >&)
 *     2. double decode(std::vector< double >&) const
 *     3. double decode(const std::vector< double >&)
 *     4. double decode(const std::vector< double >&) const
 * where the returned double corresponds to the fitness of that chromosome. If parallel decoding
 * is to be used in the BRKGA framework, then decode() *must* be thread-safe; the best way to
 * guarantee this is by via member const-correctness -- see (2) and (4) above --  so that the
 * property will be checked at compile time. An exception to this rule is the use of the mutant
 * modifier in data members of the decoder, which bypasses const-correctness and may introduce
 * dangerous race conditions in a multithreaded environment.
 *
 * The chromosome inside the BRKGA framework can be changed if desired. To do so, just use the
 * signatures (1) and (3) above which do not forbid the chromosome to be changed (and such change
 * gets reflected in BRKGA). Please use double values in the interval [0,1) when updating, thus
 * obeying the BRKGA guidelines -- for speed, BRKGA does *not* enforce this.
 *
 * Created on : Nov 17, 2011 by rtoso
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
#ifndef DECODER_H
#define DECODER_H
#include <mutex>
#include "../funciones_greedy.h"
#include <vector>
#include <algorithm>
#include <string>
#include <numeric>
#include <iostream>

class Decoder {
public:
    Decoder(){};	// Constructor
    ~Decoder(){};	// Destructor

    std::string traduccion(const std::vector< double >& chromosome) const {
        std::string solucion = "";
        int size = chromosome.size();

        //traduccion a caracteres
        for(unsigned i = 0; i < size; i++) {
            if(chromosome[i] < 0.25) {
                solucion += 'A';
            } else if(chromosome[i] >= 0.25 && chromosome[i] < 0.5) {
                solucion += 'G';
            } else if(chromosome[i] >= 0.5 && chromosome[i] < 0.75) {
                solucion += 'T';
            } else if(chromosome[i] >= 0.75 && chromosome[i] < 1) {
                solucion += 'C';
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