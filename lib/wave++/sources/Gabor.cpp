// Gabor.cc
// Implementation of real_DWT  Gabor functions

#include "Gabor.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>
  
RealGabor::RealGabor(const real_DWT &S, const real_DWT &U, const real_DWT &V, 
		     const real_DWT &W) : s(S), u(U),v(V), w(W)  
{  
  assert(s>0); 
}
//////////////////////////////////////////////////////////////////////////////
RealGabor::RealGabor(const real_DWT &S, const real_DWT &U, const real_DWT &V, 
		     const real_DWT &W, const Interval &I) : s(S), u(U),v(V), w(W)
{
  assert(s>0);
  createSample(I);
}
//////////////////////////////////////////////////////////////////////////////
void RealGabor::Set(const real_DWT &S, const real_DWT &U, const real_DWT &V, const real_DWT &W)
{
  assert(S>0);
  s=S; u=U; v=V; w=W;
}
//////////////////////////////////////////////////////////////////////////////
void RealGabor::Set(const real_DWT &S, const real_DWT &U, const real_DWT &V, const real_DWT &W,
		    const Interval &I)
{
  assert(S>0);
  s=S; u=U; v=V; w=W;
  createSample(I);
}
//////////////////////////////////////////////////////////////////////////////
real_DWT RealGabor::evaluate(const real_DWT &t) const
{
  return exp(-M_PI*(t-u)*(t-u)/(s*s)) * cos(v*t + w);
}
////////////////////////////////////////////////////////////////////////////
void RealGabor::createSample(const Interval &I) 
{
  Sample.Set(I.beg, I.end);
  real_DWT norm = 0;
  real_DWT *Iptr = I.origin + I.beg, *Sptr = Sample.origin + Sample.beg;
  integer j;
  for(j=0; j<I.length; j++) 
    {
      *Sptr = evaluate( *Iptr );
      norm += (*Sptr) * (*Sptr);
      Sptr++;  Iptr++;
    }
  norm = sqrt(norm);
  if( norm > FLT_EPSILON )
    {
      Sptr = Sample.origin + Sample.beg;
      for(j=0; j<I.length; j++)  { *Sptr /= norm;   Sptr++; }
    }
  else
    {
      std::cout << "You are evaluating Gabor function which is almost O \
on the given interval. Sample is therefore not normalized. Be careful \
with calcualtions involving this Gabor." << std::endl;
    }
}
// end of RealGabor
////////////////////////////////////////////////////////////////////////////

real_DWT g( const real_DWT &x ) {  return exp( - M_PI * x * x ); }
