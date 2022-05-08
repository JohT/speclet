#include "FFTGaborMP.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "fft.h"

// declare bunch of global variables. They get updated as you run
// RunFFTGaborMP
static real_number coef, product, a, b, a1, b1, v, phase, Pnorm2, Qnorm2, PprodQ;
static RealGabor Gtemp;
static integer_number s, u;

real_number RunFFTGaborMP(int max_iter, // maximal number of iterations 
		     real_number epsilon, // desired precision | Rf | < epsilon
		     const Interval &f, // signal to be approximated
		     // assumption : f sampled on integer_numbers starting with 0
		     // i.e.  f.beg must be 0. No constraints on f.length.
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
  assert( f.beg == 0 && dim > 0 && is_pow_of_2(dim));

  Interval I(0, dim-1);
  for(int i=0; i<dim; i++) I.origin[i] = i;
  
  //declare some temporary objects to receive outputs from getOptimalShiftGabor
  real_number error=0.0;
  
  // Initialize Rf and f_approx
  f_approx.Set(0, dim-1);
  Rf = f;    
  real_number *Gptr, *fptr, *Rfptr;
  for(int iter=0; iter < max_iter; iter++)
	{
	  getOptimalFFTGabor(Rf); // this call sets Gtemp to G closest to Rf
	  // and coef is set to <Rf, Gtemp>
	  Gtemp.createSample(I); 
	  G.push_back(Gtemp); // append vectors G and Gcoef
	  Gcoef.push_back(coef);
	  Gptr = Gtemp.Sample.origin;  fptr = f_approx.origin; 
	  Rfptr = Rf.origin;
	  // update f_approx by adding this latest Gtemp * coef
	  // also update Rf to new value: Rf = old Rf - Gtemp * coef
	  // calculate error as | new Rf |
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
	    {
	      std::cout << "Desired precision achieved in " << iter+1 
	      << " iterations. " << std::endl;  
	      break;
	    }
	}
  return error;
}
//////////////////////////////////////////////////////////////////

