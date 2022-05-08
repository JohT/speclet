//*****************************************************************************
// QMF.h                             Definitions of classes QMF, PQMF and GPQMF
//*****************************************************************************

#ifndef QMF_H
#define QMF_H

#include "common.h"

class QMF {         // base class : aperiodic quadratic mirror filter
 public:
  QMF();                                             // default constructor
  QMF(const real_number *f, integer_number alpha, integer_number omega);  // construct QMF
  QMF(const QMF &);                                  // copy constructor
  virtual ~QMF();                                    // destructor
  virtual void Set(const real_number *f, integer_number alpha, integer_number omega);  
  integer_number beg;       // least index of coef array
  integer_number end;       // final index of coef array
  real_number center;       // center of energy of the filter
  real_number dev;          // deviation
  real_number *coef;        // array of coefficients : from coef[beg] to coef[end]
};

class GPQMF : public QMF { // derived class : periodized qmf
 public:                   // All even length periodizations 
  GPQMF();
  GPQMF(const real_number *f, integer_number alpha, integer_number omega);
  GPQMF(const GPQMF &);
  virtual ~GPQMF();
  virtual void Set(const real_number *f, integer_number alpha, integer_number omega);
  void periodize(real_number *fq, integer_number q);                    // utilty used by Set

  real_number **pcoef;                   // periodized coefficient array

  int pcoef_size;                // number of arrays of periodized coefficients
};

class PQMF : public QMF { // derived class : periodized quadratic mirror filter
 public:                  // periodizations of length a power of 2
  PQMF();
  PQMF(const real_number *f, integer_number alpha, integer_number omega);
  PQMF(const PQMF &);
  virtual ~PQMF();
  virtual void Set(const real_number *f, integer_number alpha, integer_number omega);
  void periodize(real_number *fq, integer_number q);                    // utilty used by Set

  real_number **pcoef;             // periodized coefficient array

  int pcoef_size;           // number of arrays of periodized coefficients
};

#endif
