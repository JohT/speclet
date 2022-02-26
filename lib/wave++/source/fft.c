#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "fft.h"

#define fftbitrev(out,in,q)  bitrevd(out,in,q,sizeof(complex))
#define fftbrinpl(x,q)       bitrevi(x,q,sizeof(complex))

extern "C" int
  br( 
     int n,			/* Nonnegative integer to bit-reverse. */
     int log2len)		/* Reverse this many bits.  */
{
  int u;

  assert(n>=0);
  assert(n<(1<<log2len));

  u = n&1;
  while( --log2len )
    {
      n >>= 1;
      u <<= 1;
      u += n&1;
    }
  return(u);
}

extern "C" void
  bitrevd(
	  void *out,		/* Pointer to base of the output array. */
	  const void *in,	/* Pointer to base of the input array. */
	  int   q,		/* # of index bits, or elements of `in[]'. */
	  int   size)		/* Number of bytes in an `in[]' element.  */
{
  int u, n;

  assert(in);
  assert(out);
  assert(q>=0);
  assert(size>0);

  for(n=0; n<(1<<q); n++)
    {
      u = br(n,q);
      memcpy( out+n*size, in+u*size, size );
    }
  return;
}

extern "C" void
  bitrevi(
	  void *x,		/* Pointer to the input/output array. */
	  int   q,		/* # of index bits, or elements of `x[]'. */
	  int   size)		/* Number of bytes in an `x[]' element.  */
{
  int u, n;
  void *temp, *xn, *xu;

  assert(x);
  assert(q>=0);
  assert(size>0);

  temp = malloc(size);
  for(n=0; n<(1<<q); n++)
    {
      u = br(n,q);
      if( u > n )
	{
	  xu  = x+u*size;
	  xn =  x+n*size;
	  memcpy( temp, xu, size );
	  memcpy( xu, xn, size );
	  memcpy( xn, temp, size );
	}
    }
  free(temp);
  return;
}

extern "C" void
  fftproduct(                   /* Apply sparse matrix product. */
	     complex *f,        /* Input and output vector.     */
	     int q,             /* Length of `f[]' is N=1<<q.   */
	     const complex *W)	/* Exponentials: `Omega(N/2)' */
{
  int b, j, k, N, N1, M;
  complex  *fptr1, *fptr2, tmp;
  const complex *Wptr;

  N  = 1<<q;
  for(k=q-1; k>=0; k--)
    {
      N1 = N>>k;		/* block size */
      M = N1>>1;		/* butterfly size */
      /* Each F_k has 2^{k} blocks E_{2M}, where 2M=2^{q-k},
       * and the matrix E_M is defined by:
       *	I_M   W_M
       *	I_M  -W_M
       */
      for( b=0; b<N; b+=N1 )
	{
	  /* Multiply `f[]' by a direct summand `E_{2M}':
	   *   We use a temporary variable `tmp' to allow
	   *     multiplication in place.
	   *   We keep 2 pointers `fptr1' and `fptr2' into
	   *     the `f' array and increment them across
	   *     blocks to avoid some arithmetic.
	   *   We use the identities
	   *
	   *	     W_M[j] = W_{N/2}[j*(N/2M)]
	   *
	   *     for j=0,1,2,... so we only need
	   *     the single array Omega(N/2).
	   */
	  fptr1 = f + b;
	  fptr2 = fptr1 + M;

	  /* Do the ones: */
	  tmp.Re = fptr2->Re;	/* Mult. f[M] by +/-W[0]= +/-1 */
	  tmp.Im = fptr2->Im;
	  fptr2->Re = fptr1->Re - tmp.Re;
	  fptr2->Im = fptr1->Im - tmp.Im;
	  fptr1->Re += tmp.Re;
	  fptr1->Im += tmp.Im;

	  /* Do the cosine/sine terms */
	  Wptr = W;
	  for( j=1; j<M; j++ )
	    {
	      ++fptr1; ++fptr2;
	      Wptr += (1<<k);	/* increment W by N/2M */
	      tmp.Re = CCMULRE(*fptr2, *Wptr ); 
	      tmp.Im = CCMULIM(*fptr2, *Wptr ); 
	      fptr2->Re = fptr1->Re - tmp.Re;
	      fptr2->Im = fptr1->Im - tmp.Im;
	      fptr1->Re += tmp.Re;
	      fptr1->Im += tmp.Im;
	    }
	}
    }
  return;
}

extern "C" complex * 
  fftomega(			/* Return exp(-M_PI*i*n/M), */
	   int M)		/* for n=0,1,2,...,|M|-1. */
{
  complex *W;
  double factor, theta;
  int n;

  factor = -M_PI/(double)M;

  M = abs(M);
  W = (complex *)malloc(M*sizeof(complex));

  theta = 0.0;
  for(n=0; n<M; n++)
    {
      W[n].Re = (real)cos(theta);
      W[n].Im = (real)sin(theta);
      theta += factor;
    }
  return(W);
}

extern "C" void
  fftnormal(			/* Multiply `f[n].Re' and     */
	    complex *f,		/* `f[n].Im' by `1.0/sqrt(N), */
	    int N )		/* for n=0,1,2,...,N.         */
{
  double norm;
  int n;

  norm = sqrt(1.0/(double)N);
  for(n=0; n<N; n++)
    {
      f[n].Re = norm * f[n].Re;
      f[n].Im = norm * f[n].Im;
    }
  return;
}

extern "C" complex *
  dft(				/* Allocate, assign and return  */
      const complex *f,		/* a complex vector, the (1<<q) */
      int q)			/* point DFT of the input `f[]' */
{
  int N, inverse;
  complex *W, *fhat;

  /* If q<0, then the inverse DFT is to be computed: */
  inverse = 0;
  if( q < 0 )
    {
      q = -q ;
      inverse = 1;
    } 
  N = 1<<q;			/* Compute length of `f[]'. */

  /* Allocate an output vector of same length, `N', as `f[]': */
  fhat = (complex *)calloc(N, sizeof(complex));
  assert(fhat!=0);		/* Test `calloc()' success. */

  /* Compute two trivial cases directly: */
  if( q == 0 )
    {
      fhat[0].Re = f[0].Re; 
      fhat[0].Im = f[0].Im; 
      return(fhat);
    }
  if( q == 1 )
    {
      fhat[0].Re = (f[0].Re + f[1].Re)*M_SQRT1_2; 
      fhat[0].Im = (f[0].Im + f[1].Im)*M_SQRT1_2; 
      fhat[1].Re = (f[0].Re - f[1].Re)*M_SQRT1_2;
      fhat[1].Im = (f[0].Im - f[1].Im)*M_SQRT1_2;
      return(fhat);
    }
  /* Use the FFT factorization for longer vectors `f[]' */

  /* Permute by bit-reversal `fhat[] = P f[]': */
  fftbitrev( fhat, f, q);
  
  /* Generate the complex exponential vector `Omega_{N/2}': */
  if(inverse)
    W = fftomega(-N/2);		/* conjugate, for inverse DFT */
  else
    W = fftomega( N/2);		/* for DFT */

  /* Apply sparse matrices `F_{q-1} F_{q-2}...F_1 F_0 fhat[]': */
  fftproduct( fhat, q, W );

  /* Normalize `fhat[]': divide by sqrt(N): */
  fftnormal( fhat, N );

  free(W);			/* Clean up */

  return(fhat);
}
