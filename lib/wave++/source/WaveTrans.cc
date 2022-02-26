//*****************************************************************************
// WaveTrans.cc                  Implementation of Discrete wavelet transforms
//*****************************************************************************

#include "WaveTrans.h"
#include "ConvDec.h"
#include "QMF.h"
#include "Interval.h"
#include "common.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream.h>

/*******************  Periodic case pow of 2 ********************************/

void WaveTrans(const Interval &In, Interval &Out, 
	       const PQMF &H, const PQMF &G, cdpType con_dec)
{
  Interval Work(0, In.end);      // all elemnts of Work are 0's
  WaveTrans(In, Out, Work, H, G, con_dec);
}

///////////////////////////////////////////////////////////////////////////////
void WaveTrans(const Interval &In, Interval &Out, Interval &Work, 
	       const PQMF &H, const PQMF &G, cdpType con_dec)
{
  assert( In.beg == 0 && Out.beg <= 0 && Out.end >= In.end);
  assert( is_pow_of_2(In.length) );
  real *in=In.origin, *out=Out.origin, *work=Work.origin;
  integer k=In.length;
  while( k > 1) 
    {
      con_dec(in, out + k/2, k, G);
      con_dec(in, work + k/2, k, H);
      k /= 2;
      in = work + k;
    }
  out[0] += in[0];
}

///////////////////////////////////////////////////////////////////////////////
void InvWaveTrans(const Interval &In, Interval &Out,
		  const PQMF &H, const PQMF &G, cdpType adj_con_dec)  
{
  Interval Work(0, In.end);
  InvWaveTrans(In, Out, Work, H, G, adj_con_dec);
}

/////////////////////////////////////////////////////////////////////////////
void InvWaveTrans(const Interval &In, Interval &Out, Interval &Work,
		  const PQMF &H, const PQMF &G, cdpType adj_con_dec)
{
  assert( In.beg == 0 && Out.beg <= 0 && Out.end >= In.end);
  assert( is_pow_of_2(In.length) );
  real *in=In.origin, *out=Out.origin, *work=Work.origin;
  integer n=In.length;
  if( n > 1 )
    {
      integer k=1;
      work += k;
      work[0] = in[0];
      in += 1;
      while( k < n/2 )
	{
	  adj_con_dec(work, work + k, k, H);
	  adj_con_dec(in, work + k, k, G);
	  work += k;
	  in += k;
	  k *= 2;
	}
      adj_con_dec(work, out, k, H);
      adj_con_dec(in, out, k, G);
    }
  else *out += *in;
}

/****************************** Periodic case  m*2^L *************************/

void WaveTrans(const Interval &In, Interval &Out, integer L,
	       const GPQMF &H, const GPQMF &G, cdgpType con_dec)
{
  Interval Work(0, In.beg);      // all elemnts of Work are 0's
  WaveTrans(In, Out, Work, L, H, G, con_dec);
}

///////////////////////////////////////////////////////////////////////////////
void WaveTrans(const Interval &In, Interval &Out, Interval &Work, integer L,
	       const GPQMF &H, const GPQMF &G, cdgpType con_dec)
{
  integer k=In.length;
  assert( In.beg == 0 && Out.beg <= 0 && Out.end >= In.end);
  assert( ((k>>L)<<L) == k );
  real *in=In.origin, *out=Out.origin, *work=Work.origin; 
  while( L-- > 0 )
    {
      con_dec(in, out + k/2, k, G);
      con_dec(in, work + k/2, k, H);
      k /= 2;
      in = work + k;
    }
  while( k-- > 0 ) *out++ += *in++;
}

//////////////////////////////////////////////////////////////////////////////
void InvWaveTrans(const Interval &In, Interval &Out, integer L,
		  const GPQMF &H, const GPQMF &G, cdgpType adj_con_dec)
{
  Interval Work(0, In.end);
  InvWaveTrans(In, Out, Work, L, H, G, adj_con_dec);
}

