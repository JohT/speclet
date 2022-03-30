#include "../src/dsp/transformations/TransformationFactory.h"
#include <catch2/catch_all.hpp>
#include <memory>
#include <string>

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

unsigned long getExpectedSpectralDataSize(const SpectralDataInfo &spectralDataInfo, int transformationTypeNumber) {
    auto expectedSpectralDataSize = spectralDataInfo.getResolution();
    if (transformationTypeNumber == TransformationFactory::Type::FAST_FOURIER_TRANSFORM) {
        expectedSpectralDataSize = spectralDataInfo.getResolution() / 2 + 1;
    } else if (transformationTypeNumber == TransformationFactory::Type::FAST_WAVELET_PACKET_TRANSFORM) {
        expectedSpectralDataSize = spectralDataInfo.getResolution() / 16;// Because of 4x frequency resolution ratio
    }
    INFO("Expected spectral data size: " << expectedSpectralDataSize << " for transformation type: " << transformationTypeNumber << " and resolution: " << spectralDataInfo.getResolution());
    return expectedSpectralDataSize;
}

double getFrequencyDeviationToleranceFactor(int transformationTypeNumber) {
    auto frequencyDeviationToleranceFactor = 4.0F;
    if (transformationTypeNumber == TransformationFactory::Type::FAST_FOURIER_TRANSFORM) {
        frequencyDeviationToleranceFactor = 5.0F;
    } else if (transformationTypeNumber == TransformationFactory::Type::FAST_WAVELET_PACKET_TRANSFORM) {
        frequencyDeviationToleranceFactor = 8.0F;
    } else if (transformationTypeNumber == TransformationFactory::Type::FAST_WAVELET_PACKET_BEST_BASIS_TRANSFORM) {
        frequencyDeviationToleranceFactor = 9.0F;
    }
    INFO("Frequency deviation tolerance factor: " << frequencyDeviationToleranceFactor << " for transformation type: " << transformationTypeNumber);
    return frequencyDeviationToleranceFactor;
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
    REQUIRE(spectralDataBuffer != nullptr);

    auto expectedSpectralDataSize = getExpectedSpectralDataSize(spectralDataInfo, transformation->getTransformationNr());
    int count = 0;
    double maxPowerFrequencyDeviationSum = 0.0;
    while (transformation->isOutputAvailable()) {
        SpectralDataBuffer::ItemType spectralData;
        spectralDataBuffer->read(&spectralData);
        REQUIRE(spectralData.size() == expectedSpectralDataSize);

        std::size_t maxMagnitudeSpectrumLine = getMaxMagnitudeSpectralLineIndex(spectralData);
        double maxMagnitudeFrequency = static_cast<double>(maxMagnitudeSpectrumLine) * frequencyResolutionInHz;
        maxPowerFrequencyDeviationSum += (maxMagnitudeFrequency - expectedFrequency) * (maxMagnitudeFrequency - expectedFrequency);
        count++;
    }
    return sqrt(maxPowerFrequencyDeviationSum / count);
}

SCENARIO("Transformations Integration Test", "[integration]") {
    auto samplingRate = 44100;
    Transformation::ResolutionType resolution = GENERATE(2048U, 4096U);
    auto frequencyResolutionInHz = static_cast<double>(samplingRate) * 0.5 / static_cast<double>(resolution);

    // Generates a list of all transformation except for "BYPASS" and calls the tests methods below for each of them.
    // See: https://github.com/catchorg/Catch2/blob/devel/docs/generators.md
    TransformationFactory::Type transformationType = static_cast<TransformationFactory::Type>(GENERATE(filter([](int typeNumber) { return typeNumber != TransformationFactory::Type::BYPASS; }, range(1, static_cast<int>(TransformationFactory::Type::NUMBER_OF_OPTIONS)))));
    // To test single transformations, comment out the above line and uncomment the following line:
    //TransformationFactory::Type transformationType = static_cast<TransformationFactory::Type>(GENERATE(value(TransformationFactory::Type::FAST_WAVELET_PACKET_BEST_BASIS_TRANSFORM)));
    Transformation *transformation = TransformationFactory::getSingletonInstance().createTransformation(
            transformationType,
            samplingRate,
            resolution,
            WindowFunctionFactory::Method::BLACKMAN_HARRIS,
            AbstractWaveletTransformation::WaveletBase::VAIDYANATHAN_18,
            WaveletPacketTransformation::ResolutionRatioOption::FREQUENCY_X4);
    REQUIRE(transformation != nullptr);
    REQUIRE(transformation->getTransformationNr() == transformationType);

    auto frequencyDeviationToleranceFactor = getFrequencyDeviationToleranceFactor(transformation->getTransformationNr());

    GIVEN(transformation->getName() << " with resolution " << resolution) {
        double frequency = static_cast<double>(resolution) / 100.0f;
        WHEN("sine signal with " << frequency << "Hz is analyzed") {
            generateTestSineInput(frequency, transformation, samplingRate, resolution);
            THEN("the frequency of the maximum magnitude spectral line is within " << frequencyDeviationToleranceFactor << " times " << frequencyResolutionInHz << "Hz (frequency resolution) of the expected frequency") {
                double maxPowerFrequencyDeviationToleranceInHz = frequencyResolutionInHz * frequencyDeviationToleranceFactor;
                double maxPowerFrequencyDeviationInHz = getMaxPowerFrequencyDeviation(transformation, frequency);
                CAPTURE(maxPowerFrequencyDeviationInHz, maxPowerFrequencyDeviationToleranceInHz, frequencyResolutionInHz);
                REQUIRE(maxPowerFrequencyDeviationInHz < maxPowerFrequencyDeviationToleranceInHz);
            }
        }
    }
    TransformationFactory::getSingletonInstance().destruct();
}

TEST_CASE("Transformations Performance Test", "[.performance]") {
    auto samplingRate = 44100;
    Transformation::ResolutionType resolution = 4096;
    double frequency = static_cast<double>(resolution) / 100.0f;
    BENCHMARK_ADVANCED("Fast Fourier Transform Performance")(Catch::Benchmark::Chronometer meter) {
        Transformation *transformation = TransformationFactory::getSingletonInstance().createTransformation(
                TransformationFactory::Type::FAST_FOURIER_TRANSFORM,
                samplingRate,
                resolution,
                WindowFunctionFactory::Method::BLACKMAN_HARRIS,
                AbstractWaveletTransformation::WaveletBase::VAIDYANATHAN_18,
                WaveletPacketTransformation::ResolutionRatioOption::FREQUENCY_X4);
        generateTestSineInput(frequency, transformation, samplingRate, resolution);
        getMaxPowerFrequencyDeviation(transformation, frequency);
        meter.measure([transformation, frequency] { return getMaxPowerFrequencyDeviation(transformation, frequency); });
    };
    TransformationFactory::getSingletonInstance().destruct();
}