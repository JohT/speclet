// Gabor.cc
// Implementation of real  Gabor functions

#include "Gabor.h"
#include <assert.h>
#include <math.h>
  
RealGabor::RealGabor(const real &S, const real &U, const real &V, 
		     const real &W) : s(S), u(U),v(V), w(W)  
{  
  assert(s>0); 
}
//////////////////////////////////////////////////////////////////////////////
RealGabor::RealGabor(const real &S, const real &U, const real &V, 
		     const real &W, const Interval &I) : s(S), u(U),v(V), w(W)
{
  assert(s>0);
  createSample(I);
}
//////////////////////////////////////////////////////////////////////////////
void RealGabor::Set(const real &S, const real &U, const real &V, const real &W)
{
  assert(S>0);
  s=S; u=U; v=V; w=W;
}
//////////////////////////////////////////////////////////////////////////////
void RealGabor::Set(const real &S, const real &U, const real &V, const real &W,
		    const Interval &I)
{
  assert(S>0);
  s=S; u=U; v=V; w=W;
  createSample(I);
}
//////////////////////////////////////////////////////////////////////////////
real RealGabor::evaluate(const real &t) const
{
  return exp(-M_PI*(t-u)*(t-u)/(s*s)) * cos(v*t + w);
}
////////////////////////////////////////////////////////////////////////////
void RealGabor::createSample(const Interval &I) 
{
  Sample.Set(I.beg, I.end);
  real norm = 0;
  real *Iptr = I.origin + I.beg, *Sptr = Sample.origin + Sample.beg;
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
      cout << "You are evaluating Gabor function which is almost O
on the given interval. Sample is therefore not normalized. Be careful 
with calcualtions involving this Gabor." << endl;
    }
}
// end of RealGabor
////////////////////////////////////////////////////////////////////////////

real g( const real &x ) {  return exp( - M_PI * x * x ); }
