//file: demoTools.h
#ifndef TESTTOOLS_H
#define TESTTOOLS_H


#include "../source/libw.h"

//defined in demoWavPack.cc
extern real Factor;

real OracCost( const real *data,  const integer &n,
	       const real &sigma,const integer &k);
void stop();
#endif


















