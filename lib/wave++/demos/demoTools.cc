//file:demoTools.cc
#include "demoTools.h"
#include <stdio.h>
//last parameter not used, needed only for compatibility and keep
//the demo brief
real_number OracCost( const real_number *data,  const integer_number &n,
	       const real_number &sigma,const integer_number &k)
{
  real_number cost=0;
  real_number var=sigma*sigma;
  real_number temp;

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











