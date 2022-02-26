//*****************************************************************************
// QMF.cc                         Implementation of classes QMF, PQMF and GPQMF
//*****************************************************************************

#include "QMF.h"
#include "common.h"
#include <iostream.h>
#include <assert.h>
#include <stdlib.h>

//************************************ Q M F **********************************

QMF::QMF() : beg(0), end(-1), center(0), dev(0), coef(0) { }

QMF::QMF(const real *f, integer alpha, integer omega) 
{
 Set(f, alpha, omega);
}

QMF::QMF(const QMF &Rhs) : 
  beg(Rhs.beg), end(Rhs.end), center(Rhs.center), dev(Rhs.dev)
{
  if(Rhs.coef)
    {
      coef=new real [end-beg+1];
      assert(coef);
      coef -= beg;
      for(integer i=beg; i<=end; i++) coef[i]=Rhs.coef[i];
    }
  else coef=0;
}

QMF::~QMF()
{
  if(coef) delete [] (coef+beg);
  coef=0;
}

void QMF::Set(const real *f, integer alpha, integer omega)
{
  if(alpha > 0 || omega < 0)
    cout << "Standard convention QMF.beg <= 0 and QMF.end >= 0 not satisfied."
	 << " Be sure to use the proper ConvDec algorithm " << endl;
  center = dev = 0;
  coef = 0;
  beg = alpha;
  end = omega;
  assert ( (end-beg+1)>0 );
  coef = new real [end-beg+1];
  assert(coef);
  coef -= beg;
  real energy = 0;
  for(integer i=beg; i<=end; i++) 
    {
      coef[i] = f[i-beg];
      energy += coef[i]*coef[i];
      center += i*coef[i]*coef[i];
    }
  if(energy>0)
    {
      center /= energy;
      int sgn=-1;
      for(integer j=1; j<=(end-beg)/2; j++)
	{
	  real y=0;
	  for(integer k=j+beg; k<=end-j; k++) y += k*coef[k-j]*coef[k+j];
	  dev += sgn*y;
	  sgn = -sgn;
	}
      dev = 2 * absval(dev)/energy;
    }
}

//***************************** G P Q M F *************************************

GPQMF::GPQMF(): QMF()                     
{
  pcoef=0;
  pcoef_size=0;
}

GPQMF::GPQMF(const real *f, integer alpha, integer omega) 
{
  Set(f, alpha, omega);
}

GPQMF::GPQMF(const GPQMF &Rhs) : QMF::QMF(Rhs)
{
  int N = Rhs.end + 1 - Rhs.beg;
  N = ( ((N&1)==0) ? N : N+1 );
  pcoef_size = Rhs.pcoef_size;
  if(Rhs.pcoef)
    {
      pcoef = new real*[Rhs.pcoef_size];
      assert(pcoef);
      for(int m = 0; m < pcoef_size-1; m++)
	{
	  int index = pcoef_size-1-m;
	  N -= 2;
	  pcoef[index] = new real[N];
	  assert(pcoef[index]);
	  for(int i=0; i < N; i++)
	    pcoef[index][i] = Rhs.pcoef[index][i];
	}
      pcoef[0] = new real[1];
      assert(pcoef[0]);
      pcoef[0][0] = Rhs.pcoef[0][0];
    }
  else pcoef=0;
}

GPQMF::~GPQMF() 
{ 
  for(int i=0; i < pcoef_size; i++)
    {
      if(pcoef[i])
	delete[] pcoef[i];
      pcoef[i]=0;
    }
  if(pcoef)
    delete[] pcoef;
  pcoef = 0;
}

void GPQMF::Set(const real *f, integer alpha, integer omega)
{
  QMF::Set(f, alpha, omega);
  real *fq;
  int N = end+1-beg;
  N = ( ( (N&1)==0 ) ? N : N+1 );   // make sure N is even
  pcoef_size = N/2;
  pcoef = new real*[pcoef_size];
  assert(pcoef);
  for(int m = 0; m < pcoef_size-1; m++)
    {
      N -= 2;
      fq = new real [N];
      assert(fq);
      periodize(fq, N);
      pcoef[pcoef_size-1-m] = fq;
    }
  fq = new real [1];
  assert(fq);
  periodize(fq,1);
  pcoef[0] = fq;
}

void GPQMF::periodize(real *fq, integer q)
{
  real *pointer;
  int counter;
  int index = ( (beg%q + q)%q );
  int j = 0;
  while(j < q)
    {
      *(pointer = fq + index) = 0;
      for(counter=beg+j; counter<=end; counter +=q) *pointer += coef[counter];
      index = ( (index+1)%q );
      j++;
    }
}

//*********************************** P Q M F *********************************

PQMF::PQMF(): QMF()      
{
  pcoef=0;
  pcoef_size=0;
}

PQMF::PQMF(const real *f, integer alpha, integer omega) 
{
  Set(f, alpha, omega);
}

PQMF::PQMF(const PQMF &Rhs) : QMF::QMF(Rhs)
{
  int N;
  pcoef_size = Rhs.pcoef_size;
  if(Rhs.pcoef)
    {
      pcoef = new real*[Rhs.pcoef_size];
      assert(pcoef);
      for(int m = 0; m < pcoef_size; m++)
	{
	  N = 1 << m;
	  pcoef[m] = new real[N];
	  assert(pcoef[m]);
	  for(int i=0; i < N; i++)
	    pcoef[m][i] = Rhs.pcoef[m][i];
	}
    }
  else pcoef=0;
}
  
PQMF::~PQMF()
{ 
  for(int i=0; i < pcoef_size; i++)
    {
      if(pcoef[i])
	delete[] pcoef[i];
      pcoef[i]=0;
    }
  if(pcoef)
    delete[] pcoef;
  pcoef = 0;
}

void PQMF::Set(const real *f, integer alpha, integer omega) 
{
  QMF::Set(f, alpha, omega);
  real *fq;
  pcoef_size = 0;
  int N = end+1-beg;
  while( (1 << pcoef_size) < N ) pcoef_size++;

  pcoef = new real*[pcoef_size];

  assert(pcoef);
  for(integer m=0; m < pcoef_size; m++)
    {
      N = (1 << m);
      fq = new real [N];
      assert(fq);  
      periodize(fq, N);
      pcoef[m] = fq;
    }
}

void PQMF::periodize(real *fq, integer q)
{

  real *pointer;
  int counter;
  int index = ( (beg%q + q)%q );

  int j = 0;
  while(j < q)
    {
      *(pointer = fq + index) = 0;
      for(counter=beg+j; counter<=end; counter +=q) *pointer += coef[counter];
      index = ( (index+1)%q );
      j++;
    }
}
