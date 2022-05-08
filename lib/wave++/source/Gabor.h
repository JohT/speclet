// Gabor.h
// contains classes reprezenting real and complex Gabor functions
#ifndef GABOR_H
#define GABOR_H

#include "Interval.h"

class RealGabor 
{
 public:
  RealGabor() : s(1), u(0), v(0), w(0) {};
  RealGabor(const real_number &S, const real_number &U, const real_number &V, 
	    const real_number &W);
  // set s, u, v, w,  no sample created
  RealGabor(const real_number &S, const real_number &U, const real_number &V, 
	    const real_number &W, const Interval & I);
  // set s, u, v, w, Sample created by evaluating on I
  ~RealGabor() {};
  void Set(const real_number &S, const real_number &U, const real_number &V, const real_number &W);
  void Set(const real_number &S, const real_number &U, const real_number &V, const real_number &W,
	   const Interval &I);
  real_number evaluate( const real_number &t ) const; // return g((t-u)/s) * cos(vt + w)
  // where g(x) = exp(-Pi*x*x)
  void createSample(const Interval &I); 
  // create normalized sample on I, i.e. its L2norm is 1
  
  real_number s, u, v, w;
  Interval Sample;
};

real_number g( const real_number &x );

#endif
