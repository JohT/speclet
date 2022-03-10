/*
  ==============================================================================
  This file is part of the VST spectrum analyzer plugin "speclet" (working title)
  Copyright 2011 by Johannes Troppacher
	------------------------------------------------------------------------------
  This file may use parts of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.
	------------------------------------------------------------------------------
  This file may use parts of the fftw library
  Copyright 2003 Matteo Frigo, Copyright 2003 Massachusetts Institute of Technology
	------------------------------------------------------------------------------
  This file may use parts of the wave++ library
  Copyright 1999 by S. E. Ferrando, L. A. Kolasa and N. Kovacevic
  ==============================================================================
*/
#pragma once
#include "../../plugin/SpectronParameters.h"
#include "Transformation.h"
#include "TransformationFactory.h"
#include "fftw3.h"


class FourierTransformation : public Transformation {
public:
    FourierTransformation(
            double newSamplingRate,
            ResolutionType newResolution,
            int windowFunctionNr = SpectronParameters::WINDOWING_DEFAULT);
    ~FourierTransformation() override;

protected:
    void calculate() override;
    auto getSpectralDataInfo() -> const SpectralDataInfo & override {
      return spectralDataInfo;
    }

private:
    fftw_plan plan;   //Plan holds prepared (optimized) fft
    double *in;       //Pointer to input data
    fftw_complex *out;//Pointer to output data

    PerformanceTimer fftExecutePlanTimer;
    PerformanceTimer fftInputCopyTimer;
    PerformanceTimer fftOutputCopyTimer;
    
    SpectralDataInfo spectralDataInfo;
};
