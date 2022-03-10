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
#include "AbstractWaveletTransformation.h"

class WaveletPacketTransformation : public AbstractWaveletTransformation {
public:
    enum ResolutionRatioOption {
        EQUAL = 99,
        TIME_X2 = -2,
        TIME_X4 = -1,
        FREQUENCY_X2 = 1,
        FREQUENCY_X4 = 2,

        NUMBER_OF_OPTIONS = 5,
        DEFAULT = EQUAL
    };

    WaveletPacketTransformation(
            double newSamplingRate,
            ResolutionType newResolution,
            int windowFunctionNr = SpectronParameters::WINDOWING_DEFAULT,
            int waveletBaseTypeNr = SpectronParameters::WAVELET_DEFAULT,
            ResolutionRatioOption newResolutionRatioOption = ResolutionRatioOption::DEFAULT);
    ~WaveletPacketTransformation() override;

    auto getSpectralDataInfo() -> const SpectralDataInfo & override {
        return spectralDataInfo;
    }

protected:
     void calculate() override;

private:
    double samplingRate;
    int timeFrequencyResolutionTreeLevelOffset; //Wavelet packet transform time/freq resolution ratio (0=equal time&freq)
    unsigned int resultTreeLevel;//Wavelet packet transform result tree level,

    SpectralDataInfo spectralDataInfo;

    void setResolutionRatioOption(ResolutionRatioOption newResolutionRatio);

    auto getTimeResolution() -> ResolutionType;
    auto calculateSpectralDataInfo() -> SpectralDataInfo;

    /**
     * @brief Takes a ResolutionRatioOption and returns the offset (+/- int) to the filter tree level
     * 
     * @param resolutionRatioOption 
     * @return const int 
     */
    static auto toTimeFrequencyResolutionTreeLevelOffset(const ResolutionRatioOption& resolutionRatioOption) -> int;
    static auto getFrequencyResolution(unsigned int waveletPacketResultTreeLevel) -> ResolutionType;
    static auto getWaveletPacketResultTreeLevel(unsigned int maxLevel, int resolutionRatioOffset) -> unsigned int;
};
