// MTRand.h
// Slightly modified C++ class based on Richard J. Wagner's MersenneTwister.h
// The only changes except for the file name are w.r.t. API:
// 1. File name was changed to reflect the class name (MTRand)
// 2. Disabled 	double rand() and double rand(const double n) in favor of
//    a double rand() that simply calls double rand53() which returns a
//    53-bit real number in [0,1)
// 3. Updated the constructors to initialize members pNext and left in the
//    member initialization list
// Rodrigo Franco Toso (rtoso@cs.rutgers.edu)
// Mauricio G.C. Resende (mgcr@research.att.com)

// MersenneTwister.h
// Mersenne Twister random number generator -- a C++ class MTRand
// Based on code by Makoto Matsumoto, Takuji Nishimura, and Shawn Cokus
// Richard J. Wagner  v1.1  28 September 2009  wagnerr@umich.edu

// The Mersenne Twister is an algorithm for generating random numbers.  It
// was designed with consideration of the flaws in various other generators.
// The period, 2^19937-1, and the order of equidistribution, 623 dimensions,
// are far greater.  The generator is also fast; it avoids multiplication and
// division, and it benefits from caches and pipelines.  For more information
// see the inventors' web page at
// http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html

// Reference
// M. Matsumoto and T. Nishimura, "Mersenne Twister: A 623-Dimensionally
// Equidistributed Uniform Pseudo-Random Number Generator", ACM Transactions on
// Modeling and Computer Simulation, Vol. 8, No. 1, January 1998, pp 3-30.

// Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
// Copyright (C) 2000 - 2009, Richard J. Wagner
// Copyright (C) 2010 - 2011, Rodrigo Toso and Mauricio Resende
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
//   1. Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//   3. The names of its contributors may not be used to endorse or promote 
//      products derived from this software without specific prior written 
//      permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// The original code included the following notice:
// 
//     When you use this, send an email to: m-mat@math.sci.hiroshima-u.ac.jp
//     with an appropriate reference to your work.
// 
// It would be nice to CC: wagnerr@umich.edu and Cokus@math.washington.edu
// when you write.

#ifndef MTRAND_H
#define MTRAND_H

// Not thread safe (unless auto-initialization is avoided and each thread has
// its own MTRand object)
#pragma once

#include <iostream>
#include <climits>
#include <cstdio>
#include <ctime>
#include <cmath>

class MTRand {
public:
	using uint32 = unsigned long;

	static constexpr int N = 624;
	static constexpr int SAVE = N + 1;

protected:
	static constexpr int M = 397;

	uint32 state[N];
	uint32 *pNext;
	int left;

public:
	MTRand(uint32 oneSeed);
	MTRand(uint32 *const bigSeed, uint32 seedLength = N);
	MTRand(const MTRand& o);

	uint32 randInt();
	uint32 randInt(uint32 n);
	double randExc();
	double randExc(double n);
	double randDblExc();
	double randDblExc(double n);
	double operator()();
	double rand();
	double rand53();
	double randNorm(double mean = 0.0, double stddev = 1.0);
	void seed(uint32 oneSeed);
	void seed(uint32 *const bigSeed, uint32 seedLength = N);
	void save(uint32* saveArray) const;
	void load(uint32 *const loadArray);

	friend std::ostream& operator<<(std::ostream& os, const MTRand& mtrand);
	friend std::istream& operator>>(std::istream& is, MTRand& mtrand);

	MTRand& operator=(const MTRand& o);

protected:
	void initialize(uint32 oneSeed);
	void reload();
	uint32 hiBit(uint32 u) const { return u & 0x80000000UL; }
	uint32 loBit(uint32 u) const { return u & 0x00000001UL; }
	uint32 loBits(uint32 u) const { return u & 0x7fffffffUL; }
	uint32 mixBits(uint32 u, uint32 v) const { return hiBit(u) | loBits(v); }
	uint32 magic(uint32 u) const { return loBit(u) ? 0x9908b0dfUL : 0x0UL; }
	uint32 twist(uint32 m, uint32 s0, uint32 s1) const {
		return m ^ (mixBits(s0, s1) >> 1) ^ magic(s1);
	}
	static uint32 hash(time_t t, clock_t c);
};

