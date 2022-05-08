//*****************************************************************************
// WavePacket.h                          Prototypes for analysis and synthesis
//*****************************************************************************

#ifndef WAVEPACKET_H
#define WAVEPACKET_H

#include "QMF.h"
#include "ConvDec.h"
#include "ArrayTree.h"

typedef void (*cdpType)(const real_number *, real_number*, integer_number, const PQMF&);   
            // cdpType is pointer to periodic conv-dec function
typedef void (*cdgpType)(const real_number *, real_number*, integer_number, const GPQMF&);       
            // cdgpType is pointer to periodic conv-dec function
typedef void (*cdaType)(const Interval &, Interval &, const QMF &);
            // cdaType is pointer to aperiodic conv-dec function

/********************** Periodic case power of 2 *****************************/

void Analysis(const Interval &In, ArrayTreePer &A, 
	      const PQMF &H, const PQMF &G, cdpType con_dec);
            // In is input signal A is preallocted 0 zero tree such that
            // A.dim = In.length is power of 2
            // H for left, G for right

void Synthesis(ArrayTreePer &A, Interval &Out,
	       const PQMF &H, const PQMF &G, cdpType adj_con_dec);
            // A is predefined tree, Out is empty interval
            // Synthesis starts from the bottom row of A and works its way up 
            // by superposing onto what is already there
            // Out recieves the top row of A after synthesis

/************************* Periodic case even *******************************/

void Analysis(const Interval &In, ArrayTreePer &A, 
	      const GPQMF &H, const GPQMF &G, cdgpType con_dec);
            // In is input signal A, is preallocted 0 zero tree such that
            // A.dim = In.length is even
            // H for left, G for right

void Synthesis(ArrayTreePer &A, Interval &Out,
	       const GPQMF &H, const GPQMF &G, cdgpType adj_con_dec);
            // A is predefined tree, Out is empty interval
            // Synthesis starts from the bottom row of A and works its way up 
            // by superposing onto what is already there
            // Out recieves the top row of A after synthesis

/************************ Aperiodic case *************************************/

void Analysis(const Interval &In, ArrayTreeAper &A,
	      const QMF &H, const QMF &G, cdaType con_dec);
             // In is input signal, A is preallocted tree of empty intervals
             // use H for left, use G for right

void Synthesis(ArrayTreeAper &A, Interval &Out,
	       const QMF &H, const QMF &G, cdaType adj_con_dec);
             // A is predefined tree, with all interval sizes set
             // Out is empty interval
             // Synthesis starts from the bottom row of A and works its way up              // by superposing onto
             // what is already there
             // Out recieves the top row of A after synthesis
#endif
