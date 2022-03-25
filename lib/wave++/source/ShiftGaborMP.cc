
#include "ShiftGaborMP.h"
#include <math.h>
#include <assert.h>

real_number RunShiftGaborMP(int max_iter, // maximal number of iterations 
		     real_number epsilon, // desired precision | Rf | < epsilon
		     const Interval &f, // signal to be approximated
		     // assumption : f sampled on integers starting with 0
		     // i.e.  f.beg must be 0. No constraints on f.length.
		     const Partition &Part, // partiton which defining
		     //dictionary
		     Interval &f_approx, // MP approximation of f
		     // f_approx is a linear combination of Gabors
		     Interval &Rf, // final residual: Rf = f - f_approx
		     // error which is returned equals | Rf |
		     std::vector <RealGabor> &G, // vector of Gabors chosen for
		     // f_approx
		     std::vector <real_number> & Gcoef // coeficinets in the linear
		     // combination, corresponding to Gabors in G
		     // f_approx = sum( Gcoef[i] * G[i] ), where number of i's
		     // depends on epsilon and max_iter
		    )
{
  integer_number dim = f.length;
  assert( f.beg == 0 && dim > 0);

  Interval I(0, dim-1);
  for(int i=0; i<dim; i++) I.origin[i] = i;
  
  //declare some temporary objects to receive outputs from getOptimalShiftGabor
  real_number coef=0.0, error=0.0;
  RealGabor Gtemp;
  
  // Initialize Rf and f_approx
  f_approx.Set(0, dim-1);
  Rf = f;    
  real_number *Gptr, *fptr, *Rfptr;
  for(int iter=0; iter < max_iter; iter++)
	{
	  getOptimalShiftGabor(Rf, Part, Gtemp, coef);
	  Gtemp.createSample(I); 
	  G.push_back(Gtemp); 
	  Gcoef.push_back(coef);
	  Gptr = Gtemp.Sample.origin;  fptr = f_approx.origin; 
	  Rfptr = Rf.origin;
	  error = 0;
	  for(integer_number t=0; t<dim; t++)
	    {
	      *fptr += (*Gptr) * coef;
	      (*Rfptr) -= (*Gptr) * coef;
	      error += (*Rfptr) * (*Rfptr);
	      ++Gptr;  ++fptr;  ++Rfptr;
	    }
	  error = sqrt(error);
	  if( error < epsilon )
	    {/*
	      cout << "Desired precision achieved in " << iter+1 
	      << " iterations. " << endl;  */
	      break;
	    }
	}
  return error;
}
//////////////////////////////////////////////////////////////////

