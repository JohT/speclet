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
#include "../../parameter/SpecletParameters.h"
#include "AbstractWaveletTransformation.h"
#include "../windowing/WindowFunctionFactory.h"
#include "WaveletParameters.h"

class WaveletTransformation : public AbstractWaveletTransformation {
public:
    WaveletTransformation() = delete;//No default contructor
    WaveletTransformation(
            double newSamplingRate,
            ResolutionType newResolution,
            WindowParameters::WindowFunction newWindowFunction = WindowParameters::WindowFunction::DEFAULT,
            WaveletParameters::WaveletBase newWaveletBase = WaveletParameters::WaveletBase::DEFAULT);

    ~WaveletTransformation() override;
    WaveletTransformation(WaveletTransformation &) = delete;                     //No copy contructor
    WaveletTransformation(WaveletTransformation &&) = delete;                    //No move contructor
    auto operator=(WaveletTransformation &) -> WaveletTransformation & = delete; //No copy assignment
    auto operator=(WaveletTransformation &&) -> WaveletTransformation & = delete;//No move assignment

    auto getSpectralDataInfo() -> const SpectralDataInfo & override {
        return spectralDataInfo;
    }

    auto getName() -> const char * override { return "Fast Wavelet Transformation"; }

protected:
    void calculate() override;

private:
    SpectralDataInfo spectralDataInfo;
};
