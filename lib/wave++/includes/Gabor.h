// Gabor.h
// contains classes reprezenting real_DWT and complex_DWT Gabor functions
#ifndef GABOR_H
#define GABOR_H

#include "Interval.h"

class RealGabor 
{
 public:
  RealGabor() : s(1), u(0), v(0), w(0) {};
  RealGabor(const real_DWT &S, const real_DWT &U, const real_DWT &V, 
	    const real_DWT &W);
  // set s, u, v, w,  no sample created
  RealGabor(const real_DWT &S, const real_DWT &U, const real_DWT &V, 
	    const real_DWT &W, const Interval & I);
  // set s, u, v, w, Sample created by evaluating on I
  ~RealGabor() {};
  void Set(const real_DWT &S, const real_DWT &U, const real_DWT &V, const real_DWT &W);
  void Set(const real_DWT &S, const real_DWT &U, const real_DWT &V, const real_DWT &W,
	   const Interval &I);
  real_DWT evaluate( const real_DWT &t ) const; // return g((t-u)/s) * cos(vt + w)
  // where g(x) = exp(-Pi*x*x)
  void createSample(const Interval &I); 
  // create normalized sample on I, i.e. its L2norm is 1
  
  real_DWT s, u, v, w;
  Interval Sample;
};

real_DWT g( const real_DWT &x );

#endif
