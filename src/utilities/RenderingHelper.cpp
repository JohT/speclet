#include "RenderingHelper.h"
#include "../ui/ColourGradients.h"
#include "PerformanceLogger.h"
#include "PerformanceTimer.h"


RenderingHelper::RenderingHelper()
    : colourGradient(ColourGradients::BLUE),
      renderVerticalPointsTimer(PerformanceTimer("RenderingHelper::renderVerticalPointsTimer")) {
    
}

//method for rendering one column of spectral data
void RenderingHelper::renderVerticalPoints(
        TransformationResult *transformationResult,
        TAnalyzerSettings settings,
        int currentXPos,
        juce::Image *spectralImage) {

    LOG_PERFORMANCE_OF_SCOPE("RenderingHelper renderVerticalPoints");
    renderVerticalPointsTimer.start();

    // --- inputdata check
    assert(transformationResult);
    assert(spectralImage);
    SpectralDataBuffer *buffer = transformationResult->getSpectralDataBuffer();
    if (buffer == nullptr) {
        DBG("RenderingHelper::renderVerticalPoints(..image): buffer is null!!");
        return;
    }
    if (buffer->size() <= 0) {
        DBG("RenderingHelper::renderVerticalPoints(..image): spectral data empty!!");
        return;
    }

    // --- get spectrum from buffer
    SpectralDataBuffer::ItemType spectrum;
    transformationResult->getNextSpectrum(&spectrum);
    if (spectrum.empty()) {
        DBG("RenderingHelper::renderVerticalPoints: spectrum empty!");
        return;
    }

    // --- get statistics of spectrum (min, max, ...)
    SpectralDataBuffer::ItemStatisticsType statistics = SpectralDataBuffer::ItemStatisticsType(spectrum);

    // --- get info about spectral data (frequency/time-resolution...)
    SpectralDataInfo spectralDataInfo = transformationResult->getSpectralDataInfo();

    if (spectrum.size() != spectralDataInfo.getFrequencyResolution()) {
        DBG("RenderingHelper::renderVerticalPoints: spectrum.size()=" +
            juce::String(spectrum.size()) +
            "!= frequency resolution=" + juce::String(spectralDataInfo.getFrequencyResolution()));
        return;
    }

    // --- define and init other temporary variables
    auto height = spectralImage->getHeight();
    unsigned long spectralLineIndexOfPixel = 0;
    auto amplitudeColorIndex = 0.0;
    auto magnitude = 0.0;

    for (auto i = 0; i <= height; i++) {
        spectralLineIndexOfPixel = pixelToIndex(i, height, spectralDataInfo, settings.logFrequency);
        magnitude = spectrum[spectralLineIndexOfPixel];
        amplitudeColorIndex = getColorAmount(magnitude, statistics.min, statistics.max, settings.logMagnitude);
        juce::Colour colour = colourGradient.getColourAtPosition(amplitudeColorIndex);
        spectralImage->setPixelAt(currentXPos, (height - i), colour);
    }

    renderVerticalPointsTimer.stop();
}

auto RenderingHelper::getColorAmount(
        double magnitude,
        double minMagnitude,
        double maxMagnitude,
        bool logMagnitude) -> double {
    if (logMagnitude) {
        maxMagnitude = 20 * log(maxMagnitude);
        minMagnitude = 20 * log(minMagnitude);
        magnitude = 20 * log(magnitude);
    }
    double rangeMagnitude = maxMagnitude - minMagnitude;
    double amplitudeColorIndex = (magnitude - minMagnitude) / rangeMagnitude;

    amplitudeColorIndex = assureBorders("amplitudeColorIndex", amplitudeColorIndex, 0.0, 1.0);
    return amplitudeColorIndex;
}

auto RenderingHelper::pixelToIndex(
        int pixel,
        int height,
        const SpectralDataInfo &spectralDataInfo,
        bool logFrequency) -> unsigned long {
    if (pixel <= 0) {
        return 0;//DC in spectrum always on index = 0
    }
    assert(height > 0);
    assert(pixel <= height);

    auto frequencyResolution = spectralDataInfo.getFrequencyResolution();
    auto percentOfSpectrum = pixel / static_cast<double>(height);
    auto percentOfSpectrumPerIndex = spectralDataInfo.getFrequencyPartitionSize();
    percentOfSpectrumPerIndex = assureBorders("percentOfSpectrumPerIndex", percentOfSpectrumPerIndex, 0.0, 1.0);

    if (logFrequency) {
        auto frequencyMax = spectralDataInfo.getSamplingFrequency() / 2.0;
        auto frequencyMaxLog = log10(frequencyMax);
        auto frequencyMinLog = 1.0;
        auto frequencyRangeLog = frequencyMaxLog - frequencyMinLog;

        percentOfSpectrum = pow(10, (frequencyRangeLog * percentOfSpectrum) + frequencyMinLog) / frequencyMax;
    }
    percentOfSpectrum = assureBorders("percentOfSpectrum", percentOfSpectrum, 0.0, 1.0);

    auto index = roundToInt(percentOfSpectrum / percentOfSpectrumPerIndex);
    if (index > static_cast<int>(frequencyResolution - 1)) {
        index = static_cast<int>(frequencyResolution - 1);
    }
    index = static_cast<int>(assureBorders("index", static_cast<double>(index), 0.0, (static_cast<double>(frequencyResolution) - 1.0)));
    assert(index >= 0);
    return static_cast<unsigned int>(index);
}

auto RenderingHelper::assureBorders(const juce::String & /*paramName*/, double value, double min, double max) -> double {
    if (value < min) {
        //DBG(T("RenderingHelper::assureBorders: <")
        //	+ paramName					+ T(">")
        //	+ juce::String(value)	+ T(" is smaller than ")
        //	+ juce::String(min)		+ T(", so it's set to min"));
        return min;
    }
    if (value > max) {
        //DBG(T("RenderingHelper::assureBorders: <")
        //	+ paramName				+ T(">")
        //	+ juce::String(value)+ T(" is greater than ")
        //	+ juce::String(max)	+ T(", so it's set to max."));
        return max;
    }
    return value;
}