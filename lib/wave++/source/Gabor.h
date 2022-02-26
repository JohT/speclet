// Gabor.h
// contains classes reprezenting real and complex Gabor functions
#ifndef GABOR_H
#define GABOR_H

#include "Interval.h"

class RealGabor 
{
 public:
  RealGabor() : s(1), u(0), v(0), w(0) {};
  RealGabor(const real &S, const real &U, const real &V, 
	    const real &W);
  // set s, u, v, w,  no sample created
  RealGabor(const real &S, const real &U, const real &V, 
	    const real &W, const Interval & I);
  // set s, u, v, w, Sample created by evaluating on I
  ~RealGabor() {};
  void Set(const real &S, const real &U, const real &V, const real &W);
  void Set(const real &S, const real &U, const real &V, const real &W,
	   const Interval &I);
  real evaluate( const real &t ) const; // return g((t-u)/s) * cos(vt + w)
  // where g(x) = exp(-Pi*x*x)
  void createSample(const Interval &I); 
  // create normalized sample on I, i.e. its L2norm is 1
  
  real s, u, v, w;
  Interval Sample;
};

real g( const real &x );

#endif
