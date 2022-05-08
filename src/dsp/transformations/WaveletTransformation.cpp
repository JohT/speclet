#include "WaveletTransformation.h"
#include "../../utilities/PerformanceLogger.h"
#include "TransformationParameters.h"
#include <limits>


WaveletTransformation::WaveletTransformation(
        double newSamplingRate,
        ResolutionType newResolution,
        WindowParameters::WindowFunction newWindowFunction,
        WaveletParameters::WaveletBase newWaveletBase)
    : AbstractWaveletTransformation(newSamplingRate, newResolution, TransformationParameters::Type::FAST_WAVELET_TRANSFORM, newWindowFunction, newWaveletBase),
      spectralDataInfo(SpectralDataInfo(newSamplingRate, newResolution, newResolution, newResolution / 2)) {

    DBG("WaveletTransformation::initialize done with fs=" + juce::String(newSamplingRate) + ",res=" + juce::String(newResolution));
    assert(newResolution <= std::numeric_limits<long>::max());//wave++ Interval requires the resolution to be a long
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
    Interval outDWT(0, static_cast<integer_number>(getResolution() - 1));
    //fast wavelet transform
    {
        LOG_PERFORMANCE_OF_SCOPE("WaveletTransformation analyse");
        analyse(outDWT);
    }
    //fills the outputQueue with the spectral data (in a ready to draw order)
    extractSpectrum(outDWT);
}