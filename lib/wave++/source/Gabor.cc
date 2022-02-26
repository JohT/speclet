// Gabor.cc
// Implementation of real  Gabor functions

#include "Gabor.h"
#include <assert.h>
#include <math.h>
#include <iostream>
#include <cfloat>
  
RealGabor::RealGabor(const real_number &S, const real_number &U, const real_number &V, 
		     const real_number &W) : s(S), u(U),v(V), w(W)  
{  
  assert(s>0); 
}
//////////////////////////////////////////////////////////////////////////////
RealGabor::RealGabor(const real_number &S, const real_number &U, const real_number &V, 
		     const real_number &W, const Interval &I) : s(S), u(U),v(V), w(W)
{
  assert(s>0);
  createSample(I);
}
//////////////////////////////////////////////////////////////////////////////
void RealGabor::Set(const real_number &S, const real_number &U, const real_number &V, const real_number &W)
{
  assert(S>0);
  s=S; u=U; v=V; w=W;
}
//////////////////////////////////////////////////////////////////////////////
void RealGabor::Set(const real_number &S, const real_number &U, const real_number &V, const real_number &W,
		    const Interval &I)
{
  assert(S>0);
  s=S; u=U; v=V; w=W;
  createSample(I);
}
//////////////////////////////////////////////////////////////////////////////
real_number RealGabor::evaluate(const real_number &t) const
{
  return exp(-M_PI*(t-u)*(t-u)/(s*s)) * cos(v*t + w);
}
////////////////////////////////////////////////////////////////////////////
void RealGabor::createSample(const Interval &I) 
{
  Sample.Set(I.beg, I.end);
  real_number norm = 0;
  real_number *Iptr = I.origin + I.beg, *Sptr = Sample.origin + Sample.beg;
  integer_number j;
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
        std::cout << "You are evaluating Gabor function which is almost O"
                  << "on the given interval. Sample is therefore not normalized. Be careful "
                  << "with calcualtions involving this Gabor." << std::endl;
    }
}
// end of RealGabor
////////////////////////////////////////////////////////////////////////////

real_number g( const real_number &x ) {  return exp( - M_PI * x * x ); }
