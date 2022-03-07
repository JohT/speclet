#pragma once
#include "WaveletTransformation.h"

WaveletTransformation::WaveletTransformation(
        double samplingRate,
        ResolutionType newResolution,
        int windowFunctionNr,
        int waveletBaseTypeNr)
    : AbstractWaveletTransformation(samplingRate, newResolution, windowFunctionNr, waveletBaseTypeNr),
      fastWaveletTransformTimer(PerformanceTimer("Fast Wavelet Transform")) {

    mFrequencyResolution = newResolution;
    mTimeResolution = newResolution / 2;
    mSpectralDataInfo = new SpectralDataInfo(samplingRate, newResolution, mFrequencyResolution, mTimeResolution);

    DBG("WaveletTransformation::initialize done with fs=" + juce::String(mSamplingRate) + ",res=" + juce::String(newResolution));

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
    WaveTrans(*mDwtInput, outDWT, mDwtFilterH, mDwtFilterG, ConvDecPer);
    fastWaveletTransformTimer.stop();
    //fills the outputQueue with the spectral data (in a ready to draw order)
    extractSpectrum(outDWT);
}