//////////////////////////////////////////////////////////////////////////////
void InvWaveTrans(const Interval &In, Interval &Out, Interval &Work, integer L,
		  const GPQMF &H, const GPQMF &G, cdgpType adj_con_dec)
{
  assert(L>0);
  integer k, n=In.length;
  assert( In.beg == 0 && Out.beg <= 0 && Out.end >= In.end);
  assert( ((n>>L)<<L) == n );
  real *in=In.origin, *out=Out.origin, *work=Work.origin; 
  if( L > 0 )
    {
      k = n>>L;
      work += k;
      for(integer j=0; j<k; j++)  work[j] = in [j];
      in += k;
      while( k < n/2 )
	{
	  adj_con_dec(work, work + k, k, H);
	  adj_con_dec(in, work + k, k, G);
	  work += k;
	  in += k;
	  k *= 2;
	}
      adj_con_dec(work, out, k, H);
      adj_con_dec(in, out, k, G);
    }
  else Out += In;
}

/**************************  Aperiodic  **************************************/

void WaveTrans(const Interval &In, Interval *Out, integer L,
	       const QMF &H, const QMF &G, cdaType con_dec)
{
  assert(L>0);
  Interval *Work = new Interval[L];
  WaveTrans(In, Out, Work, L, H, G, con_dec);
  delete [] Work;
}

///////////////////////////////////////////////////////////////////////////////
void WaveTrans(const Interval &In, Interval *Out, Interval *Work,
	       integer L, const QMF &H, const QMF &G, cdaType con_dec)
{
  assert(L > 0);
  Interval *Temp;
  Temp = const_cast<Interval *> (&In);
  for(integer k=L-1; k>=0; k--)
    {
      Work[k].Set(ICH((Temp->beg)+H.beg), IFH((Temp->end)+H.end));  // all 0's
      Out[k+1].Set(ICH((Temp->beg)+G.beg), IFH((Temp->end)+G.end));  // all 0's
      con_dec(*Temp, Work[k], H);
      con_dec(*Temp, Out[k+1], G);
      Temp = Work + k;
    }
  Out[0] = Work[0];  
}

///////////////////////////////////////////////////////////////////////////////
void InvWaveTrans(const Interval *In, Interval &Out, integer L,
		  const QMF &H, const QMF &G, cdaType adj_con_dec)
{
  assert(L>0);
  Interval *Work = new Interval[L];
  InvWaveTrans(In, Out, Work, L, H, G, adj_con_dec);
  delete [] Work;
}

///////////////////////////////////////////////////////////////////////////////
void InvWaveTrans(const Interval *In, Interval &Out, Interval *Work, 
		  integer L, const QMF &H, const QMF &G, cdaType adj_con_dec)
{
  assert(L>0);
  Work[0] = In[0];
  integer alpha, omega;
  for(integer k=1; k<L; k++)
    {
      alpha = minimum( LEAST(Work[k-1], H), LEAST(In[k], G) );
      omega = maximum( FINAL(Work[k-1], H), FINAL(In[k], G) ); 
      // alpha = In[k+1].beg;
      // omega = In[k+1].end;
      Work[k].Set(alpha, omega);   // all elts set to 0
      adj_con_dec(Work[k-1], Work[k], H);           // Work[k] = H*Work[k-1]
      adj_con_dec(In[k], Work[k], G); // by superposition Work[k] += G*In[k]
    }
  alpha = minimum( LEAST(Work[L-1], H), LEAST(In[L], G) );
  omega = maximum( FINAL(Work[L-1], H), FINAL(In[L], G) );
  if(Out.origin == 0)   // do this if Out was not set, i.e. we have no
                       // knowledge about size of the recoverd signal
    {
      alpha = minimum( LEAST(Work[L-1], H), LEAST(In[L], G) );
      omega = maximum( FINAL(Work[L-1], H), FINAL(In[L], G) );
      Out.Set(alpha, omega); 
    }
  adj_con_dec(Work[L-1], Out, H);
  adj_con_dec(In[L], Out, G);
}





