#pragma once
#include "WaveletTransformation.h"

WaveletTransformation::WaveletTransformation(
        double newSamplingRate,
        ResolutionType newResolution,
        int windowFunctionNr,
        WaveletBase newWaveletBase)
    : AbstractWaveletTransformation(newSamplingRate, newResolution, windowFunctionNr, newWaveletBase),
      spectralDataInfo(SpectralDataInfo(newSamplingRate, newResolution, newResolution, newResolution / 2)),
      fastWaveletTransformTimer(PerformanceTimer("Fast Wavelet Transform")) {

    DBG("WaveletTransformation::initialize done with fs=" + juce::String(newSamplingRate) + ",res=" + juce::String(newResolution));

    setReady();
    setCalculated();
}

WaveletTransformation::~WaveletTransformation() {
    setReady(false);
    DBG("WaveletTransformation destructed");
}

void WaveletTransformation::calculate() {
    //fills the mDWT_Input with data from the inputQueue
    fillDWTInput();
    //output data container to hold the result of the wavelet transformation ("coefficients")
    Interval outDWT(0, getResolution() - 1);
    //fast wavelet transform
    fastWaveletTransformTimer.start();
    WaveTrans(getDwtInput(), outDWT, mDwtFilterH, mDwtFilterG, ConvDecPer);
    fastWaveletTransformTimer.stop();
    //fills the outputQueue with the spectral data (in a ready to draw order)
    extractSpectrum(outDWT);
}