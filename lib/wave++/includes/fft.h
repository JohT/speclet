
#ifndef FFT_H
#define FFT_H

#include "common.h"


#define CRRMULRE(z, yre, yim)        ((z).Re*(yre)-(z).Im*(yim))
#define CRRMULIM(z, yre, yim)        ((z).Re*(yim)+(z).Im*(yre))
#define CCMULRE(z1, z2)        ((z1).Re*(z2).Re-(z1).Im*(z2).Im)
#define CCMULIM(z1, z2)        ((z1).Re*(z2).Im+(z1).Im*(z2).Re)

typedef struct {
  real_DWT Re;
  real_DWT Im;
} complex_DWT;

extern /*"C"*/ int
  br( 
     int n,			/* Nonnegative integer to bit-reverse. */
     int log2len);		/* Reverse this many bits.  */


extern /*"C"*/ void
  bitrevd(
	  void *out,		/* Pointer to base of the output array. */
	  const void *in,	/* Pointer to base of the input array. */
	  int   q,		/* # of index bits, or elements of `in[]'. */
	  int   size);		/* Number of bytes in an `in[]' element.  */

extern /*"C"*/ void
  bitrevi(
	  void *x,		/* Pointer to the input/output array. */
	  int   q,		/* # of index bits, or elements of `x[]'. */
	  int   size);		/* Number of bytes in an `x[]' element.  */

extern /*"C"*/ void
  fftproduct(                  /* Apply sparse matrix product. */
	     complex_DWT *f,       /* Input and output vector.     */
	     int q,            /* Length of `f[]' is N=1<<q.   */
	     const complex_DWT *W); /* Exponentials: `Omega(N/2)' */

extern /*"C"*/ complex_DWT * 
  fftomega(			/* Return exp(-PI*i*n/M), */
	   int M);		/* for n=0,1,2,...,|M|-1. */

extern /*"C"*/ void
  fftnormal(			/* Multiply `f[n].Re' and     */
	    complex_DWT *f,		/* `f[n].Im' by `1.0/sqrt(N), */
	    int N );		/* for n=0,1,2,...,N.         */

extern /*"C"*/ complex_DWT *
  dft(							/* Allocate, assign and return  */
      const complex_DWT *f,		/* a complex_DWT vector, the (1<<q) */
      int q);					/* point DFT of the input `f[]' */


#endif