inline MTRand::uint32 MTRand::hash(time_t t, clock_t c) {
	static uint32 differ = 0;
	uint32 h1 = 0;
	auto* p = reinterpret_cast<unsigned char*>(&t);
	for (size_t i = 0; i < sizeof(t); ++i) {
		h1 *= UCHAR_MAX + 2U;
		h1 += p[i];
	}
	uint32 h2 = 0;
	p = reinterpret_cast<unsigned char*>(&c);
	for (size_t j = 0; j < sizeof(c); ++j) {
		h2 *= UCHAR_MAX + 2U;
		h2 += p[j];
	}
	return (h1 + differ++) ^ h2;
}

inline void MTRand::initialize(uint32 seed) {
	auto* s = state;
	auto* r = state;
	int i = 1;
	*s++ = seed & 0xffffffffUL;
	for (; i < N; ++i) {
		*s++ = (1812433253UL * (*r ^ (*r >> 30)) + i) & 0xffffffffUL;
		++r;
	}
}

inline void MTRand::reload() {
	const int MmN = M - N;
	auto* p = state;
	for (int i = N - M; i--; ++p) {
		*p = twist(p[M], p[0], p[1]);
	}
	for (int i = M; --i; ++p) {
		*p = twist(p[MmN], p[0], p[1]);
	}
	*p = twist(p[MmN], p[0], state[0]);

	left = N;
	pNext = state;
}

inline void MTRand::seed(uint32 oneSeed) {
	initialize(oneSeed);
	reload();
}

inline void MTRand::seed(uint32 *const bigSeed, uint32 seedLength) {
	initialize(19650218UL);
	int i = 1;
	uint32 j = 0;
	int k = (N > int(seedLength) ? N : int(seedLength));
	for (; k; --k) {
		state[i] ^= ((state[i - 1] ^ (state[i - 1] >> 30)) * 1664525UL);
		state[i] += (bigSeed[j] & 0xffffffffUL) + j;
		state[i] &= 0xffffffffUL;
		++i;
		++j;
		if (i >= N) { state[0] = state[N - 1]; i = 1; }
		if (j >= seedLength) j = 0;
	}
	for (k = N - 1; k; --k) {
		state[i] ^= ((state[i - 1] ^ (state[i - 1] >> 30)) * 1566083941UL);
		state[i] -= i;
		state[i] &= 0xffffffffUL;
		++i;
		if (i >= N) { state[0] = state[N - 1]; i = 1; }
	}
	state[0] = 0x80000000UL;
	reload();
}

inline MTRand::MTRand(uint32 oneSeed) : pNext(nullptr), left(0) { seed(oneSeed); }

inline MTRand::MTRand(uint32 *const bigSeed, uint32 seedLength)
	: pNext(nullptr), left(0) { seed(bigSeed, seedLength); }

inline MTRand::MTRand(const MTRand& o) : pNext(nullptr), left(0) {
	const uint32 *t = o.state;
	uint32 *s = state;
	int i = N;
	for (; i--; *s++ = *t++) {}
	left = o.left;
	pNext = &state[N - left];
}

inline MTRand::uint32 MTRand::randInt() {
	if (left == 0) reload();
	--left;

	uint32 s1 = *pNext++;
	s1 ^= (s1 >> 11);
	s1 ^= (s1 << 7) & 0x9d2c5680UL;
	s1 ^= (s1 << 15) & 0xefc60000UL;
	return s1 ^ (s1 >> 18);
}

inline MTRand::uint32 MTRand::randInt(uint32 n) {
	uint32 used = n;
	used |= used >> 1;
	used |= used >> 2;
	used |= used >> 4;
	used |= used >> 8;
	used |= used >> 16;

	uint32 i;
	do
		i = randInt() & used;
	while (i > n);
	return i;
}

inline double MTRand::randExc() {
	return double(randInt()) * (1.0 / 4294967296.0);
}

inline double MTRand::randExc(double n) {
	return randExc() * n;
}

inline double MTRand::randDblExc() {
	return (double(randInt()) + 0.5) * (1.0 / 4294967296.0);
}

inline double MTRand::randDblExc(double n) {
	return randDblExc() * n;
}

