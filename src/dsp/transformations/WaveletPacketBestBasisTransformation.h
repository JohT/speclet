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
#include "AbstractWaveletTransformation.h"

class WaveletPacketBestBasisTransformation : public AbstractWaveletTransformation {
public:
    WaveletPacketBestBasisTransformation(
            double newSamplingRate,
            ResolutionType newResolution,
            int windowFunctionNr,
            WaveletBase newWaveletBaseType = WaveletBase::DEFAULT);
    ~WaveletPacketBestBasisTransformation() override;

    auto getSpectralDataInfo() -> const SpectralDataInfo & override {
        return spectralDataInfo;
    }


protected:
    void calculate() override;

private:
    SpectralDataInfo spectralDataInfo;

    //advanced cost function type with param factor
    using costFunAdv = real_number (WaveletPacketBestBasisTransformation::*)(const real_number *, const integer_number &, const real_number &, const real_number &, const integer_number &);

    // using functional F  calculate costs from given tree A.
    // assumption: on input B is default empty tree
    // on output B filled up with costs and B.maxlevel=A.maxlevel
    // advanced function to integrate the cost function used in wave++ demo "demoWavPack.cpp"
    void getCosts(const ArrayTreePer &a, Tree &t, costFunAdv f, const real_number &sigma, const real_number &factor);

    // utility
    // advanced function to integrate the cost function used in wave++ demo "demoWavPack.cpp"
    void getCostsHelp(const ArrayTreePer &a, Node<real_number> **ptr,
                      costFunAdv f, const real_number &sigma, const real_number &factor,
                      const integer_number &l, const integer_number &b);

    // wrapping function: start with array bin tree A, calculate costs
    // using F and sigma. Temporarily store costs in a bin tree and then
    // find best basis which minimizes cost. On input H is an empty hedge,
    // on output H completely filled up and contains the best basis
    // advanced function to integrate the cost function used in wave++ demo "demoWavPack.cpp"
    void extractBestBasis(const ArrayTreePer &a, HedgePer &h, const double &sigma, const double &factor);

    // advanced function to integrate the cost function used in "demoWavPack.cpp"
    // and former defined in wave++ demo "demoTools.cpp"
    auto oracCostAdv(const real_number *data, const integer_number &n,
                     const real_number &sigma, const real_number &factor, const integer_number &k) -> real_number;
};
