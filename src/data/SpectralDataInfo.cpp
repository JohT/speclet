#include "SpectralDataInfo.h"
#include "../dsp/transformations/Transformation.h"
#include <assert.h>

SpectralDataInfo::SpectralDataInfo(
        double newSamplingRate,
        ResolutionType newResolution,
        ResolutionType newFrequencyResolution,
        ResolutionType newTimeResolution,
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
        frequencyResolution = static_cast<ResolutionType>(lrint((newSamplingRate / 2.0) + 1.0));
    }
    if (newFrequencyPartitionSize == 0) {
        //default for frequencyResolution (fits for FFT)
        frequencyPartitionSize = 1.0 / static_cast<double>(newFrequencyResolution);
    }
    if (timeResolution > Transformation::TIME_RESOLUTION_LIMIT) {
        timeResolution = Transformation::TIME_RESOLUTION_LIMIT;
    }
    const auto minFrequencyPartitionSize = 0.0000001;

    assert(newSamplingRate > 100);
    assert(resolution > 0);
    assert(newFrequencyResolution >= 1);
    assert(frequencyPartitionSize > minFrequencyPartitionSize);

    DBG("SpectralDataInfo constructed" + toString());
}

//TODO (JohT) Gets the relative frequency partition size for the specified spectral line number.
auto SpectralDataInfo::constgetSpectralLineFrequencyPartitionSize(ResolutionType /*spectralLineNr*/) const -> double {
    return frequencyPartitionSize;
}

auto SpectralDataInfo::toString() const -> std::string {
    return "SpectralDataInfo(samplingFrequency=" + std::to_string(samplingFrequency) +
        ",resolution=" + std::to_string(resolution) +
        ",frequency resolution=" + std::to_string(frequencyResolution) +
        ",time resolution=" + std::to_string(timeResolution) +
        ",partition size=" + std::to_string(frequencyPartitionSize) + ")";
}