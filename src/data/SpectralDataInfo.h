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

    SpectralDataInfo() = delete; // no default constructor
    ~SpectralDataInfo() = default;// default destructor

    auto operator==(SpectralDataInfo &compareObject) -> bool;

    [[nodiscard]] auto getSamplingFrequency() const -> double {
        return samplingFrequency;
    }
    [[nodiscard]] auto getMaxFrequency() const -> double {
        return maxFrequency;
    }
    [[nodiscard]] auto getResolution() const -> long {
        return resolution;
    }
    //Gets the spectral line count within mSamplingFrequency/2 Hz ("vertical resolution")
    [[nodiscard]] auto getFrequencyResolution() const -> long {
        return frequencyResolution;
    }
    //Gets the spectral line count within 1/mResolution ms ("horizontal resolution")
    [[nodiscard]] auto getTimeResolution() const -> long {
        return timeResolution;
    }
    //Gets the time resolution in ms (best resolution in case of non linear time resolution)
    [[nodiscard]] auto getTimeResolutionMs() const -> double {
        return timeResolutionMs;
    }
    //Gets the frequency partition (e.g. 1/32 -> partition [Hz] = mSamplingFrequency/2 split into 1/32)
    [[nodiscard]] auto getFrequencyPartitionSize() const -> double {
        return frequencyPartitionSize;
    }
    //Gets the relative frequency partition size for the specified spectral line number.
    //Example 1: Fast Fourier Transformation FFT: mFrequencyPartitionSize=1/16: 1/16, 1/16, 1/16....
    //Example 2: Discrete (Decimated) Wavelet Transform DWT: mFrequencyPartitionSize=1/16, Progression=2: 1/16, 1/16, 1/8, 1/4, 1/2
    auto getSpectralLineFrequencyPartitionSize(long spectralLineNr) -> const double;

private:
    SpectralDataInfo(const SpectralDataInfo &) = default;                    // no copy constructor
    auto operator=(const SpectralDataInfo &) -> SpectralDataInfo & = default;// no assignment operator
    SpectralDataInfo(SpectralDataInfo &&) = default;                         // no move constructor
    auto operator=(SpectralDataInfo &&) -> SpectralDataInfo & = default;     // no move assignment operator

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
