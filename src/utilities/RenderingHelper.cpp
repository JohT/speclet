#include "RenderingHelper.h"
#include "../ui/ColourGradients.h"
#include "../utilities/PerformanceManager.h"


using namespace std;

RenderingHelper::RenderingHelper(void) {
    colourGradient = GRADIENT_BLUE;
}

RenderingHelper::~RenderingHelper(void) {
}

//method for rendering one column of spectral data
void RenderingHelper::renderVerticalPoints(
        Transformation *transformation,
        TAnalyzerSettings settings,
        long currentXPos,
        juce::Image *spectralImage) {
    PerformanceManager::getSingletonInstance().start("renderVerticalPoints");

    // --- inputdata check
    jassert(transformation);
    jassert(spectralImage);
    SpectralDataBuffer *buffer = transformation->getSpectralDataBuffer();
    if (!buffer) {
        DBG("RenderingHelper::renderVerticalPoints(..image): buffer is null!!");
        return;
    }
    if (buffer->size() <= 0) {
        DBG("RenderingHelper::renderVerticalPoints(..image): spectral data empty!!");
        return;
    }

    // --- get spectrum from buffer
    SpectralDataBuffer::ItemType spectrum;
    transformation->getNextSpectrum(&spectrum);
    if (spectrum.empty()) {
        DBG("RenderingHelper::renderVerticalPoints: spectrum empty!");
        return;
    }

    // --- get statistics of spectrum (min, max, ...)
    SpectralDataBuffer::ItemStatisticsType statistics = transformation->getSpectrumStatistics(&spectrum);

    // --- get info about spectral data (frequency/time-resolution...)
    SpectralDataInfo *spectralDataInfo = transformation->getSpectralDataInfo();
    if (spectralDataInfo == nullptr) {
        DBG("RenderingHelper::renderVerticalPoints: spectralDataInfo null!");
        return;
    }

    if (spectrum.size() != spectralDataInfo->getFrequencyResolution()) {
        DBG("RenderingHelper::renderVerticalPoints: spectrum.size()=" +
            juce::String(spectrum.size()) +
            "!= fres=" + juce::String(spectralDataInfo->getFrequencyResolution()));
        return;
    }

    // --- define and init other temporary variables
    int height = spectralImage->getHeight();
    int spectralLineIndexOfPixel = 0;
    double amplitudeColorIndex = 0.0;
    double magnitude = 0.0;

    for (int i = 0; i <= height; i++) {
        spectralLineIndexOfPixel = pixelToIndex(i, height, spectralDataInfo, settings.logFrequency);
        magnitude = spectrum[spectralLineIndexOfPixel];
        amplitudeColorIndex = getColorAmount(magnitude, statistics.min, statistics.max, settings.logMagnitude);
        juce::Colour colour = colourGradient.getColourAtPosition(amplitudeColorIndex);
        spectralImage->setPixelAt(currentXPos, (height - i), colour);
    }

    PerformanceManager::getSingletonInstance().stop("renderVerticalPoints");
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
        SpectralDataInfo *spectralDataInfo,
        bool logFrequency) -> long {
    if (pixel <= 0) return 0;//DC in spectrum always on index = 0
    jassert(height > 0);
    jassert(pixel <= height);
    //assert(spectralDataInfo);

    double frequencyResolution = spectralDataInfo->getFrequencyResolution();
    double percentOfSpectrum = pixel / (double) height;
    double percentOfSpectrumPerIndex = spectralDataInfo->getFrequencyPartitionSize();
    percentOfSpectrumPerIndex = assureBorders("percentOfSpectrumPerIndex", percentOfSpectrumPerIndex, 0.0, 1.0);

    if (logFrequency) {
        double frequencyMax = spectralDataInfo->getSamplingFrequency() / 2.0;
        double frequencyMaxLog = log10(frequencyMax);
        double frequencyMinLog = 1.0;
        double frequencyRangeLog = frequencyMaxLog - frequencyMinLog;

        percentOfSpectrum = pow(10, (frequencyRangeLog * percentOfSpectrum) + frequencyMinLog) / frequencyMax;
    }
    percentOfSpectrum = assureBorders("percentOfSpectrum", percentOfSpectrum, 0.0, 1.0);

    int index = roundToInt(percentOfSpectrum / percentOfSpectrumPerIndex);
    if (index > (int) (frequencyResolution - 1)) index = (int) (frequencyResolution - 1);
    index = (int) assureBorders("index", (double) index, 0.0, (double) (frequencyResolution - 1));

    return index;
}

auto RenderingHelper::assureBorders(const juce::String &paramName, double value, double min, double max) -> double {
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

auto RenderingHelper::roundToInt(double d) -> int {
    return d < 0 ? d - 0.5F : d + 0.5F;
}