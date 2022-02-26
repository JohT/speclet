//*****************************************************************************
// QMF.h                             Definitions of classes QMF, PQMF and GPQMF
//*****************************************************************************

#ifndef QMF_H
#define QMF_H

#include "common.h"

class QMF {         // base class : aperiodic quadratic mirror filter
 public:
  QMF();                                             // default constructor
  QMF(const real *f, integer alpha, integer omega);  // construct QMF
  QMF(const QMF &);                                  // copy constructor
  virtual ~QMF();                                    // destructor
  virtual void Set(const real *f, integer alpha, integer omega);  
  integer beg;       // least index of coef array
  integer end;       // final index of coef array
  real center;       // center of energy of the filter
  real dev;          // deviation
  real *coef;        // array of coefficients : from coef[beg] to coef[end]
};

class GPQMF : public QMF { // derived class : periodized qmf
 public:                   // All even length periodizations 
  GPQMF();
  GPQMF(const real *f, integer alpha, integer omega);
  GPQMF(const GPQMF &);
  virtual ~GPQMF();
  virtual void Set(const real *f, integer alpha, integer omega);
  void periodize(real *fq, integer q);                    // utilty used by Set

  real **pcoef;                   // periodized coefficient array

  int pcoef_size;                // number of arrays of periodized coefficients
};

class PQMF : public QMF { // derived class : periodized quadratic mirror filter
 public:                  // periodizations of length a power of 2
  PQMF();
  PQMF(const real *f, integer alpha, integer omega);
  PQMF(const PQMF &);
  virtual ~PQMF();
  virtual void Set(const real *f, integer alpha, integer omega);
  void periodize(real *fq, integer q);                    // utilty used by Set

  real **pcoef;             // periodized coefficient array

  int pcoef_size;           // number of arrays of periodized coefficients
};

#endif