void getOptimalShiftGabor(const Interval &f,  // signal
			  const Partition &Part,
			  RealGabor &G, // optimal Gabor, the one closest to f
			  real_number &coef   // <f, G>		 
		     )
{
  integer_number dim = f.length;
  integer_number c = dim / 2;  // midpoint or left midpoint of the interval [0, dim-1]
                        // depending on whether dim is odd or even
  integer_number t, ddu;  // ddu will hold (integer)du[j]
  real_number ddv;        // ddv will hold dv[j]
  integer_number lmshift = Part.lmu - c, rmshift = Part.rmu - c; // leftmost and rightmost
  // shifts with respect to the central position c
  real_number codv, sidv, covshift, sivshift; // codv will hold cos( ddv )
                                       // covshift will hold cos( v * shift )
  Interval CO, SI; // CO[t] will hold cos( t * v )

  integer_number  beg, end;
  Interval P, Q;
  real_number a, b, phase, Pnorm2, Qnorm2, PprodQ, a1, b1, product;
  real_number atemp, btemp, Pnorm2temp, Qnorm2temp, PprodQtemp;
  real_number *Pptr, *Qptr, *fptr, *COptr, *SIptr; 
  real_number covlms, sivlms, covdu, sivdu, codvlms, sidvlms, codvdu, sidvdu, temp;
  // covlms will hold cos(v * lmshift), codvlms will hold cos(ddv * lmshift)
  // covdu will hold cos( v * ddu ), codvdu will hold cos(ddv * ddu)
  coef = 0;
  for( integer_number j = Part.s.beg; j <= Part.s.end; j++ )
    {
      // set up
      ddv = Part.dv.origin[j];  
      ddu = (integer_number)(Part.du.origin[j]); // effectively floor 
    
      beg = maximum(c - 4 * (integer_number)(Part.s.origin[j]), c - dim);  
      end = minimum(c + 4 * (integer_number)(Part.s.origin[j]), c + dim);
      P.Set(beg, end);
      Q.Set(beg, end);
      CO.Set(beg, end);   
      SI.Set(beg, end);
      codv = cos( ddv );  sidv = sin( ddv ); // used for uodating CO, SI
      Pptr = P.origin + beg; 
      COptr = CO.origin + beg;  SIptr = SI.origin + beg;
      // do first elements separately
      *Pptr++ = g((real_number)(beg - c) / Part.s.origin[j]);
      *COptr++ = cos( beg * ddv );    *SIptr++ = sin( beg * ddv );
      for(t = beg + 1; t <= end; t++) 
	{
	  *Pptr++ = g((real_number)(t - c)/Part.s.origin[j]);
	  *COptr = *(COptr - 1) * codv - *(SIptr - 1) * sidv;
	  *SIptr = *(COptr - 1) * sidv + *(SIptr - 1) * codv;
	  COptr++;  SIptr++;
	}     
      codvlms = cos( ddv * lmshift ) ;  sidvlms = sin( ddv * lmshift );
      codvdu = cos( ddv * ddu );   sidvdu = sin( ddv * ddu );
      covlms = covdu = 1;  sivlms = sivdu = 0; 
      // by now everything is set up for v=0
      // Now do v=0 separately 
      for(integer_number shift = lmshift; shift <= rmshift; shift += ddu)
	{
	  // covshift = cos(v*shift);  sivshift = sin(v*shift)
	  beg = maximum(0, P.beg + shift);  
	  end = minimum(dim-1, P.end + shift);
	  // calculate just a and norm P
	  a = Pnorm2 = 0;
	  Pptr = P.origin + beg - shift;  
	  fptr = f.origin + beg;
	  for( t=beg; t<=end; t++)
	    {
	      Pnorm2 += (*Pptr) * (*Pptr);
	      a += (*fptr++) * (*Pptr++);
	    }
	  product = a / sqrt(Pnorm2);
	  phase = 0;
	  if ( absval(product) > absval(coef)  )
	    {
	      coef = product;
	      G.Set( Part.s.origin[j], c + shift, 0, phase );
	    }
	} // end of shift loop for case v = 0
      // Now do the rest of v's
      for(real_number v=ddv; v < Part.rmv; v += ddv)
	{ 
	  // update P, Q for this new value of v
	  for( t = P.beg; t <= P.end; t++)
	    {
	      temp = P.origin[t];
	      P.origin[t] = temp * CO.origin[t] - Q.origin[t] * SI.origin[t];
	      Q.origin[t] = temp * SI.origin[t] + Q.origin[t] * CO.origin[t];
	    }
	  // update covlms, sivlms , covdu, sivdu for this new value of v
	  temp = covlms;
	  covlms = temp * codvlms - sivlms * sidvlms;
	  sivlms = temp * sidvlms + sivlms * codvlms;
	  temp = covdu;
	  covdu = temp * codvdu - sivdu * sidvdu;
	  sivdu = temp * sidvdu + sivdu * codvdu;
	  // initialize covshift with cos( v * lmshift )   
	  covshift = covlms;  sivshift = sivlms;	
	  for(integer_number shift = lmshift; shift <= rmshift; shift += ddu)
	    {
	      // covshift = cos(v*shift);  sivshift = sin(v*shift)
	      beg = maximum(0, P.beg + shift);  
	      end = minimum(dim-1, P.end + shift);
	      // calculate a, b, a1, b1
	      atemp = btemp = PprodQtemp = Pnorm2temp = Qnorm2temp = 0;
	      Pptr = P.origin + beg - shift;  Qptr = Q.origin + beg - shift;
	      fptr = f.origin + beg;
	      for( t=beg; t<=end; t++)
		{
		  Pnorm2temp += (*Pptr) * (*Pptr);
		  Qnorm2temp += (*Qptr) * (*Qptr);
		  PprodQtemp += (*Pptr) * (*Qptr);
		  atemp += (*fptr) * (*Pptr++);
		  btemp += (*fptr++) * (*Qptr++);
		}
	      Pnorm2 = Pnorm2temp * covshift * covshift 
                   + Qnorm2temp * sivshift * sivshift
	           - 2.0 * PprodQtemp * covshift * sivshift;
	      Qnorm2 = Pnorm2temp * sivshift * sivshift 
	              + Qnorm2temp * covshift * covshift
		      + 2.0 * PprodQtemp * covshift * sivshift;
	      PprodQ = (Pnorm2temp - Qnorm2temp) * covshift * sivshift 
	           + PprodQtemp * (covshift * covshift - sivshift * sivshift);
	      a = atemp * covshift - btemp * sivshift;
	      b = atemp * sivshift + btemp * covshift;
	      a1 = a * Qnorm2 - b * PprodQ;
	      b1 = b * Pnorm2 - a * PprodQ;
	      if( Pnorm2 > 1.0e-16  && Qnorm2 > 1.0e-16 
		  && absval(a1) > 1.0e-16 ) 
		{	  
		  product= (a*a1 + b*b1)/
		    sqrt(a1*a1*Pnorm2 + b1*b1*Qnorm2 + 2*a1*b1*PprodQ);
		  phase = atan2(-b1, a1);
		}
	      else    // deal specialy with cases P=0 and Q=0
		{
		  if( Qnorm2 <= 1.0e-16 ) // Q = 0
		    {  product = a / sqrt(Pnorm2);  phase = 0.0;   }
		  else // P = 0 or a1 = 0
		    {  product = -b / sqrt(Qnorm2) ;   phase = M_PI * 0.5;  }
		}
	      // cout  << "product " << product << endl;
	      if ( absval(product) > absval(coef)  )
		{
		  coef = product;
		  G.Set( Part.s.origin[j], c + shift, v, phase );
		}
	      // update covshift and sivshift for next value of shift
	      temp = covshift;
	      covshift = temp * covdu - sivshift * sivdu;
	      sivshift = temp * sivdu + sivshift * covdu;
	    } // end of shift loop
	} // end of v loop
    } // end of s loop
}
//////////////////////////////////////////////////////////

      
