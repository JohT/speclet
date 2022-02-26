#include "Partition.h"
#include <math.h>

Partition::Partition(integer_number dim, real_number a)
{
   integer_number largest_pow =  ((integer_number)(floor((log(dim) / log(a)) + 0.5 )));
   // largest_pow = nearest integer to log(dim)/log(a), i.e. it is
   // the largest integer power of a which is <= dim
   // In this implementation for the smallest power od a we choose 1.
   // In other words smallest s is a.

  // Define partition of s's and corresponding values for dv's and du's
  s.Set(1, largest_pow);
  dv.Set(s.beg, s.end);
  du.Set(s.beg, s.end);
  for(integer_number j = s.beg; j <= s.end; j++) 
    {
      s.origin[j] = pow(a, j);               // s[j] = a^j
      dv.origin[j] = M_PI / s.origin[j];     // dv[j] = Pi / s[j]
      du.origin[j] = s.origin[j] / 2;        // du[j] = s[j]/2;
      
      if( (integer_number)(du.origin[j]) <= 0 )
	{
	  // in this case take du=1
	  du.origin[j] = 1.0;
	}
    
    }
  lmu = 0; 
  rmu = dim - 1; 
  
  // Leftmost value of v is always set to 0 - can't change this
  rmv = 2 * M_PI - 1.0e-10;  // explain....
}
