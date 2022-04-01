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
    enum Constants {
        //TODO (JohT) could this be provided dynamically e.g. by measurement?
        SIGNAL_TO_NOISE_RATIO = 48,
    };

    WaveletPacketBestBasisTransformation(
            double newSamplingRate,
            ResolutionType newResolution,
            WindowParameters::WindowFunction newWindowFunction = WindowParameters::WindowFunction::DEFAULT,
            WaveletParameters::WaveletBase newWaveletBaseType = WaveletParameters::WaveletBase::DEFAULT);
    ~WaveletPacketBestBasisTransformation() override;

    WaveletPacketBestBasisTransformation(const WaveletPacketBestBasisTransformation &) = delete;                    //No copy contructor
    WaveletPacketBestBasisTransformation(WaveletPacketBestBasisTransformation &&) = delete;                         //No move contructor
    auto operator=(const WaveletPacketBestBasisTransformation &) -> WaveletPacketBestBasisTransformation & = delete;//No copy assignment
    auto operator=(WaveletPacketBestBasisTransformation &&) -> WaveletPacketBestBasisTransformation & = delete;     //No move assignment

    auto getSpectralDataInfo() -> const SpectralDataInfo & override {
        return spectralDataInfo;
    }

    auto getName() -> const char * override { return "Fast Wavelet Packet Best Basis Transformation"; }

protected:
    void calculate() override;

private:
    SpectralDataInfo spectralDataInfo;

    //advanced cost function type with param factor
    using costFunAdv = real_number (WaveletPacketBestBasisTransformation::*)(const real_number *, const integer_number &, const real_number &, const real_number &, const integer_number &);

    //

    /**
     * @brief Calculates 
     *
     * using functional F calculate costs from given tree A. 
     * assumption: on input B is default empty tree 
     * on output B filled up with costs and B.maxlevel=A.maxlevel
     * advanced function to integrate the cost function used in wave++ demo "demoWavPack.cpp"
     *
     * @param a 
     * @param t 
     * @param costFunction 
     * @param sigma 
     * @param factor 
     */
    void getCosts(const ArrayTreePer &a, Tree &t, costFunAdv costFunction, const real_number &sigma, const real_number &factor);

    /**
     * @brief Advanced function to integrate the cost function used in wave++ demo "demoWavPack.cpp"
     * 
     * @param a 
     * @param ptr 
     * @param costFunction
     * @param sigma 
     * @param factor 
     * @param level
     * @param b 
     */
    void getCostsHelp(const ArrayTreePer &a, Node<real_number> **ptr,
                      costFunAdv costFunction, const real_number &sigma, const real_number &factor,
                      const integer_number &level, const integer_number &b);

    /**
     * @brief Finds the best wavelet packet base which minimizes the cost function.
     *
     * Wrapping function: start with array bin tree A, calculate costs
     * using F and sigma. Temporarily store costs in a bin tree and then 
     * find best basis which minimizes cost. On input H is an empty hedge, 
     * on output H completely filled up and contains the best basis
     * advanced function to integrate the cost function used in wave++ demo "demoWavPack.cpp".

     * @param a 
     * @param h 
     * @param sigma 
     * @param factor 
     */
    void extractBestBasis(const ArrayTreePer &a, HedgePer &h, const double &sigma, const double &factor);

    /**
     * @brief advanced function to integrate the cost function used in "demoWavPack.cpp" and former defined in wave++ demo "demoTools.cpp"
     * 
     * @param data 
     * @param n 
     * @param sigma 
     * @param factor 
     * @param k 
     * @return real_number 
     */
    auto oracCostAdv(const real_number *data, const integer_number &n,
                     const real_number &sigma, const real_number &factor, const integer_number &k) -> real_number;
};
