//file: demoTools.h
#ifndef TESTTOOLS_H
#define TESTTOOLS_H


#include "../source/libw.h"

//defined in demoWavPack.cc
extern real_number Factor;

real_number OracCost( const real_number *data,  const integer_number &n,
	       const real_number &sigma,const integer_number &k);
void stop();
#endif


















