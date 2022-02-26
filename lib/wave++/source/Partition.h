// Partition.h

#ifndef PARTITION_H
#define PARTITION_H

#include  "Interval.h"

class Partition
{
 public:
  Partition(integer dim, real a);

  Interval s;     // scale
  Interval du; 
  Interval dv;
  integer lmu, rmu;
  real rmv;
};



#endif
