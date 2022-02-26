//****************************************************************************
// Interval.h                                    Definition of class Interval
//****************************************************************************

#ifndef INTERVAL_H
#define INTERVAL_H

#include "common.h"
#include <iostream.h>

class Interval {
public: 
  Interval() :  origin(0), beg(0), end(-1), length(0) { }  
       // default constructor

  Interval(const integer &alpha, const integer &omega, const real *data = 0);
       // construct interval from data. data must be omega-alpha+1 long,
       // or else by default data=0 and all elts set to 0

  Interval(const integer &p, const real *data=0);       
       // construct interval indexed from 0 to 2^p-1, filled from data or 0's

  Interval(const Interval &);               // copy constructor

  ~Interval();                              // destructor

  const Interval &operator=(const Interval&);     // overload =

  real &operator[](const integer &i);       // overload [] with  bound checking

  const real &operator[](integer i) const;  // overload [] for const objects

  const Interval &operator+=(const Interval &I);  // overload +=another interv.

  const Interval &operator+=(const real &c);      // overload +=scalar
  
  Interval operator+(const Interval &I) const;    // overload +another interval

  Interval operator+(const real &c) const;        // overload +scalar 

  const Interval &operator*=(const real &c);             // overload *=scalar
  
  Interval operator*(const real &c) const;        // overload *scalar
 
  void Set(integer alpha, integer omega, const real *data = 0);  
       // utility which  can be used for redefining an existing interval

  /////////////////////////////////////////////////////////////////////////////
  real* origin;                 // pointer to begining of data

  integer beg;                  // least valid index

  integer end;                  // final valid index

  integer length;               // length = end - beg + 1
};  
// end of class Interval

/**************************** Utilities **************************************/
Interval Absval(const Interval &I);                         
     // return  absolute value of interval as a new interval whose
     // elements are abs values of elements of I

real L2norm(const Interval &I);       // return L2 norm of I

real Average(const Interval &I);      // return average of all elements of I

real StDev(const Interval &I);        // return standard deviation of I

#endif
