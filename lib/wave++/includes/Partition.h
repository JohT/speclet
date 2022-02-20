// Partition.h

#ifndef PARTITION_H
#define PARTITION_H

#include  "Interval.h"

class Partition
{
 public:
  Partition(integer dim, real_DWT a);

  Interval s;     // scale
  Interval du; 
  Interval dv;
  integer lmu, rmu;
  real_DWT rmv;
};



#endif
