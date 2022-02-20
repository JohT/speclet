//*****************************************************************************
// ArrayTree.cc                                    Implementations of classes
//                                               ArrayTreePer and ArrayTreeAper
//*****************************************************************************

#include <assert.h>
#include <fstream>
#include "ArrayTree.h"

/*******************************  ArrayTreePer *******************************/

ArrayTreePer::ArrayTreePer(integer MAXLEVEL) : maxlevel(MAXLEVEL)
{
  assert(maxlevel>0);
  dim = 1<<maxlevel;
  integer total = dim * (maxlevel + 1);
  origin = new real_DWT [total];    
  assert(origin); 
  for(integer i=0; i<total; i++) origin[i] = 0;
}

///////////////////////////////////////////////////////////////////////////////
ArrayTreePer::ArrayTreePer(integer DIM, integer MAXLEVEL) : 
  maxlevel(MAXLEVEL), dim(DIM)
{
  assert(maxlevel > 0 && ((dim>>maxlevel)<<maxlevel) == dim);  
  integer total = dim * (maxlevel + 1);
  origin = new real_DWT [total];              
  assert(origin); 
  for(integer i=0; i<total; i++) origin[i] = 0;
}

/////////////////////////////////////////////////////////////////////////////  
ArrayTreePer::ArrayTreePer(const ArrayTreePer &Rhs) : 
  maxlevel(Rhs.maxlevel), dim(Rhs.dim)
{
  if( Rhs.origin != 0 )
    {
      integer total = dim * (maxlevel+1);
      origin = new real_DWT [total];
      assert(origin);
      for(integer i=0; i<total; i++) origin[i] = Rhs.origin[i];
    }
  else origin = 0;
}

/////////////////////////////////////////////////////////////////////////////
void ArrayTreePer::DestroyTree()
{
  if(origin != 0) delete [] origin;
  origin = 0;  
  dim = maxlevel = 0;
}

/////////////////////////////////////////////////////////////////////////////
const ArrayTreePer &ArrayTreePer::operator=(const ArrayTreePer &Rhs)
{
  if(this != &Rhs )
    {
      DestroyTree();
      maxlevel = Rhs.maxlevel;
      dim = Rhs.dim;
      integer total = dim * (maxlevel+1);
      origin = new real_DWT [total];
      assert(origin);
      for(integer i=0; i<total; i++) origin[i] = Rhs.origin[i];
    }
  return (*this);
}

/////////////////////////////////////////////////////////////////////////////
real_DWT *ArrayTreePer::block_start(const integer &L, const integer &B) const
{
  assert( 0<=L && L<=maxlevel && 0<=B && B<(1<<L) );
  return origin + (L * dim + B * ( (dim) >> (L) ));  
}

/////////////////////////////////////////////////////////////////////////////
integer ArrayTreePer::block_length(const integer &L) const
{
  assert(0<=L && L<=maxlevel);
  return (dim) >> (L);                                        // 2^(maxlevel-L)
}

/////////////////////////////////////////////////////////////////////////////
real_DWT *ArrayTreePer::left_child(const integer &L, const integer &B) const
{
   assert( 0<=L && L<maxlevel && 0<=B && B<(1<<L) );
   return origin + (L+1)*dim + B*(dim>>L);             // block_start(L+1, 2*B)
}

/////////////////////////////////////////////////////////////////////////////
real_DWT *ArrayTreePer::right_child(const integer &L, const integer &B) const
{
  assert( 0<=L && L<maxlevel && 0<=B && B<(1<<L) );
  return origin + (L+1)*dim + B*(dim>>L) + (dim>>(L+1)); 
                                                     // block_start(L+1, 2*B+1)
}
// end of implementation of class ArrayTreePer

/*******************************  ArrayTreeAper ******************************/

ArrayTreeAper::ArrayTreeAper(integer MAXLEVEL) : maxlevel(MAXLEVEL)
{
  assert(maxlevel>0);
  size = ((1)<<(maxlevel+1)) - 1;
  root = new Interval [size];
  assert(root);
}

/////////////////////////////////////////////////////////////////////////////
ArrayTreeAper::ArrayTreeAper(const ArrayTreeAper &Rhs) :
  maxlevel(Rhs.maxlevel), size(Rhs.size)
{
  if(Rhs.root != 0)
    {
      root = new Interval [size];
      assert(root);
      for(integer i=0; i<size; i++) root[i] = Rhs.root[i];
    }
  else root = 0;
}
/////////////////////////////////////////////////////////////////////////////
void ArrayTreeAper::DestroyTree()
{
  if(root != 0) delete [] root;
  root = 0;
  size = maxlevel = 0;
}

/////////////////////////////////////////////////////////////////////////////
const ArrayTreeAper &ArrayTreeAper::operator=(const ArrayTreeAper &Rhs)
{
  if(this != &Rhs)
    {
      DestroyTree();
      maxlevel = Rhs.maxlevel;
      size = Rhs.size;
      root = new Interval [size];
      assert(root);
      for(integer i=0; i<size; i++) root[i] = Rhs.root[i];
    }
  return (*this);
}

/////////////////////////////////////////////////////////////////////////////
Interval *ArrayTreeAper::block(const integer &L, const integer &B) const
{
  assert( 0<=L && L<=maxlevel && 0<=B && B<(1<<L) );
  return root + (1<<L) - 1 + B;
}

/////////////////////////////////////////////////////////////////////////////
Interval *ArrayTreeAper::left_child(const integer &L, const integer &B) const
{
   assert( 0<=L && L<maxlevel && 0<=B && B<(1<<L) );
   return root + (1<<(L+1)) - 1 + 2*B;                      // block(L+1, 2*B)
}

/////////////////////////////////////////////////////////////////////////////
Interval *ArrayTreeAper::right_child(const integer &L, const integer &B) const
{
  assert( 0<=L && L<maxlevel && 0<=B && B<(1<<L) );
  return root + (1<<(L+1)) + 2*B;                         // block(L+1, 2*B+1)
}

/////////////////////////////////////////////////////////////////////////////
void ArrayTreeAper::ZeroTree()
{
  if( root )
    {
      integer i,j;
      for(i=0; i<size; i++)
	{
	  if( root[i].origin )                         // if interval non-empty
	    {
	      for(j=root[i].beg; j<=root[i].end; j++)
		root[i].origin[j] = 0;
	    }
	}
    }
}



























































































































































































































