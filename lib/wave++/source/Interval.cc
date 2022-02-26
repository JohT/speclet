//****************************************************************************
// Interval.cc                                 Implementation of class Interval
//****************************************************************************

#include <assert.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <new>
#include "Interval.h"

Interval::Interval(const integer_number &alpha, const integer_number &omega, const real_number *data) : origin(0)
{
  if(data == 0) Set( alpha, omega );
  else Set( alpha, omega, data );
}

///////////////////////////////////////////////////////////////////////////////
Interval::Interval(const integer_number &p, const real_number *data) : origin(0)
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
real_number &Interval::operator[](const integer_number &i)
{
  assert(beg<=i && i<=end);
  return origin[i];
}

///////////////////////////////////////////////////////////////////////////////
const real_number &Interval::operator[](integer_number i)  const
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
	  for(integer_number j=beg; j<=end; j++) origin[j] += I.origin[j];
	}
      else
	{
	  Interval Temp( minimum(beg, I.beg), maximum(end, I.end) );
	  integer_number j;
	  for(j=beg; j<=end; j++) Temp.origin[j] += origin[j];    // add  *this
	  for(j=I.beg; j<=I.end; j++) origin[j] += I.origin[j];   // add  I
	  *this = Temp;
	}
    }
  return *this;
}   

///////////////////////////////////////////////////////////////////////////////
const Interval &Interval::operator+=(const real_number &c)
{
  for(integer_number i=beg; i<=end; i++) origin[i] += c;
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
Interval Interval::operator+(const real_number &c) const
{
  Interval Temp(*this);
  Temp += c;
  return Temp;
}

///////////////////////////////////////////////////////////////////////////////
const Interval &Interval::operator*=(const real_number &c)
{
 for(integer_number i=beg; i<=end; i++) origin[i] *= c;
 return *this;
}

///////////////////////////////////////////////////////////////////////////////
Interval Interval::operator*(const real_number &c) const
{
  Interval Temp(*this);
  Temp *= c;
  return Temp;
}

///////////////////////////////////////////////////////////////////////////////
void Interval::Set(integer_number alpha, integer_number omega, const real_number *data) 
{
  if(origin) delete [] (origin+beg);    // if redefining the existing interval
  length=omega-alpha+1;
  beg=alpha; end=omega; 
  if(length > 0)
    {     
      origin=new real_number [length];
      assert(origin);
      origin -= beg;
      if(data == 0)  for(integer_number i=beg; i<=end; i++) origin[i]=0;
      else for(integer_number i=0; i<length; i++) origin[beg+i]=data[i];
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
      for(integer_number i=I.beg; i<=I.end; i++) Temp.origin[i] = absval(I.origin[i]);
    }
  else  
    std::cout << "Returning empty interval as absolute value of empty interval."
	 << std::endl;
  return Temp;
}

///////////////////////////////////////////////////////////////////////////////
real_number L2norm(const Interval &I)
{
  if(I.origin)
    {
      real_number temp=0;
      for(integer_number j=I.beg; j<=I.end; j++) temp += I.origin[j] * I.origin[j];
      return sqrt(temp);
    }
  else
    {
      std::cout << "Interval empty. Returning 0 as its norm." << std::endl;
      return 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
real_number Average(const Interval &I)
{
  if(I.origin)
    {
      real_number a=0;
      for(integer_number j=I.beg; j<=I.end; j++) a += I.origin[j];
      return a/I.length;
    }
  else
    {
      std::cout << "Interval empty. Returning 0 as its average." << std::endl;
      return 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
real_number StDev(const Interval &I)
{
  if(I.origin)
    {
      real_number a=Average(I);
      real_number var=0;
      for(integer_number j=I.beg; j<=I.end; j++) 
	var += (I.origin[j]-a) * (I.origin[j]-a);
      var /= I.length - 1.0;
      return sqrt(var);
    }
  else 
    {
      std::cout << "Interval empty. Returning 0 as its stdev." << std::endl;
      return 0;
    }
}
 
      

