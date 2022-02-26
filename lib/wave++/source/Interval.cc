//****************************************************************************
// Interval.cc                                 Implementation of class Interval
//****************************************************************************

#include <assert.h>
#include <math.h>
#include <iostream.h>
#include <fstream.h>
#include <new.h>
#include "Interval.h"

Interval::Interval(const integer &alpha, const integer &omega, const real *data) : origin(0)
{
  if(data == 0) Set( alpha, omega );
  else Set( alpha, omega, data );
}

///////////////////////////////////////////////////////////////////////////////
Interval::Interval(const integer &p, const real *data) : origin(0)
{
  if(data==0) Set(0, (1<<p)-1);
  else Set(0, (1<<p)-1, data);
}

///////////////////////////////////////////////////////////////////////////////
Interval::Interval(const Interval &Rhs) : origin(0)
{  
  Set( Rhs.beg, Rhs.end, Rhs.origin + Rhs.beg);
}

Interval::~Interval()
{
  if(origin)  delete [] (origin + beg); 
  origin=0;
}

///////////////////////////////////////////////////////////////////////////////
const Interval &Interval::operator=(const Interval &Rhs)
{
  if(this != &Rhs) 
    {
      if(Rhs.origin) Set( Rhs.beg, Rhs.end, Rhs.origin + Rhs.beg );
      else Set(0, -1, 0);
    }
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
real &Interval::operator[](const integer &i)
{
  assert(beg<=i && i<=end);
  return origin[i];
}

///////////////////////////////////////////////////////////////////////////////
const real &Interval::operator[](integer i)  const
{
  assert(beg<=i && i<=end);
  return origin[i];  
}

///////////////////////////////////////////////////////////////////////////////
const Interval &Interval::operator+=(const Interval &I)
{
  if(I.origin)          // if I empty, return without any changes
    {
      if(beg==I.beg && end==I.end)
	{
	  for(integer j=beg; j<=end; j++) origin[j] += I.origin[j];
	}
      else
	{
	  Interval Temp( minimum(beg, I.beg), maximum(end, I.end) );
	  integer j;
	  for(j=beg; j<=end; j++) Temp.origin[j] += origin[j];    // add  *this
	  for(j=I.beg; j<=I.end; j++) origin[j] += I.origin[j];   // add  I
	  *this = Temp;
	}
    }
  return *this;
}   

///////////////////////////////////////////////////////////////////////////////
const Interval &Interval::operator+=(const real &c)
{
  for(integer i=beg; i<=end; i++) origin[i] += c;
  return *this;
}
  
///////////////////////////////////////////////////////////////////////////////
Interval Interval::operator+(const Interval &I) const
{
  Interval Temp(*this);
  Temp += I;
  return Temp;
}

///////////////////////////////////////////////////////////////////////////////
Interval Interval::operator+(const real &c) const
{
  Interval Temp(*this);
  Temp += c;
  return Temp;
}

///////////////////////////////////////////////////////////////////////////////
const Interval &Interval::operator*=(const real &c)
{
 for(integer i=beg; i<=end; i++) origin[i] *= c;
 return *this;
}

///////////////////////////////////////////////////////////////////////////////
Interval Interval::operator*(const real &c) const
{
  Interval Temp(*this);
  Temp *= c;
  return Temp;
}

///////////////////////////////////////////////////////////////////////////////
void Interval::Set(integer alpha, integer omega, const real *data) 
{
  if(origin) delete [] (origin+beg);    // if redefining the existing interval
  length=omega-alpha+1;
  beg=alpha; end=omega; 
  if(length > 0)
    {     
      origin=new real [length];
      assert(origin);
      origin -= beg;
      if(data == 0)  for(integer i=beg; i<=end; i++) origin[i]=0;
      else for(integer i=0; i<length; i++) origin[beg+i]=data[i];
    } 
  else origin=0;
}

/*********************** Interval utilities **********************************/

Interval Absval(const Interval &I)
{
  Interval Temp;
  if(I.origin)
    {
      Temp.Set(I.beg, I.end);
      for(integer i=I.beg; i<=I.end; i++) Temp.origin[i] = absval(I.origin[i]);
    }
  else  
    cout << "Returning empty interval as absolute value of empty interval."
	 << endl;
  return Temp;
}

///////////////////////////////////////////////////////////////////////////////
real L2norm(const Interval &I)
{
  if(I.origin)
    {
      real temp=0;
      for(integer j=I.beg; j<=I.end; j++) temp += I.origin[j] * I.origin[j];
      return sqrt(temp);
    }
  else
    {
      cout << "Interval empty. Returning 0 as its norm." << endl;
      return 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
real Average(const Interval &I)
{
  if(I.origin)
    {
      real a=0;
      for(integer j=I.beg; j<=I.end; j++) a += I.origin[j];
      return a/I.length;
    }
  else
    {
      cout << "Interval empty. Returning 0 as its average." << endl;
      return 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
real StDev(const Interval &I)
{
  if(I.origin)
    {
      real a=Average(I);
      real var=0;
      for(integer j=I.beg; j<=I.end; j++) 
	var += (I.origin[j]-a) * (I.origin[j]-a);
      var /= I.length - 1.0;
      return sqrt(var);
    }
  else 
    {
      cout << "Interval empty. Returning 0 as its stdev." << endl;
      return 0;
    }
}
 
      

