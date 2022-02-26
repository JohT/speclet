//****************************************************************************
// Interval.h                                    Definition of class Interval
//****************************************************************************

#ifndef INTERVAL_H
#define INTERVAL_H

#include "common.h"
#include <iostream>

class Interval {
public: 
  Interval() :  origin(0), beg(0), end(-1), length(0) { }  
       // default constructor

  Interval(const integer_number &alpha, const integer_number &omega, const real_number *data = 0);
       // construct interval from data. data must be omega-alpha+1 long,
       // or else by default data=0 and all elts set to 0

  Interval(const integer_number &p, const real_number *data=0);       
       // construct interval indexed from 0 to 2^p-1, filled from data or 0's

  Interval(const Interval &);               // copy constructor

  ~Interval();                              // destructor

  const Interval &operator=(const Interval&);     // overload =

  real_number &operator[](const integer_number &i);       // overload [] with  bound checking

  const real_number &operator[](integer_number i) const;  // overload [] for const objects

  const Interval &operator+=(const Interval &I);  // overload +=another interv.

  const Interval &operator+=(const real_number &c);      // overload +=scalar
  
  Interval operator+(const Interval &I) const;    // overload +another interval

  Interval operator+(const real_number &c) const;        // overload +scalar 

  const Interval &operator*=(const real_number &c);             // overload *=scalar
  
  Interval operator*(const real_number &c) const;        // overload *scalar
 
  void Set(integer_number alpha, integer_number omega, const real_number *data = 0);  
       // utility which  can be used for redefining an existing interval

  /////////////////////////////////////////////////////////////////////////////
  real_number* origin;                 // pointer to begining of data

  integer_number beg;                  // least valid index

  integer_number end;                  // final valid index

  integer_number length;               // length = end - beg + 1
};  
// end of class Interval

/**************************** Utilities **************************************/
Interval Absval(const Interval &I);                         
     // return  absolute value of interval as a new interval whose
     // elements are abs values of elements of I

real_number L2norm(const Interval &I);       // return L2 norm of I

real_number Average(const Interval &I);      // return average of all elements of I

real_number StDev(const Interval &I);        // return standard deviation of I

#endif
