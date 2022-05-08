// Partition.h

#ifndef PARTITION_H
#define PARTITION_H

#include  "Interval.h"
#include "common.h"

class Partition
{
 public:
  Partition(integer_number dim, real_number a);

  Interval s;     // scale
  Interval du; 
  Interval dv;
  integer_number lmu, rmu;
  real_number rmv;
};



#endif
