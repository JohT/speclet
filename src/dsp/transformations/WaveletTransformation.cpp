#pragma once
#include "WaveletTransformation.h"
#include "../../utilities/PerformanceManager.h"

WaveletTransformation::WaveletTransformation(
        double samplingRate,
        long resolution,
        int windowFunctionNr,
        int waveletBaseTypeNr)
    : AbstractWaveletTransformation(samplingRate, resolution, windowFunctionNr, waveletBaseTypeNr) {
    mFrequencyResolution = resolution;
    mTimeResolution = resolution / 2;
    mSpectralDataInfo = new SpectralDataInfo(samplingRate, resolution, mFrequencyResolution, mTimeResolution);

    DBG("WaveletTransformation::initialize done with fs=" + juce::String(mSamplingRate) + ",res=" + juce::String(mResolution));

    ready = true;
    calculated = true;
};

WaveletTransformation::~WaveletTransformation() {
    ready = false;
    DBG("WaveletTransformation destructed");
}

void WaveletTransformation::calculate() {
    //fills the mDWT_Input with data from the inputQueue
    fillDWTInput();
    //output data container to hold the result of the wavelet transformation ("coefficients")
    Interval outDWT(0, mResolution - 1);
    //fast wavelet transform
    PerformanceManager::getSingletonInstance().start("fwt");
    WaveTrans(*mDwtInput, outDWT, mDwtFilterH, mDwtFilterG, ConvDecPer);
    PerformanceManager::getSingletonInstance().stop("fwt");
    //fills the outputQueue with the spectral data (in a ready to draw order)
    extractSpectrum(outDWT);
}