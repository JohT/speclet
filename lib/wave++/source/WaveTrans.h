//*****************************************************************************
// WaveTrans.h                       Prototypes of discrete wavelet transforms
//*****************************************************************************

#ifndef WAVETRANS_H
#define WAVETRANS_H

#include "common.h"
#include "Interval.h"
#include "ConvDec.h"
#include "QMF.h"

typedef void (*cdpType)(const real_number *, real_number*, integer_number, const PQMF&);   
            // cdpType is pointer to periodic conv-dec function
typedef void (*cdgpType)(const real_number *, real_number*, integer_number, const GPQMF&);       
            // cdgpType is pointer to periodic conv-dec function
typedef void (*cdaType)(const Interval &, Interval &, const QMF &);
            // cdaType is pointer to aperiodic conv-dec function

/********************* Periodic case pow of 2  *******************************/

void WaveTrans(const Interval &In, Interval &Out, 
	       const PQMF &H, const PQMF &G, cdpType con_dec);
          // In is input interval, Out is output
          // assumptions: In.beg=0, In.length= pow of 2,  
          //  Out exists s.t. Out.beg <= 0 and Out.end >= In.end
          // on output  Out is superposed up with wavelet coefficients
         // starting from bottom level up to level 1

void WaveTrans(const Interval &In, Interval &Out, Interval &Work, 
	       const PQMF &H, const PQMF &G, cdpType con_dec);
           // same as above, only with Work
           // assumptions: same as above

void InvWaveTrans(const Interval &In, Interval &Out,
		  const PQMF &H, const PQMF &G, cdpType adj_con_dec);  
          // In is input interval, Out is output
          // assumptions: In.beg=0, In.length= pow of 2,  
          //  Out exists s.t. Out.beg <= 0 and Out.end >= In.end
          // on output  Out is superposed up with recovered signal

void InvWaveTrans(const Interval &In, Interval &Out, Interval &Work,
		  const PQMF &H, const PQMF &G, cdpType adj_con_dec);
           // same as above, only with Work
           // assumptions: same as above                 

/**************************** Periodic, n=m*2^L******************************/

void WaveTrans(const Interval &In, Interval &Out, integer_number L,
	       const GPQMF &H, const GPQMF &G, cdgpType con_dec);
           // transform to level L, everything else same as above
           // assumption: In.length divisible by 2^L
                                        
void WaveTrans(const Interval &In, Interval &Out, Interval &Work, integer_number L,
	       const GPQMF &H, const GPQMF &G, cdgpType con_dec);
                     
void InvWaveTrans(const Interval &In, Interval &Out, integer_number L,
		  const GPQMF &H, const GPQMF &G, cdgpType adj_con_dec);

void InvWaveTrans(const Interval &In, Interval &Out, Interval &Work, integer_number L,
		  const GPQMF &H, const GPQMF &G, cdgpType adj_con_dec);

/***************************  Aperiodic  *************************************/

void WaveTrans(const Interval &In, Interval *Out, integer_number L,
	       const QMF &H, const QMF &G, cdaType con_dec);
           // In is input signal, L is depth of transform       
           // assumption:  Out is array of L+1 default intervals
           // on output Out is filled up with wavelet coefficients s.t.
           // Out[0] and Out[1] are from level L,..., Out[L] is from level 1

void WaveTrans(const Interval &In, Interval *Out, Interval *Work,
	       integer_number L, const QMF &H, const QMF &G, cdaType con_dec);  
           // same as above onlywith Work     
           // additional assumption: Work is  array of L default intervals
           // on output Work[0]=Out[0] is at level L... Work[L-1] is at level 1

void InvWaveTrans(const Interval *In, Interval &Out, integer_number L,
		  const QMF &H, const QMF &G, cdaType adj_con_dec);           
         // assumptions : In is array of L+1 intervals, L is depth of transform
         // on input Out can be either default empty interval 
         // in which case on output it will be padded with 0's, 
         // or if on input Out is completely defined, then on output 
         // it will be neatly filled up with recoverd signal without extra 0's

void InvWaveTrans(const Interval *In, Interval &Out, Interval *Work, 
		  integer_number L, const QMF &H, const QMF &G, cdaType adj_con_dec);
         // same as above only with Work
         // additional assumption: Work is array of L default intervals
         // on output Work[0]=In[0] is at level L...Work[l-1] is at level 1   

#endif
