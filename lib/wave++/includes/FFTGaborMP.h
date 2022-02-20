#ifndef FFTGABORMP_H
#define FFTGABORMP_H

#include "Gabor.h"
#include <vector>

// Functions which runs MP algorithm until epsilon precision is achieved
// i.e. |Rf|<epsilon, or maximal number of iterations is performed. 
// It makes repeated calls to getOptimalFFTGabor function.
// Partitionis fixed (Mallat).
// Works only for signals whose length is power of 2
real_DWT RunFFTGaborMP(int max_iter, // maximal number of iterations 
		     real_DWT epsilon, // desired precision | Rf | < epsilon
		     const Interval &f, // signal to be approximated
		     // assumption : f sampled on integers starting with 0
		     // i.e.  f.beg must be 0
		     // check that f.length is power if 2 performed
		     Interval &f_approx, // MP approximation of f
		     // f_approx is a linear combination of Gabors
		     Interval &Rf, // final residual: Rf = f - f_approx
		     // error which is returned equals | Rf |
		     std::vector <RealGabor> &G, // std::vector of Gabors chosen for
		     // f_approx
		     std::vector <real_DWT> & Gcoef // coeficinets in the linear
		     // combination, corresponding to Gabors in G
		     // f_approx = sum( Gcoef[i] * G[i] ), where number of i's
		     // depends on epsilon and max_iter
		    );
// Given signal f, getOptimalShiftGabor finds Gabor function G 
// which is closest in l2 norm to f, from the dictionary of Gabors 
// defined by Mallat's partition.
// On output G is set to this optimal function, no sample for G created.
// Also, on output coef = <f, G>
void getOptimalFFTGabor(const Interval &f); 
	
// utility function
void proces1(const integer &N, const real_DWT *fptr);

// utility function
void proces2(const integer &N, const integer &stop, const real_DWT *fptr);
	
// utility for updating global variables
void update();




#endif
