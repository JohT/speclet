//***************************************************************
// common.h
//****************************************************************

#ifndef COMMON_H
#define COMMON_H

typedef double real;
typedef long integer;

#define maximum(x,y)   ((x)>(y) ? (x):(y))
#define minimum(x,y)   ((x)<(y) ? (x):(y))
#define absval(x)  ((x)<0 ? (-(x)):(x))

#define IFH(x)  ((x)&1 ? (((x)-1)/2) : ((x)/2)) // IFH = Interval Floor Half
#define ICH(x)  ((x)&1 ? (((x)+1)/2) : ((x)/2)) // ICH = Interval Ceiling Half
#define LEAST(I,F)  (2*(I.beg) - (F.beg) - 1)   // least after adjoint conv-dec
#define FINAL(I,F)  (2*(I.end) - (F.end) + 1)   // final after adjoint conv-dec

integer Log2( integer r );                 // log base 2 of integer r
bool is_pow_of_2( integer r );             // check if r is power of 2

integer NumOfData(const char *filename);   
     // count total=number of reals in file, return total
#endif
