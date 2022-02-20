// ShiftGaborMP.h

#ifndef SHIFTGABORMP_H
#define SHIFTGABORMP_H

#include "Gabor.h"
#include "Partition.h"
#include <vector>

// Functions which runs MP algorithm until epsilon precision is achieved
// i.e. |Rf|<epsilon, or maximal number of iterations is performed. 
// It makes repeated calls to getOptimalShiftGabor function.
// Works for any dimension of the signal, slower algorithm
real_DWT RunShiftGaborMP(int max_iter, // maximal number of iterations 
		     real_DWT epsilon, // desired precision | Rf | < epsilon
		     const Interval &f, // signal to be approximated
		     // assumption : f sampled on integers starting with 0
		     // i.e.  f.beg must be 0. No constraints on f.length.
		     const Partition &Part, // partiton which defining
		     //dictionary
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
// which is closest in 
// l2 norm to f, from the dictionary of Gabors defined by partition Part.
// On output G is set to this optimal function, no sample for G created.
// Also, on output coef = <f, G>
void getOptimalShiftGabor(const Interval &f,  // signal
			  const Partition &Part, 
			  RealGabor &G, // optimal Gabor, the one closest to f
			  real_DWT &coef   // <f, G>		 
			  );


#endif
