#include "TestCases.h"
#include <cstddef>

int test_DWPT_BestBasis(TCommonSettings settings) {
    TCommonSettings localSettings = setCommonSettings(settings.samplingRate, 16, settings.channels);

    Transformation *transform = TransformationFactory::getSingletonInstance().createTransformation(
            TransformationFactory::Type::FAST_WAVELET_PACKET_BEST_BASIS_TRANSFORM,
            localSettings.samplingRate,
            localSettings.resolution,
            WindowFunctionFactory::Method::RECTANGULAR,
            AbstractWaveletTransformation::WaveletBase::DAUBECHIES_08);
    assert(transform);

    double inputData[] = {32.0, 10.0, 20.0, 38.0, 37.0, 28.0, 38.0, 34.0,
                          18.0, 24.0, 18.0, 9.00, 23.0, 24.0, 28.0, 34.0};

    for (std::size_t i = 0; i < localSettings.resolution; i++) {
        transform->setNextInputSample(inputData[i] / 100.0);
    }

    SpectralDataBuffer *spectralDataQueue = transform->getSpectralDataBuffer();
    int dataAvailableCounter = 1;
    while (transform->isOutputAvailable()) {
        SpectralDataBuffer::ItemType spectrum;
        spectralDataQueue->read(&spectrum);

        auto spectralLinesCount = spectrum.size();
        assert(spectralLinesCount > 0);

        for (std::size_t i = 0; i < spectralLinesCount; i++) {
            printf("[%2d,%2d]=%6.2lf ", dataAvailableCounter, i, sqrt(spectrum[i]));
            if (((i + 1) % 4) == 0) printf("\n");
        }
        printf("-------------------------------------------------------------------\n");
        dataAvailableCounter++;
    }

    printf("\nclosed\ntest successful!\n");
    fflush(stdout);

    //TODO (JohT) Missing Test Assertion
    return 0;
}