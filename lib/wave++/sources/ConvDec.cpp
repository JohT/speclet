//*****************************************************************************
// Convdec.cc                 Implementation of convolution-decimation routines
//*****************************************************************************

#include "ConvDec.h"
#include "common.h"
#include <iostream>
#include <assert.h>

void ConvDecPer( const real_DWT *In, real_DWT *Out, integer q, const PQMF &F )
{
  int j, offset, i = 0;
  real_DWT *In_end, *In_End, *Coef;
  real_DWT *Initial = const_cast<real_DWT *>(In) - F.beg; // Input array offset to
                                              // match the coefficients
  real_DWT *Final = const_cast<real_DWT *>(In) + q - 1; // End of the input array
  real_DWT  *CoefBegin = F.coef + F.beg; // Actual beginning of coefficient array

  if (q > (F.end - F.beg)) // Long input array
    {
      while (i < F.end)
	{
	  j = F.beg;
	  In_end = Initial + i;
	  In_End = Final;
	  Coef = CoefBegin;
	 
	  while( j <= i)
	    {
	      *Out += (*Coef++)*(*In_end--);
	      j++;
	    }
	  while (j <= F.end )
	    {
	      *Out += (*Coef++)*(*In_End--);
	      j++;
	    }
	  Out++;
	  i += 2;
	}      
      while(i < q + F.beg)
	{
	  j = F.beg;
	  Coef = CoefBegin;
	  In_end = Initial + i;
	  while (j <= F.end)
	    {
	      *Out += (*Coef++)*(*In_end--);
	      j++;
	    }
	  Out++;
	  i += 2;
	}
      while(i < q)
	{
	  offset = i-q;
	  j = F.beg;
	  Coef = CoefBegin;
	  In_end = Initial + offset;
	  In_End = Final;	  
	  while(j <= offset)
	    {
	      *Out += (*Coef++)*(*In_end--);
	      j++;
	    }
	  while(j <= F.end)
	    {
	      *Out += (*Coef++)*(*In_End--);
	      j++;
	    }
	  Out++;
	  i += 2;
	}
    }
  else // Short input case using periodized coefficients
    {
      int Q = Log2(q); 
      for ( ; i < q; i += 2)
	{
	  j = 0;
	  Coef = F.pcoef[Q];
	  In_end = const_cast<real_DWT *>(In) + i;
	  
	  while (j <= i)
	    {
	      *Out += (*Coef++)*(*In_end--);
	      j++;
	    }
	  In_End = Final;
	  while (j < q)
	    {
	      *Out += (*Coef++)*(*In_End--);
	      j++;
	    }
	  Out++;
	}
    }
}
///////////////////////////////////////////////////////////////////
void ConvDecPer( const real_DWT *In, real_DWT *Out, integer q, const GPQMF &F )
{
  int j, offset, i = 0;
  real_DWT *In_end, *In_End, *Coef;
  real_DWT *Initial = const_cast<real_DWT *>(In) - F.beg; // Input array offset to match
                                                // the coefficients
  real_DWT *Final = const_cast<real_DWT *>(In) + q - 1; // End of the input array
  real_DWT  *CoefBegin = F.coef + F.beg; // Actual beginning of coefficient array

  if (q > (F.end - F.beg)) // Long input array
    {
      while (i < F.end)
	{
	  j = F.beg;
	  In_end = Initial + i;
	  In_End = Final;
	  Coef = CoefBegin;
	 
	  while( j <= i)
	    {
	      *Out += (*Coef++)*(*In_end--);
	      j++;
	    }
	  while (j <= F.end )
	    {
	      *Out += (*Coef++)*(*In_End--);
	      j++;
	    }
	  Out++;
	  i += 2;
	}      
      while(i < q + F.beg)
	{
	  j = F.beg;
	  Coef = CoefBegin;
	  In_end = Initial + i;
	  while (j <= F.end)
	    {
	      *Out += (*Coef++)*(*In_end--);
	      j++;
	    }
	  Out++;
	  i += 2;
	}
      while(i < q)
	{
	  offset = i-q;
	  j = F.beg;
	  Coef = CoefBegin;
	  In_end = Initial + offset;
	  In_End = Final;	  
	  while(j <= offset)
	    {
	      *Out += (*Coef++)*(*In_end--);
	      j++;
	    }
	  while(j <= F.end)
	    {
	      *Out += (*Coef++)*(*In_End--);
	      j++;
	    }
	  Out++;
	  i += 2;
	}
    }
  else // Short input case using periodized coefficients
    {
      int Q = q/2; 
      for ( ; i < q; i += 2)
	{
	  j = 0;
	  Coef = F.pcoef[Q];
	  In_end = const_cast<real_DWT *>(In) + i;
	  
	  while (j <= i)
	    {
	      *Out += (*Coef++)*(*In_end--);
	      j++;
	    }
	  In_End = Final;
	  while (j < q)
	    {
	      *Out += (*Coef++)*(*In_End--);
	      j++;
	    }
	  Out++;
	}
    }
}
///////////////////////////////////////////////////////////////////
void ConvDecV2Per(const real_DWT* In, real_DWT* Out, integer q, const PQMF &F)
{
  int i, j, q1 = q-1, q2i;
  real_DWT *Coef;

  if( q > (F.end - F.beg) )
    {
      for (i = 0; i < q; i+=2)
	{
	  Coef = F.coef + F.beg;
	  j = F.beg;
	  q2i = q + i;
	  while( j <= F.end)
	    {
	      *Out += (*Coef++)*( In[(q2i - j) & q1] );
	      j++;
	    }
	  Out++;
	}
    }
  else
    {
      int Q = Log2(q);
      for (i = 0; i < q; i+=2)
	{
	  Coef = F.pcoef[Q];
	  j = 0;
	  q2i = q + i;
	  while( j < q)
	    {
	      *Out += (*Coef++)*( In[(q2i - j) & q1] );
	      j++;
	    }
	  Out++;
	}
    }
}
///////////////////////////////////////////////////////////////////
void ConvDecV2Per(const real_DWT* In, real_DWT* Out, integer q, const GPQMF &F)
{
  int i, j, q1 = q-1, q2i;
  real_DWT *Coef;

  if( q > (F.end - F.beg) )
    {
      for (i = 0; i < q; i+=2)
	{
	  Coef = F.coef + F.beg;
	  j = F.beg;
	  q2i = q + i;
	  while( j <= F.end)
	    {
	      *Out += (*Coef++)*( In[(q2i - j) & q1] );
	      j++;
	    }
	  Out++;
	}
    }
  else
    {
      int Q = q/2;
      for (i = 0; i < q; i+=2)
	{
	  Coef = F.pcoef[Q];
	  j = 0;
	  q2i = q + i;
	  while( j < q)
	    {
	      *Out += (*Coef++)*( In[(q2i - j) & q1] );
	      j++;
	    }
	  Out++;
	}
    }
}

