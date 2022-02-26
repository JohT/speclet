#include "common.h"
#include <fstream>
#include <iostream>
#include <assert.h>

integer_number Log2(integer_number r)
{
  integer_number temp=0;
  r >>= 1;
  while(r > 0) 
    {
      r >>= 1;
      temp++;
    }
  return temp;
}
//////////////////////////////////////////////////////////////////////////////
bool is_pow_of_2( integer_number r )
{
  return ( ((r>>Log2(r))<<Log2(r)) == r );
}
///////////////////////////////////////////////////////////////////////////////
integer_number NumOfData(const char *filename)
{
  std::ifstream glug(filename);
  if( !glug )
    {
      std::cout << "Can't open input file " << filename << ". Exiting." << std::endl;
      exit(1);
    }
  real_number x;
  integer_number total=0;
  while( !glug.eof() )
    { 
      glug >> x;
      total++;
    }
  return total;
}

///////////////////////////////////////////////////////////////////////////////