inline double MTRand::rand53() {
	uint32 a = randInt() >> 5, b = randInt() >> 6;
	return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

inline double MTRand::rand() {
	return rand53();
}

inline double MTRand::randNorm(double mean, double stddev) {
	double x, y, r;
	do {
		x = 2.0 * rand53() - 1.0;
		y = 2.0 * rand53() - 1.0;
		r = x * x + y * y;
	} while (r >= 1.0 || r == 0.0);
	double s = sqrt(-2.0 * log(r) / r);
	return mean + x * s * stddev;
}

inline double MTRand::operator()() {
	return rand53();
}

inline void MTRand::save(uint32* saveArray) const {
	const uint32 *s = state;
	uint32 *sa = saveArray;
	int i = N;
	for (; i--; *sa++ = *s++) {}
	*sa = left;
}

inline void MTRand::load(uint32 *const loadArray) {
	uint32 *s = state;
	uint32 *la = loadArray;
	int i = N;
	for (; i--; *s++ = *la++) {}
	left = *la;
	pNext = &state[N - left];
}

inline std::ostream& operator<<(std::ostream& os, const MTRand& mtrand) {
	const MTRand::uint32 *s = mtrand.state;
	int i = mtrand.N;
	for (; i--; os << *s++ << '\t') {}
	return os << mtrand.left;
}

inline std::istream& operator>>(std::istream& is, MTRand& mtrand) {
	MTRand::uint32 *s = mtrand.state;
	int i = mtrand.N;
	for (; i--; is >> *s++) {}
	is >> mtrand.left;
	mtrand.pNext = &mtrand.state[mtrand.N - mtrand.left];
	return is;
}

inline MTRand& MTRand::operator=(const MTRand& o) {
	if (this == &o) return *this;
	const uint32 *t = o.state;
	uint32 *s = state;
	int i = N;
	for (; i--; *s++ = *t++) {}
	left = o.left;
	pNext = &state[N - left];
	return *this;
}


#endif  // MERSENNETWISTER_H

// Change log:
//
// v0.1 - First release on 15 May 2000
//      - Based on code by Makoto Matsumoto, Takuji Nishimura, and Shawn Cokus
//      - Translated from C to C++
//      - Made completely ANSI compliant
//      - Designed convenient interface for initialization, seeding, and
//        obtaining numbers in default or user-defined ranges
//      - Added automatic seeding from /dev/urandom or time() and clock()
//      - Provided functions for saving and loading generator state
//
// v0.2 - Fixed bug which reloaded generator one step too late
//
// v0.3 - Switched to clearer, faster reload() code from Matthew Bellew
//
// v0.4 - Removed trailing newline in saved generator format to be consistent
//        with output format of built-in types
//
// v0.5 - Improved portability by replacing static const int's with enum's and
//        clarifying return values in seed(); suggested by Eric Heimburg
//      - Removed MAXINT constant; use 0xffffffffUL instead
//
// v0.6 - Eliminated seed overflow when uint32 is larger than 32 bits
//      - Changed integer [0,n] generator to give better uniformity
//
// v0.7 - Fixed operator precedence ambiguity in reload()
//      - Added access for real numbers in (0,1) and (0,n)
//
// v0.8 - Included time.h header to properly support time_t and clock_t
//
// v1.0 - Revised seeding to match 26 Jan 2002 update of Nishimura and Matsumoto
//      - Allowed for seeding with arrays of any length
//      - Added access for real numbers in [0,1) with 53-bit resolution
//      - Added access for real numbers from normal (Gaussian) distributions
//      - Increased overall speed by optimizing twist()
//      - Doubled speed of integer [0,n] generation
//      - Fixed out-of-range number generation on 64-bit machines
//      - Improved portability by substituting literal constants for long enum's
//      - Changed license from GNU LGPL to BSD
//
// v1.1 - Corrected parameter label in randNorm from "variance" to "stddev"
//      - Changed randNorm algorithm from basic to polar form for efficiency
//      - Updated includes from deprecated <xxxx.h> to standard <cxxxx> forms
//      - Cleaned declarations and definitions to please Intel compiler
//      - Revised twist() operator to work on ones'-complement machines
//      - Fixed reload() function to work when N and M are unsigned
//      - Added copy constructor and copy operator from Salvador Espana
