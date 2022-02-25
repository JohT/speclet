#include "SpectralDataInfo.h"
#include "../dsp/transformations/Transformation.h"
#include "JuceHeader.h"

SpectralDataInfo::SpectralDataInfo(
        double newSamplingRate,
        long newResolution,
        long newFrequencyResolution,
        long newTimeResolution,
        double newFrequencyPartitionSize)

    : samplingFrequency(newSamplingRate),
      resolution(newResolution),
      frequencyResolution(newFrequencyResolution),
      timeResolution(newTimeResolution),
      frequencyPartitionSize(newFrequencyPartitionSize),
      maxFrequency(newSamplingRate / 2.0),
      timeResolutionMs(static_cast<double>(newResolution) / static_cast<double>(newTimeResolution) / samplingFrequency * 1000) {
    if (newFrequencyResolution == 0) {
        //default for frequencyResolution (fits for FFT)
        frequencyResolution = lrint((newSamplingRate / 2.0) + 1.0);
    }
    if (newFrequencyPartitionSize == 0) {
        //default for frequencyResolution (fits for FFT)
        frequencyPartitionSize = 1.0 / static_cast<double>(newFrequencyResolution);
    }
    if (timeResolution > Transformation::TIME_RESOLUTION_LIMIT) {
        timeResolution = Transformation::TIME_RESOLUTION_LIMIT;
    }

    jassert(newSamplingRate > 100);
    jassert(newFrequencyResolution >= 1);
    jassert(frequencyPartitionSize > 0.0000001);

    DBG("SpectralDataInfo constructed with resolution=" +
        juce::String(newResolution) +
        ",frequency resolution=" + juce::String(newFrequencyResolution) +
        ",time resolution=" + juce::String(newTimeResolution) +
        ",partition size=" + juce::String(frequencyPartitionSize));
}

auto SpectralDataInfo::getSamplingFrequency() -> double {
    return samplingFrequency;
}

auto SpectralDataInfo::getMaxFrequency() -> double {
    return maxFrequency;
}

auto SpectralDataInfo::getResolution() -> long {
    return resolution;
}

//gets the spectral line count within mSamplingFrequency/2 Hz
auto SpectralDataInfo::getFrequencyResolution() -> long {
    return frequencyResolution;
}

//gets the spectral line count within 1/mResolution ms
auto SpectralDataInfo::getTimeResolution() -> long {
    return timeResolution;
}

//gets the time resolution in ms (best resolution in case of non lin time res.)
auto SpectralDataInfo::getTimeResolutionMs() -> double {
    return timeResolutionMs;
}

//gets the freq. partition (e.g. 1/32 -> partition [Hz] = mSamplingFrequency/2 split into 1/32)
auto SpectralDataInfo::getFrequencyPartitionSize() -> double {
    return frequencyPartitionSize;
}

auto SpectralDataInfo::getSpectralLineFrequencyPartitionSize(long spectralLineNr) -> double {
    //e.g. FFT mFrequencyPartitionSize=1/16: 1/16, 1/16, 1/16....
    //e.g. DWT mFrequencyPartitionSize=1/16, Progression=2: 1/16, 1/16, 1/8, 1/4, 1/2
    jassert(spectralLineNr >= 0);
    return frequencyPartitionSize;
}