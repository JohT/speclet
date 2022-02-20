//*****************************************************************************
// ConvDec.h                     Prototypes of convolution-decimation froutines
//*****************************************************************************
#ifndef CONVDEC_H
#define CONVDEC_H

#include "common.h"
#include "QMF.h"
#include "Interval.h"

void ConvDecPer(const real_DWT *In, real_DWT *Out, integer q, const PQMF &F );
void ConvDecPer(const real_DWT *In, real_DWT *Out, integer q, const GPQMF &F );
  
  // Periodic convolution-decimation. Fast version.
  // It is assumed that F.beg <= 0 <= F.end.  Else use version 2.
  // In predefined of length q, q must be even.
  // Out predefined so that Out[0].... Out[q/2-1] exist.
  // Output superimposed, so if assignment wanted all elts of Out should be 0's

void ConvDecV2Per(const real_DWT *In, real_DWT *Out, integer q, const PQMF &F);
void ConvDecV2Per(const real_DWT *In, real_DWT *Out, integer q, const GPQMF &F);

  // Periodic convolution-decimation version 2.
  // It is NOT assumed that F.beg <= 0 <= F.end.  Slower algorithm.
  // In predefined of length q, where q must be even.
  // Out predefined so that Out[0].... Out[q/2-1] exist.
  // Output superimposed, so if assignment wanted all elts of Out should be 0's

void AdjConvDecPer(const real_DWT *In, real_DWT *Out, integer q, const PQMF &F );
void AdjConvDecPer(const real_DWT *In, real_DWT *Out, integer q, const GPQMF &F );
  
  // Adjoint periodic convolution-decimation.  Fast version.
  // It is assumed that F.beg <= 0 <= F.end.  Else use version 2.
  // In predefined of length q.
  // Out  predefined so that Out[0].... Out[2*q-1] exist
  // Output superimposed, so if assignment wanted all elts of Out should be 0's

void AdjConvDecV2Per(const real_DWT *In, real_DWT *Out, integer q, const PQMF &F );
void AdjConvDecV2Per(const real_DWT *In, real_DWT *Out, integer q, const GPQMF &F );
  
  // Adjoint periodic convolution-decimation.
  // It is NOT assumed that F.beg <= 0 <= F.end.  Slower algorithm.
  // In predefined of length q.
  // Out predefined so that Out[0].... Out[2*q-1] exist
  // Output superimposed, so if assignment wanted all elts of Out should be 0's

void ConvDecAper(const Interval &In, Interval &Out, const QMF &F);

  // Aperiodic convolution-decimation.
  // In predefined, Out predefined. Must have
  // Out.beg = ICH(In.beg + F.beg) and Out.end = IFH(In.end  + F.end).
  // Output superimposed, so if assignment wanted all elts of Out should be 0's

void AdjConvDecAper(const Interval &In, Interval &Out, const QMF &F);

  // Adjoint aperiodic convolution-decimation.
  // In predefined, Out predefined. Must have
  // Out.beg = 2*In.beg-F.end and Out.end = 2*In.end-F.beg
  // Output superimposed, so if assignment wanted all elts of Out should be 0's

#endif
