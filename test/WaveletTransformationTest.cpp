#include "../src/PluginProcessor.h"
#include <catch2/catch_all.hpp>
#include <string>

SCENARIO("Wavelet Transformation") {
    auto samplingRate = 44100;
    Transformation::ResolutionType resolution = 1024;
    Transformation *transformation = TransformationFactory::getSingletonInstance().createTransformation(
            TransformationFactory::Type::FAST_WAVELET_TRANSFORM,
            samplingRate,
            resolution,
            WindowFunctionFactory::Method::RECTANGULAR,
            AbstractWaveletTransformation::WaveletBase::VAIDYANATHAN_18);
    REQUIRE(transformation != nullptr);

    GIVEN("get window") {

        WHEN("window functions are cached") {
            THEN("window is newly created when resolution changes") {
            }
        }
    }
}

void generateTestSineInput(double frequency, Transformation *transformation, double samplingRate, Transformation::ResolutionType resolution) {
    double omega = 2.0f * M_PI * frequency;
    double periode = 1.0f / frequency;
    double samplesPerPeriode = samplingRate * periode;
    INFO("\nfrequency=" << frequency << " Hz, omega=" << omega << " rad/s, periode=" << periode << " ms, samplesPerPeriode=" << samplesPerPeriode << "\n");

    //Generate sine wave input samples
    for (std::size_t t = 0; t < resolution * 10; t++) {
        double samplingPeriod = 1.0 / samplingRate;
        double sample = 0.1f * sin(omega * static_cast<double>(t) * samplingPeriod);
        REQUIRE(sample >= -1.0);
        REQUIRE(sample <= 1.0);
        transformation->setNextInputSample(sample);
    }
}

std::size_t getMaxMagnitudeSpectralLineIndex(const SpectralDataBuffer::ItemType &spectralData) {
    double maxMagnitude = 0.0;
    std::size_t maxMagnitudeSpectrumLine = 0;
    for (std::size_t spectrumLine = 0; spectrumLine < spectralData.size(); spectrumLine++) {
        if (spectralData[spectrumLine] >= maxMagnitude) {
            maxMagnitude = spectralData[spectrumLine];
            maxMagnitudeSpectrumLine = spectrumLine;
        }
    }
    return maxMagnitudeSpectrumLine;
}

/**
 * @brief Determines the frequency of the maximum magnitude spectral line to calculate its deviation from the expected frequency.
 * 
 * @param transformation Transformation object
 * @param expectedFrequency The expected ("right") frequency of the sine generator
 * @return double
 */
double getMaxPowerFrequencyDeviation(Transformation *transformation, double expectedFrequency) {
    auto &spectralDataInfo = transformation->getSpectralDataInfo();
    auto frequencyResolutionInHz = static_cast<double>(spectralDataInfo.getSamplingFrequency()) * 0.5 / static_cast<double>(spectralDataInfo.getResolution());

    SpectralDataBuffer *spectralDataBuffer = transformation->getSpectralDataBuffer();
    REQUIRE(spectralDataBuffer);

    int count = 0;
    double maxPowerFrequencyDeviationSum = 0.0;
    while (transformation->isOutputAvailable()) {
        SpectralDataBuffer::ItemType spectralData;
        spectralDataBuffer->read(&spectralData);
        REQUIRE(spectralData.size() == spectralDataInfo.getResolution());

        std::size_t maxMagnitudeSpectrumLine = getMaxMagnitudeSpectralLineIndex(spectralData);
        double maxMagnitudeFrequency = static_cast<double>(maxMagnitudeSpectrumLine) * frequencyResolutionInHz;
        maxPowerFrequencyDeviationSum += (maxMagnitudeFrequency - expectedFrequency) * (maxMagnitudeFrequency - expectedFrequency);
        count++;
    }
    return sqrt(maxPowerFrequencyDeviationSum / count);
}

SCENARIO("Wavelet Transformation Integration Test", "[integration]") {
    auto samplingRate = 44100;
    Transformation::ResolutionType resolution = 4096;
    auto frequencyResolutionInHz = (double) samplingRate * 0.5 / (double) resolution;

    GIVEN("wavelet transformation") {
        Transformation *transformation = TransformationFactory::getSingletonInstance().createTransformation(
                TransformationFactory::Type::FAST_WAVELET_TRANSFORM,
                samplingRate,
                resolution,
                WindowFunctionFactory::Method::BLACKMAN_HARRIS,
                AbstractWaveletTransformation::WaveletBase::VAIDYANATHAN_18);
        REQUIRE(transformation != nullptr);

        double frequency = static_cast<double>(resolution) / 100.0f;
        WHEN("sine signal with " << frequency << "Hz is analyzed") {
            generateTestSineInput(frequency, transformation, samplingRate, resolution);
            auto toleranceFactor = 4.0F;
            THEN("the frequency of the maximum magnitude spectral line is within " << toleranceFactor << " times " << frequencyResolutionInHz << "Hz (frequency resolution) of the expected frequency") {
                double maxPowerFrequencyDeviationToleranceInHz = frequencyResolutionInHz * toleranceFactor;
                double maxPowerFrequencyDeviationInHz = getMaxPowerFrequencyDeviation(transformation, frequency);

                CAPTURE(maxPowerFrequencyDeviationInHz, maxPowerFrequencyDeviationToleranceInHz, frequencyResolutionInHz);
                REQUIRE(maxPowerFrequencyDeviationInHz < maxPowerFrequencyDeviationToleranceInHz);
            }
        }
    }
}