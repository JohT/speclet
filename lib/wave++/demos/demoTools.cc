//file:demoTools.cc
#include "demoTools.h"
#include <stdio.h>
//last parameter not used, needed only for compatibility and keep
//the demo brief
real OracCost( const real *data,  const integer &n,
	       const real &sigma,const integer &k)
{
  real cost=0;
  real var=sigma*sigma;
  real temp;

  for(int i=0;i<n;i++)
    {
      temp = data[i]*data[i];
      
      if(temp>=var*Factor*Factor) cost += var;
      else cost += temp;
      
      
    }
  return cost;
}

void stop(){
  while(getchar()==0) { 
    getchar();}
}











