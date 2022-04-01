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

#include <string>

class SpectralDataInfo {
public:
    using ResolutionType = unsigned long;

    SpectralDataInfo(
            double newSamplingRate,
            ResolutionType newResolution,
            ResolutionType newFrequencyResolution = 0,
            ResolutionType newTimeResolution = 1,
            double newFrequencyPartitionSize = 0.0);

    SpectralDataInfo() = delete;  // no default constructor
    ~SpectralDataInfo() = default;// default destructor

    SpectralDataInfo(const SpectralDataInfo &) = default;                    // default copy constructor
    auto operator=(const SpectralDataInfo &) -> SpectralDataInfo & = default;// default assignment operator
    SpectralDataInfo(SpectralDataInfo &&) = default;                         // default move constructor
    auto operator=(SpectralDataInfo &&) -> SpectralDataInfo & = default;     // default move assignment operator

    auto operator==(SpectralDataInfo &compareObject) -> bool;

    [[nodiscard]] auto getSamplingFrequency() const -> double {
        return samplingFrequency;
    }
    [[nodiscard]] auto getMaxFrequency() const -> double {
        return maxFrequency;
    }
    [[nodiscard]] auto getResolution() const -> ResolutionType {
        return resolution;
    }
    /**
     * @brief Gets the spectral line count within mSamplingFrequency/2 Hz ("vertical resolution")
     * 
     * @return ResolutionType 
     */
    [[nodiscard]] auto getFrequencyResolution() const -> ResolutionType {
        return frequencyResolution;
    }
    /**
     * @brief Gets the spectral line count within 1/mResolution ms ("horizontal resolution")
     * 
     * @return ResolutionType 
     */
    [[nodiscard]] auto getTimeResolution() const -> ResolutionType {
        return timeResolution;
    }
    /**
     * @brief Gets the time resolution in ms (best resolution in case of non linear time resolution)
     * 
     * @return double 
     */
    [[nodiscard]] auto getTimeResolutionMs() const -> double {
        return timeResolutionMs;
    }
    /**
     * @brief Gets the frequency partition (e.g. 1/32 -> partition [Hz] = mSamplingFrequency/2 split into 1/32)
     * 
     * @return double 
     */
    [[nodiscard]] auto getFrequencyPartitionSize() const -> double {
        return frequencyPartitionSize;
    }
    /**
     * @brief Gets the relative frequency partition size for the specified spectral line number.
     * Example 1: Fast Fourier Transformation FFT: mFrequencyPartitionSize=1/16: 1/16, 1/16, 1/16....
     * Example 2: Discrete (Decimated) Wavelet Transform DWT: mFrequencyPartitionSize=1/16, Progression=2: 1/16, 1/16, 1/8, 1/4, 1/2
     * @param spectralLineNr ResolutionType
     * @return double 
     */
    [[nodiscard]] auto constgetSpectralLineFrequencyPartitionSize(ResolutionType spectralLineNr) const -> double;

    [[nodiscard]] auto toString() const -> std::string;

private:
    double samplingFrequency;
    ResolutionType resolution;
    ResolutionType frequencyResolution;
    ResolutionType timeResolution;
    //fields for non constant frequency partitions
    double frequencyPartitionSize;
    //calculated fields
    double maxFrequency;
    double timeResolutionMs;
};