///////////////////////////////////////////////////////////////
void AdjConvDecPer( const real_DWT *In, real_DWT *Out, integer q2, const PQMF &F )
{
  int j, offset, i = 0, q = 2*q2;
  real_DWT *Out_end, *Out_End, *Coef;
  real_DWT *Initial = Out - F.beg; // Output array offset to match the coefficients
  real_DWT *Final = Out + q - 1; // End of the Output array
  real_DWT  *CoefBegin = F.coef + F.beg; // Actual beginning of coefficient array

  if (q > (F.end - F.beg)) // Long input array
    {
      while (i < F.end)
	{
	  j = F.beg;
	  Out_end = Initial + i;
	  Out_End = Final;
	  Coef = CoefBegin;
	 
	  while( j <= i)
	    {
	      (*Out_end--) += (*Coef++)*(*In);
	      j++;
	    }
	  while (j <= F.end )
	    {
	      (*Out_End--) += (*Coef++)*(*In);
	      j++;
	    }
	  In++;
	  i += 2;
	}      
      while(i < q + F.beg)
	{
	  j = F.beg;
	  Coef = CoefBegin;
	  Out_end = Initial + i;
	  while (j <= F.end)
	    {
	      (*Out_end--) += (*Coef++)*(*In);
	      j++;
	    }
	  In++;
	  i += 2;
	}
      while(i < q)
	{
	  offset = i-q;
	  j = F.beg;
	  Coef = CoefBegin;
	  Out_end = Initial + offset;
	  Out_End = Final;	  
	  while(j <= offset)
	    {
	      (*Out_end--) += (*Coef++)*(*In);
	      j++;
	    }
	  while(j <= F.end)
	    {
	      (*Out_End--) += (*Coef++)*(*In);
	      j++;
	    }
	  In++;
	  i += 2;
	}
    }
  else // Short input case using periodized coefficients
    {
      int Q = Log2(q); 
      for ( ; i < q; i += 2)
	{
	  j = 0;
	  Coef = F.pcoef[Q];
	  Out_end = Out + i;
	  
	  while (j <= i)
	    {
	      (*Out_end--) += (*Coef++)*(*In);
	      j++;
	    }
	  Out_End = Final;
	  while (j < q)
	    {
	      (*Out_End--) += (*Coef++)*(*In);
	      j++;
	    }
	  In++;
	}
    }
}
///////////////////////////////////////////////////////////////
void AdjConvDecPer( const real_DWT *In, real_DWT *Out, integer q2, const GPQMF &F )
{
  int j, offset, i = 0, q = 2*q2;
  real_DWT *Out_end, *Out_End, *Coef;
  real_DWT *Initial = Out - F.beg; // Output array offset to match the coefficients
  real_DWT *Final = Out + q - 1; // End of the Output array
  real_DWT  *CoefBegin = F.coef + F.beg; // Actual beginning of coefficient array

  if (q > (F.end - F.beg)) // Long input array
    {
      while (i < F.end)
	{
	  j = F.beg;
	  Out_end = Initial + i;
	  Out_End = Final;
	  Coef = CoefBegin;
	 
	  while( j <= i)
	    {
	      (*Out_end--) += (*Coef++)*(*In);
	      j++;
	    }
	  while (j <= F.end )
	    {
	      (*Out_End--) += (*Coef++)*(*In);
	      j++;
	    }
	  In++;
	  i += 2;
	}      
      while(i < q + F.beg)
	{
	  j = F.beg;
	  Coef = CoefBegin;
	  Out_end = Initial + i;
	  while (j <= F.end)
	    {
	      (*Out_end--) += (*Coef++)*(*In);
	      j++;
	    }
	  In++;
	  i += 2;
	}
      while(i < q)
	{
	  offset = i-q;
	  j = F.beg;
	  Coef = CoefBegin;
	  Out_end = Initial + offset;
	  Out_End = Final;	  
	  while(j <= offset)
	    {
	      (*Out_end--) += (*Coef++)*(*In);
	      j++;
	    }
	  while(j <= F.end)
	    {
	      (*Out_End--) += (*Coef++)*(*In);
	      j++;
	    }
	  In++;
	  i += 2;
	}
    }
  else // Short input case using periodized coefficients
    {
      int Q = q/2; 
      for ( ; i < q; i += 2)
	{
	  j = 0;
	  Coef = F.pcoef[Q];
	  Out_end = Out + i;
	  
	  while (j <= i)
	    {
	      (*Out_end--) += (*Coef++)*(*In);
	      j++;
	    }
	  Out_End = Final;
	  while (j < q)
	    {
	      (*Out_End--) += (*Coef++)*(*In);
	      j++;
	    }
	  In++;
	}
    }
}
///////////////////////////////////////////////////////////////////
void AdjConvDecV2Per(const real_DWT* In, real_DWT* Out, integer q2, const PQMF &F)
{
  int i, j, q = 2*q2, q1 = q-1, q2i;
  real_DWT *Coef;

  if( q > (F.end - F.beg) )
    {
      for (i = 0; i < q; i+=2)
	{
	  Coef = F.coef + F.beg;
	  j = F.beg;
	  q2i = q + i;
	  while( j <= F.end)
	    {
	      Out[(q2i - j) & q1] += (*Coef++)*( *In );
	      j++;
	    }
	  In++;
	}
    }
  else
    {
      int Q = Log2(q);
      for (i = 0; i < q; i+=2)
	{
	  Coef = F.pcoef[Q];
	  j = 0;
	  q2i = q + i;
	  while( j < q)
	    {
	      Out[(q2i - j) & q1] += (*Coef++)*( *In );
	      j++;
	    }
	  In++;
	}
    }
}
///////////////////////////////////////////////////////////////////
void AdjConvDecV2Per(const real_DWT* In, real_DWT* Out, integer q2, const GPQMF &F)
{
  int i, j, q = 2*q2, q1 = q-1, q2i;
  real_DWT *Coef;

  if( q > (F.end - F.beg) )
    {
      for (i = 0; i < q; i+=2)
	{
	  Coef = F.coef + F.beg;
	  j = F.beg;
	  q2i = q + i;
	  while( j <= F.end)
	    {
	      Out[(q2i - j) & q1] += (*Coef++)*( *In );
	      j++;
	    }
	  In++;
	}
    }
  else
    {
      int Q = q/2;
      for (i = 0; i < q; i+=2)
	{
	  Coef = F.pcoef[Q];
	  j = 0;
	  q2i = q + i;
	  while( j < q)
	    {
	      Out[(q2i - j) & q1] += (*Coef++)*( *In );
	      j++;
	    }
	  In++;
	}
    }
}
///////////////////////////////////////////////////////////////
void ConvDecAper(const Interval &In, Interval &Out, const QMF &F)
{
  integer alpha=ICH(In.beg + F.beg), omega=IFH(In.end + F.end);
  assert(Out.beg <= alpha);
  assert(Out.end >= omega );
  integer i, Begin, End, j;
  integer i2;
  for(i=alpha; i<=omega; i++) 
    { 
      i2 = 2 * i;
      Begin = maximum( F.beg, i2-In.end);
      End = minimum( F.end, i2-In.beg);
      for(j=Begin; j<=End; j++)  
	Out.origin[i] += F.coef[j] * In.origin[i2-j];
    }
}
//////////////////////////////////////////////////////////////////	
void AdjConvDecAper(const Interval &In, Interval &Out, const QMF &F)
{
  integer j = Out.beg, Begin, End, i;
  while(j <= Out.end)
    {
      Begin = maximum( In.beg, ICH(j+F.beg) );
      End = minimum( In.end, IFH(j+F.end) );
      for(i=Begin; i<=End; i++) 
	Out.origin[j] += In.origin[i] * F.coef[2*i-j];
      j++;
    }
}
