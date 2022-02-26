//*****************************************************************************
// ArrayTree.h                                      Definitions of classes
//                                               ArrayTreePer and ArrayTreeAper
//*****************************************************************************

#ifndef ARRAYTREE_H
#define ARRAYTREE_H

#include "common.h"
#include "Interval.h"

/************************ Array Tree Periodic ********************************/

class ArrayTreePer {                   
 public:
  ArrayTreePer() : maxlevel(0), dim(0), origin(0) { }        
       // default constructor

  ArrayTreePer(integer MAXLEVEL);                         
       // construct full zero tree with maxlevel=MAXLEVEL and dim=2^maxlevel 

  ArrayTreePer(integer DIM, integer MAXLEVEL);           
       // construct truncated zero tree with maxlevel=MAXLEVEL and dim=DIM
       // check that dim is divisible by  2^maxlevel  

  ArrayTreePer(const ArrayTreePer &);                      // copy constructor

  ~ArrayTreePer() { DestroyTree(); }                       // destructor

  void DestroyTree();                                      // free all memory 
  
  const ArrayTreePer &operator=(const ArrayTreePer &);     // overload =

  real *block_start(const integer &L, const integer &B) const;      
       // pointer to the start of block B at level L

  integer block_length(const integer &L) const;      
       // block length at level L

  real *left_child(const integer &L, const integer &B) const; 
       // pointer to begining of left child of block B at level L

  real *right_child(const integer &L, const integer &B) const; 
       // pointer to begining of right child of block B at level L

  ///////////////////////////////////////////////////////////////////////////
  integer maxlevel;       // levels numbered  from 0 to maxlevel
                                           
  integer dim;            // dim = total length of each level
  
  real *origin;           // pointer to begining of data array
};
// end of ArrayTreePer class definition

/************************ Array Tree Aperiodic *******************************/

class ArrayTreeAper {                        
 public:
  ArrayTreeAper() : maxlevel(0), size(0), root(0) { }           
       // default constructor

  ArrayTreeAper(integer MAXLEVEL);                 
       // construct full tree with maxlevel=MAXLEVEL and 
       // size=2^(maxlevel+1)-1 many empty intervals allocated 

  ArrayTreeAper(const ArrayTreeAper &);                     // copy constructor

  ~ArrayTreeAper() { DestroyTree(); }                       // destructor

  void DestroyTree();                                       // free all memory
  
  const ArrayTreeAper &operator=(const ArrayTreeAper &);    // overload =

  Interval *block(const integer &L, const integer &B) const;         
       // pointer to block B at level L

  Interval *left_child(const integer &L, const integer &B) const; 
       // pointer to left child of block B at level L

  Interval *right_child(const integer &L, const integer &B) const;  
       // pointer to right child of block B at level L
  
  void ZeroTree();               
       // set all elts of all nonempty intervals to 0

  ////////////////////////////////////////////////////////////////////////////
  integer maxlevel;        // levels numbered from 0 to maxlevel

  integer size;            // size=2^(maxlevel+1)-1=total number of intervals

  Interval *root;          // pointer to the first interval
};

#endif
  
  
  