void getOptimalFFTGabor(const Interval &f)
{
  integer_number dim = f.length;
  integer_number m = Log2(dim);
  integer_number du;
  coef = product = 0;
  // case 8s > dim, i.e. s=dim=2^m, s=2^(m-1) and s=2^(m-2)
  for(s = dim; s >= (1<<(m-2)); s>>=1)
    {
      du = s>>1;  // s = 2^j , du = 2^(j-1), j = m, m-1, m-2
      for(u = 0; u < dim; u += du)
	proces1(dim, f.origin); // using FFT calculate a,b,...
      //and update product and coef for all Gabores with fixed s and u.
      // i.e., go thtough all frequences
    }
  // continue decreasing s through powers of 2 until s=1
  for(; s > 1; s >>= 1)
    {
      du = s>>1;
     
      //case : dim>= 8s and 0 <= u <= 4s
      for(u = 0; u <= 4*s; u += du)
	proces1(8*s, f.origin); // using FFT calculate a,b,..
      //and update product and coef for all Gabores with fixed s and u.
      // i.e., go thtough all frequences

      // case : dim >= 8s and 4s < u <= dim - 4s
      for(; u <= dim - 4*s; u += du) 
	proces2(8*s, 8*s - 1, f.origin + u - 4*s);// using FFT calculate a,b,..
      //and update product and coef for all Gabores with fixed s and u.
      // i.e., go thtough all frequences

       // case : dim >= 8s and dim - 4s < u
      for(; u < dim; u+= du)
	proces2(8*s, dim - u + 4*s -1, f.origin + u - 4*s);
    }
}
///////////////////////////////////////////////////////////////
void proces1(const integer_number &N, const real_number *fptr) 
{
  Interval X(0, N-1), Y(0, N-1), U(0, N-1), V(0, N-1);
 
  real_number temp;
  real_number *Xptr = X.origin, *Yptr = Y.origin;
  for(integer_number t=0; t < N; t++)
    {
      temp = g((real_number)(t-u)/(real_number)s);
      *Xptr++ = (*fptr++) * temp;
      *Yptr++ = temp * temp;
    }

  complex_number *E1, *E2, *O1, *O2;
  E1 = (complex_number *) calloc(N, sizeof(complex_number));
  assert(E1);
  E2 = (complex_number *) calloc(N, sizeof(complex_number));
  assert(E2);
  
  for(integer_number t=0; t<N; t++)
    {
      E1[t].Re = X.origin[t];
      E1[t].Im = E2[t].Im = 0 ;
      E2[t].Re = Y.origin[t];
    }
  O1 = dft(E1, Log2(N));
  O2 = dft(E2, Log2(N));
  
  real_number sq = sqrt(N);
  for(integer_number t=0; t<N; t++)
    {
      X.origin[t] = O1[t].Re * sq;
      U.origin[t] = O1[t].Im * sq;
      Y.origin[t] = O2[t].Re * sq;
      V.origin[t] = O2[t].Im * sq;
    }
  
  free(E1);  free(E2);
  free(O1); free(O2);
  
   
  real_number c = Y.origin[0];
  for(integer_number k = 0; k < (N>>1); k++)
    {
      Pnorm2 = (c + Y.origin[2*k]) / 2;
      Qnorm2 = (c - Y.origin[2*k]) / 2;
      PprodQ = - V.origin[2*k] / 2;
      a = X.origin[k];
      b = -U.origin[k];
      v = 2 * M_PI * k / N;
      update();

      a = X.origin[k + (N>>1)];
      b = - U.origin[k +(N>>1)];
      v += M_PI;
      update();
    } 
}
/////////////////////////////////////////////////////////////////////////////
void proces2(const integer_number &N, const integer_number &stop, const real_number *fptr)  
{
  Interval X(0, N-1);
  Interval Y(0, N-1);
  Interval U(0, N-1), V(0, N-1);
  real_number temp;
  real_number *Xptr = X.origin, *Yptr = Y.origin;
  integer_number d = u - 4*s;
  for(integer_number t=0; t <= stop; t++)
    {
      temp = g((real_number)(t-4*s)/(real_number)s);
      *Xptr++ = (*fptr++) * temp;
      *Yptr++ = temp * temp;
    }
  
  complex_number *E1, *E2, *O1, *O2;
  E1 = (complex_number *) calloc(N, sizeof(complex_number));
  assert(E1);
  E2 = (complex_number *) calloc(N, sizeof(complex_number));
  assert(E2);
  
  for(integer_number t=0; t<N; t++)
    {
      E1[t].Re = X.origin[t];
      E1[t].Im = E2[t].Im = 0 ;
      E2[t].Re = Y.origin[t];
    }
  O1 = dft(E1, Log2(N));
  O2 = dft(E2, Log2(N));
  
  real_number sq = sqrt(N);
  for(integer_number t=0; t<N; t++)
    {
      X.origin[t] = O1[t].Re * sq;
      U.origin[t] = O1[t].Im * sq;
      Y.origin[t] = O2[t].Re * sq;
      V.origin[t] = O2[t].Im * sq;
    }
  
  free(E1);  free(E2);
  free(O1); free(O2);
  
  real_number c = Y.origin[0];
  real_number alpha = 2 * M_PI * d / N ;
  real_number cosal, cos2al, sinal, sin2al;
  cosal = cos2al = 1;
  sinal = sin2al = 0;
  real_number co = cos(alpha), si = sin(alpha);
  real_number x, y, uu, vv;
  for(integer_number k = 0; k < (N>>1); k++)
    {
      y = Y.origin[2*k];  vv = V.origin[2*k];
      Pnorm2 = (c + y * cos2al + vv * sin2al) / 2;
      Qnorm2 = (c - y * cos2al - vv * sin2al) / 2;
      PprodQ = (y * sin2al - vv * cos2al) / 2;
      x = X.origin[k];  uu = U.origin[k];
      a = x * cosal + uu * sinal;
      b = x * sinal - uu * cosal;
      v = 2 * M_PI * k / N;
      update();
    
      x = X.origin[k + (N>>1)];  uu = U.origin[k + (N>>1)];
      a = x * cosal + uu * sinal;
      b = x * sinal - uu * cosal;
      if( (d>>1)<<1 != d) { a *= -1.0;  b *= -1.0;} // not very elegant!
      v += M_PI;
      update();

      // update cosines and sines
      temp = cosal;
      cosal = temp * co - sinal * si;
      sinal = temp * si + sinal * co;
      cos2al = cosal * cosal - sinal * sinal;
      sin2al = 2 * sinal * cosal;
    }
}
///////////////////////////////////////////////////////////////////////////
void update()
{
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
       Gtemp.Set( s, u, v, phase );
     }
}
