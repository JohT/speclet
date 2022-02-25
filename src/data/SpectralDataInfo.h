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
#include <vector>

class SpectralDataInfo {
public:
    SpectralDataInfo(
            double samplingRate,
            long resolution,
            long frequencyResolution = 0,
            long timeResolution = 1,
            double frequencyPartitionRatio = 0.0);
    ~SpectralDataInfo();
    auto operator==(SpectralDataInfo &compareObject) -> bool;

    auto getSamplingFrequency() -> double;
    auto getMaxFrequency() -> double;
    auto getResolution() -> long;
    auto getFrequencyResolution() -> long;     //spectral line count within mSamplingFrequency/2 Hz ("vertical resolution")
    auto getTimeResolution() -> long;          //spectral line count within 1/mResolution ms ("horizontal resolution")
    auto getTimeResolutionMs() -> double;      //time resolution in ms (best resolution in case of non lin time res.)
    auto getFrequencyPartitionSize() -> double;//(e.g. 1/32 -> partition [Hz] = mSamplingFrequency/2 split into 1/32)
    auto getSpectralLineFrequencyPartitionSize(long spectralLineNr) -> double;

private:
    SpectralDataInfo(const SpectralDataInfo &) = default;                    // no copy constructor
	auto operator=(const SpectralDataInfo &) -> SpectralDataInfo & = default; // no assignment operator
	SpectralDataInfo(SpectralDataInfo &&) = default;                         // no move constructor
    SpectralDataInfo() = default;                                            // no default constructor

    double samplingFrequency;
    long resolution;
    long frequencyResolution;
    long timeResolution;
    //fields for non constant frequency partitions
    double frequencyPartitionSize;
    //calculated fields
    double maxFrequency;
    double timeResolutionMs;
};
