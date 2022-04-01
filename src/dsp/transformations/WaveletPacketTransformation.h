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
#include "../../plugin/SpecletParameters.h"
#include "AbstractWaveletTransformation.h"
#include "WaveletParameters.h"

class WaveletPacketTransformation : public AbstractWaveletTransformation {
public:
    WaveletPacketTransformation(
            double newSamplingRate,
            ResolutionType newResolution,
            WindowFunctionFactory::Method newWindowFunction,
            WaveletParameters::WaveletBase newWaveletBaseType = WaveletParameters::WaveletBase::DEFAULT,
            WaveletParameters::ResolutionRatioOption newResolutionRatioOption = WaveletParameters::ResolutionRatioOption::DEFAULT);
    ~WaveletPacketTransformation() override;
    WaveletPacketTransformation(const WaveletPacketTransformation &) = delete;                    //No copy contructor
    WaveletPacketTransformation(WaveletPacketTransformation &&) = delete;                         //No move contructor
    auto operator=(const WaveletPacketTransformation &) -> WaveletPacketTransformation & = delete;//No copy assignment
    auto operator=(WaveletPacketTransformation &&) -> WaveletPacketTransformation & = delete;     //No move assignment

    auto getSpectralDataInfo() -> const SpectralDataInfo & override {
        return spectralDataInfo;
    }
    auto getName() -> const char * override { return "Fast Wavelet Packet Transform"; }

protected:
    void calculate() override;

private:
    double samplingRate;
    int timeFrequencyResolutionTreeLevelOffset;//Wavelet packet transform time/freq resolution ratio (0=equal time&freq)
    WaveletLevelType resultTreeLevel;          //Wavelet packet transform result tree level,

    SpectralDataInfo spectralDataInfo;

    void setResolutionRatioOption(WaveletParameters::ResolutionRatioOption newResolutionRatio);

    auto getTimeResolution() -> ResolutionType;
    auto calculateSpectralDataInfo() -> SpectralDataInfo;

    /**
     * @brief Takes a ResolutionRatioOption and returns the offset (+/- int) to the filter tree level
     * 
     * @param resolutionRatioOption 
     * @return const int 
     */
    static auto toTimeFrequencyResolutionTreeLevelOffset(const WaveletParameters::ResolutionRatioOption &resolutionRatioOption) -> int;
    static auto getFrequencyResolution(WaveletLevelType waveletPacketResultTreeLevel) -> ResolutionType;
    static auto getWaveletPacketResultTreeLevel(WaveletLevelType maxLevel, int resolutionRatioOffset) -> WaveletLevelType;
};
