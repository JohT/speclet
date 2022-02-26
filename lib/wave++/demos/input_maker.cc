// input_maker.cc

#include <iostream.h>
#include <math.h>
#include "input_maker.h"

void Input_maker(int type, Interval &In)
{
  int i,j;
  real_number var, norm = 0;
  const real_number t[11] = {.1, .13, .15, .23, .25, .40, .44, .65, .76, .78, .81};
  const real_number h[11] = {4, -5, 3, -4, 5, -4.2, 2.1, 4.3, -3.1, 5.1, -7.2};
  const real_number w[11] = {.005, .005, .006, .01, .01, .03, .01, .01, .005, .008, .005};
  switch(type)
    {
    case 1:   /* Chirp */
      for(i=In.beg; i<=In.end; i++)
	{
	  var = (1.0*i)/In.length;
	  In.origin[i] = 10.0*(var*var*(1-var)*(1-var))*cos(200*var*var);
	  norm += In.origin[i]*In.origin[i];
	}
      break;		
    case 2:   /* Doppler */
      for(i=In.beg; i<=In.end; i++)
	{
	  var = (1.0*i)/In.length;
	  In.origin[i] = sin(2.0*3.1415926*(1+.05)/(.05 + var));
	  norm += In.origin[i]*In.origin[i];
	}
      break;
		
    case 3:  /* Polynomial */
      for(i=In.beg; i<=In.end; i++)
	{
	  var = (1.0*i)/In.length;
	  In.origin[i]= -96.0*(i)/In.length*(var-1)*(var-1)*(var-1)*(var-.5)*(var-.5);
	  norm += In.origin[i]*In.origin[i];
	}
      break;
		
    case 4:  /* Blocks */
      for(i=In.beg; i<=In.end; i++)
	{
	  var = (1.0*i)/In.length;
	  In.origin[i] = 0;
	  for(j=0;j<11;j++) 
	    {
	      In.origin[i] += ( h[j]*0.5*(1 + signum(var-t[j])) );
	    }
	  norm += In.origin[i]*In.origin[i];
	}
      break;
	
    case 5:  /* Bumps */
      for(i=In.beg; i<=In.end; i++)
	{
	  var = (1.0*i)/In.length;
	  In.origin[i] = 0;
	  for(j=0; j<11; j++)
	    {
	      In.origin[i] += ( h[j]*(funct((var-t[j])/w[j])) );
	    }
	   norm += In.origin[i]*In.origin[i];
	}
      break;
		
    case 6:  /* Heavisine */
      for(i=In.beg; i<=In.end; i++)
	{
	  var = (1.0*i)/In.length;
	  In.origin[i] = 4*sin(4*Pi*var) - signum(var - 0.3) - signum(.72 - var);
	  norm += In.origin[i]*In.origin[i];
	}
      break;

    case 7:  /* Zeros */
      for(i=In.beg; i<=In.end; i++) In.origin[i] = 0.0;
      break;

    case 8: // constant
      for(i=In.beg; i<=In.end; i++)
	{
	  In.origin[i] = 1.0;
	  norm += In.origin[i]*In.origin[i];
	}

      break;    case 9:   /* tent */
      for(i=In.beg; i<=In.end; i++)
	{
	  var = (1.0*i)/In.length;
	  In.origin[i] = fabs(var - 0.5);
	  norm += In.origin[i]*In.origin[i];
	}
      break;		


      
    default:
      cout << "You goofed in Input_maker" << endl;
      exit(1);
      break;
    }
  if (norm > 0) norm = (1.0)/sqrt(norm);
  for(int i=In.beg; i<=In.end; i++) In.origin[i] *= norm;

  return;
}

////////////////////////////////////////////////////////
int signum(real_number x)
{
  int j;
  j = (x>0 ? 1:(x<0 ? -1:0));
  return j;
}
//////////////////////////////////////////////////////////
real_number funct(real_number x)
{	
  return  1.0/(1.0 + x*x*x*x);
}
//////////////////////////////////////////////////////////

