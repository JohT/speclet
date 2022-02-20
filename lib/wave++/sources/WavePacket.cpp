//*****************************************************************************
// WavePacket.cc                      Implementations of analysis and synthesis
//*****************************************************************************

#include <iostream>
#include <assert.h>
#include "WavePacket.h"

/******************* Periodic case power of 2 ********************************/

void Analysis(const Interval &In, ArrayTreePer &A,
	      const PQMF &H, const PQMF &G, cdpType con_dec)
{
  assert(In.length == A.dim && A.origin);
  assert( (1<<(A.maxlevel)) == A.dim ); // check pow of 2 case
  for(integer i=0; i<A.dim; i++) A.origin[i] = In.origin[In.beg+i]; 
  integer L, B;
  for(L=0; L<A.maxlevel; L++)
    {
      for(B=0; B < (1<<L); B++)
	{
	  con_dec(A.block_start(L, B), A.left_child(L, B), A.block_length(L),
		  H);
	  con_dec(A.block_start(L, B), A.right_child(L, B), A.block_length(L),
		  G);
	}
    }
}

///////////////////////////////////////////////////////////////////////////////
void Synthesis(ArrayTreePer &A, Interval &Out,
	       const PQMF &H, const PQMF &G, cdpType adj_con_dec)
{
  assert(A.origin);
  assert( (1<<(A.maxlevel)) == A.dim ); // check pow of 2 case
  integer L, B;
  for(L=A.maxlevel - 1; L>=0; L--)       // go through all levels starting
    {                                    // from maxlevel-1 up to 0    
      for(B=0; B < (1<<L); B++)          // go through all blocks at level L
	{ 
	  adj_con_dec(A.left_child(L, B), A.block_start(L, B), 
		      A.block_length(L+1), H);
	  adj_con_dec(A.right_child(L, B), A.block_start(L, B), 
		      A.block_length(L+1), G);
	}
    }
  Out.Set(0, A.dim-1, A.origin);           // copy top row of A into Out
}

/************************* Periodic case even ********************************/

void Analysis(const Interval &In, ArrayTreePer &A,
	      const GPQMF &H, const GPQMF &G, cdgpType con_dec)
{
  assert(In.length == A.dim && A.origin);
  for(integer i=0; i<A.dim; i++) A.origin[i] = In.origin[In.beg+i]; 
  integer L, B;
  for(L=0; L<A.maxlevel; L++)
    {
      for(B=0; B < (1<<L); B++)
	{
	  con_dec(A.block_start(L, B), A.left_child(L, B), A.block_length(L),
		  H);
	  con_dec(A.block_start(L, B), A.right_child(L, B), A.block_length(L),
		  G);
	}
    }
}

///////////////////////////////////////////////////////////////////////////////
void Synthesis(ArrayTreePer &A, Interval &Out,
	       const GPQMF &H, const GPQMF &G, cdgpType adj_con_dec)
{
  assert(A.origin);
  integer L, B;
  for(L=A.maxlevel - 1; L>=0; L--)       // go through all levels starting
    {                                    // from maxlevel-1 up to 0    
      for(B=0; B < (1<<L); B++)          // go through all blocks at level L
	{ 
	  adj_con_dec(A.left_child(L, B), A.block_start(L, B), 
		      A.block_length(L+1), H);
	  adj_con_dec(A.right_child(L, B), A.block_start(L, B), 
		      A.block_length(L+1), G);
	}
    }
  Out.Set(0, A.dim-1, A.origin);           // copy top row of A into Out
}

/*********************** Aperiodic case **************************************/

void Analysis(const Interval &In, ArrayTreeAper &A,
	      const QMF &H, const QMF &G, cdaType con_dec)
{
  assert(A.root);
  A.root[0] = In;                                  // copy In into O'th node
  integer L, B;
  integer alpha, omega;
  for(L=0; L<A.maxlevel; L++)
    {
      for(B=0; B < (1<<L); B++)
	{
	  Interval *parent = A.block(L, B);
	  Interval *child = A.left_child(L, B);
	  alpha = ICH(parent->beg + H.beg);  omega = IFH(parent->end + H.end);
	  child->Set(alpha, omega);
	  con_dec(*parent, *child, H);
	  child++;       // child = A.right_child(L, B);
	  alpha = ICH(parent->beg + G.beg);  omega = IFH(parent->end + G.end);
	  child->Set(alpha, omega);
	  con_dec(*parent, *child, G);
	}
    }
}

///////////////////////////////////////////////////////////////////////////////
void Synthesis(ArrayTreeAper &A, Interval &Out,
	       const QMF &H, const QMF &G, cdaType adj_con_dec)
{
  assert(A.root);
  integer L, B;
  for(L=A.maxlevel - 1; L>=0; L--)    // go through all levels from P-1 up to 0
    {
      for(B=0; B < (1<<L); B++)       // go through all blocks at level L
	{ 
	  adj_con_dec(*(A.left_child(L, B)), *(A.block(L, B)), H);
	  adj_con_dec(*(A.right_child(L, B)), *(A.block(L, B)), G);
	}
    }
  Out = A.root[0];                                // copy top row of A into Out
}
