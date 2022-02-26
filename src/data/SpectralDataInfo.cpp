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
    const auto minFrequencyPartitionSize = 0.0000001;

    jassert(newSamplingRate > 100);
    jassert(newFrequencyResolution >= 1);
    jassert(frequencyPartitionSize > minFrequencyPartitionSize);

    DBG("SpectralDataInfo constructed with resolution=" +
        juce::String(newResolution) +
        ",frequency resolution=" + juce::String(newFrequencyResolution) +
        ",time resolution=" + juce::String(newTimeResolution) +
        ",partition size=" + juce::String(frequencyPartitionSize));
}

auto SpectralDataInfo::getSpectralLineFrequencyPartitionSize(long spectralLineNr) -> const double {
    jassert(spectralLineNr >= 0);
    return frequencyPartitionSize;
}