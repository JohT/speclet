//file: rms_error.cc  This returns ||in - out||/||in|| in l^2-norm

#include <math.h>
#include "rms_error.h"

real RMS_error(const Interval &input, const Interval &output )
{
  assert(input.beg==output.beg && input.end==output.end);
  real error = 0, temp = 0;
  real temp2;
  for(int i=input.beg; i<=input.end; i++)
    {
      temp2 = input[i]-output[i];
      error += temp2*temp2;
      temp += input[i]*input[i];
    }
  error /= temp;
  error = sqrt(error);
  return error;
}






