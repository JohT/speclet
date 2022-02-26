//*****************************************************************************
// Utility.cc                              Implementations of utility functions
//*****************************************************************************

#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <assert.h>
#include "Utility.h"

/***************************** Periodic case *********************************/

void ExtractHedge(HedgePer &H, const ArrayTreePer &A)
{
  assert( H.dim == A.dim );
  assert( H.maxlevel() <= A.maxlevel );
  integer j, shift=0;
  real *Hptr, *Aptr;
      for(integer i=0; i < H.num_of_levels; i++)
	{ 
	  Hptr = H.block_start(i);
	  Aptr = A.block_start(H.levels[i], 0) + shift;
	  for(j=0; j < H.block_length(i); j++) Hptr[j] = Aptr[j];
	  shift += H.block_length(i);
	}
}

///////////////////////////////////////////////////////////////////////////////
void SuperposeHedge(const HedgePer &H, ArrayTreePer &A)
{
  assert( H.dim == A.dim );
  assert( H.maxlevel() <= A.maxlevel );
   integer j, shift=0;
   real *Hptr, *Aptr;
      for(integer i=0; i < H.num_of_levels; i++)
	{ 
	  Hptr = H.block_start(i);
	  Aptr = A.block_start(H.levels[i], 0) + shift;
	  for(j=0; j < H.block_length(i); j++) Aptr[j] += Hptr[j] ;
	  shift += H.block_length(i);
	}
}

/////////////////////////////////////////////////////////////////////////////
void GetCosts(const ArrayTreePer &A, Tree &T, costFun F, const real &sigma)
{
  GetCostsHelp( A, &(T.root), F, sigma, 0, 0);
  T.maxlevel = A.maxlevel;
}

////////////////////////////////////////////////////////////////////////////
void GetCostsHelp(const ArrayTreePer &A,  Node<real> **ptr,
		  costFun F, const real &sigma,
		  const integer &L, const integer &B )    
{
  if( L <= A.maxlevel )
    {
      real cost = F( A.block_start(L, B), A.block_length(L), sigma, A.dim );
      (*ptr) = new Node<real>(cost, 0, 0);
      assert( *ptr );
      if( L < A.maxlevel )
	{
	  GetCostsHelp( A, &((*ptr)->left), F, sigma, L+1, B<<1 ); 
	  GetCostsHelp( A, &((*ptr)->right), F, sigma, L+1, (B<<1)|1 );    
	}
    }
}                                                         

///////////////////////////////////////////////////////////////////////////////
void BestBasis(HedgePer &H, const Tree &B)
{
  assert(B.root != 0);
  integer *Levs;
  Levs = new integer [1<<(B.maxlevel)];  // start with maximal space for levels
  integer n = 0;
  BestBasisHelp(Levs, n, B.root, 0);
  H.num_of_levels = n;
  H.levels = new integer [n];
  for (integer i=0; i<n; i++) H.levels[i] = Levs[i];
  delete [] Levs;
}                             

////////////////////////////////////////////////////////////////////////////
real BestBasisHelp(integer *Levs, integer &n, Node<real> *ptr, 
		   const integer &L)
{
  assert(ptr);
  if( !ptr->left && !ptr->right )    // if at the bottom row of the tree
    {
      Levs[n] = L;
      n++;
      return (ptr->content);
    }
  else
    {
      integer blocks = n;
      real cost=0;
      if(ptr->left)  cost += BestBasisHelp(Levs, n, ptr->left, L+1);
      if(ptr->right) cost += BestBasisHelp(Levs, n, ptr->right, L+1);

      if((ptr->content) > cost)  return cost;
      else
	{
	  n = blocks;
	  Levs[n] = L;
	  n++;
	  return (ptr->content);
	}
   }
}

////////////////////////////////////////////////////////////////////////////
void ExtractBestBasis(const ArrayTreePer &A, HedgePer &H, costFun F, 
		      const double &sigma)
{
  assert( A.origin );
  Tree B;
  GetCosts(A, B, F, sigma);
  H.dim = A.dim;
  BestBasis(H, B);
  H.origin = new real [H.dim];
  ExtractHedge(H, A);
}
////////////////////////////////////////////////////////////////////////////
/***************************** Aperiodic case ********************************/

void ExtractHedge(HedgeAper &H, const ArrayTreeAper &A)
{
  assert( H.maxlevel() <= A.maxlevel );
  H.root[0] = *(A.block(H.levels[0], 0));
  integer B=0;
  for(integer i=1; i<H.num_of_levels; i++)
    {    
      B = ((B + 1)<<(H.levels[i]))>>(H.levels[i-1]);
      H.root[i] = *(A.block(H.levels[i], B));
    } 
}

////////////////////////////////////////////////////////////////////////////
void SuperposeHedge(const HedgeAper &H, ArrayTreeAper &A)
{
  assert( H.maxlevel() <= A.maxlevel );
  *(A.block(H.levels[0], 0)) +=  H.root[0];
  integer B=0;
  for(integer i=1; i<H.num_of_levels; i++)
    {
      B = ((B + 1)<<(H.levels[i]))>>(H.levels[i-1]);
      *(A.block(H.levels[i], B)) += H.root[i];
    } 
}

////////////////////////////////////////////////////////////////////////////
void GetCosts(const ArrayTreeAper &A, Tree &B, costFun F,
	      const real &sigma)
{
  GetCostsHelp(A, &(B.root), F, sigma, 0, 0);
  B.maxlevel = A.maxlevel;
}  
                         
void GetCostsHelp(const ArrayTreeAper &A,  Node<real> **ptr,
		  costFun F, const real &sigma,
		  const integer &L, const integer &B )
{
  if( L <= A.maxlevel )
    {
      Interval *ip = A.block(L, B);
      real cost = F( ip->origin + ip->beg, ip->length, sigma, A.root->length);
      (*ptr) = new Node<real>(cost, 0, 0);
      assert( *ptr );
      if( L < A.maxlevel )
	{
	  GetCostsHelp(A, &((*ptr)->left), F, sigma, L+1, B<<1);
	  GetCostsHelp(A, &((*ptr)->right), F, sigma, L+1, (B<<1)|1);
	}
    }
}

////////////////////////////////////////////////////////////////////////////
void BestBasis(HedgeAper &H, const Tree &B)
{
  assert( B.root );
  integer *Levs;
  Levs = new integer [1<<(B.maxlevel)];  // start with maximal space for levels
  integer n=0;
  BestBasisHelp(Levs, n, B.root, 0);
  H.num_of_levels = n;
  H.levels = new integer [n];
  for (integer i=0; i<n; i++) H.levels[i] = Levs[i];
  delete [] Levs;
}                              

////////////////////////////////////////////////////////////////////////////
void ExtractBestBasis(const ArrayTreeAper &A, HedgeAper &H, costFun F, 
		      const double &sigma)
{
  assert( A.root );
  Tree B;
  GetCosts(A, B, F, sigma);
  BestBasis(H, B);
  H.root = new Interval [H.num_of_levels];
  ExtractHedge(H, A);
}
