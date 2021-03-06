//****************************************************************************
// Hedge.h                                             Definitions of classes
//                                                     HedgePer and HedgeAper
//****************************************************************************

#ifndef HEDGE_H
#define HEDGE_H
#include "common.h"
#include "Interval.h"

/********************** Hedge Periodic **************************************/

class HedgePer {                    
public:
  HedgePer() : dim(0), num_of_levels(0), levels(0), origin(0) { }       
            // default constructor, all empty

  HedgePer(const integer_number &DIM, const integer_number &n, const integer_number *Levs);   
            // constructor which sets dim=DIM, num_of_levels=n and 
            // levels sequence is set from Levs 
            // check that dim is divisible by 2^maxlevel() performed
            // assumption: Levs is defined as sequence of n integers
            // levels are checked for consistency, if O.K.
            // origin is allocated dim amount of  memory
  
  HedgePer(const HedgePer &);                   // copy constructor

  ~HedgePer() { DestroyHedge(); }               // destructor

  const HedgePer &operator=(const HedgePer &);  // overload =

  void DestroyHedge();                          // free all memory

  bool levels_consistent() const;               // checks if levels gives rise
                                                // to a proper hedge
  
  integer_number maxlevel() const;                     // return maximum of levels 

  integer_number block_length(const integer_number &i) const; // length of block 
                                                // corresponding to levels[i]

  real_number *block_start(const integer_number &i) const;    // pointer to begining of block
                                                // corresponding to levels[i]
  ////////////////////////////////////////////////////////////////////////////

  integer_number dim;                 // total length of origin=dim

  integer_number num_of_levels;       // length of levels sequence

  integer_number *levels;             // sequence of levels defining hedge

  real_number *origin;                // pointer to the begining of hedge's data
};
// end of HedgePer class definition

/************************* Hedge Aperiodic ***********************************/

class HedgeAper{                           
 public:
  HedgeAper() : num_of_levels(0), levels(0), root(0) { }   
            // default constructor, all empty
  
  HedgeAper(const integer_number &n, const integer_number *Levs);   
            // constructor which sets dim=d, num_of_levels=n and 
            // levels sequence is set from Levs 
            // assumption: Levs is defined as sequence of n integers
            // levels are checked for consistency, if O.K.
            // root is allocated n empty intervals                 
  
  HedgeAper(const HedgeAper &);                   // copy constructor

  ~HedgeAper() { DestroyHedge(); }                // destructor

  const HedgeAper &operator=(const HedgeAper &);  // overload =

  void DestroyHedge();                            // free all memory

  bool levels_consistent() const;                // checks if levels gives 
                                                 // rise to a proper hedge

  integer_number maxlevel() const;                      // return maximum of levels

  integer_number total() const {return num_of_levels;}  // InOut utility

  Interval *start() { return root;}                  // InOut utility
  ////////////////////////////////////////////////////////////////////////////

  integer_number num_of_levels;         // length of levels sequence

  integer_number *levels;               // sequence of levels defining hedge

  Interval *root;                // pointer to the first hedge's interval
};
// end of HedgeAper class definition
#endif

	  



