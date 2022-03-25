#include "TestCases.h"
#include <cstddef>

int test_DWPT_1(TCommonSettings settings) {
    TCommonSettings localSettings = setCommonSettings(settings.samplingRate, 16, settings.channels);

    Transformation *transformation = TransformationFactory::getSingletonInstance().createTransformation(
            TransformationFactory::Type::FAST_FOURIER_TRANSFORM,
            localSettings.samplingRate,
            localSettings.resolution,
            WindowFunctionFactory::Method::HANN,
            AbstractWaveletTransformation::WaveletBase::DAUBECHIES_02);
    assert(transformation);

    double inputData[] = {32.0, 10.0, 20.0, 38.0, 37.0, 28.0, 38.0, 34.0,
                          18.0, 24.0, 18.0, 9.00, 23.0, 24.0, 28.0, 34.0};

    //expected output after sqrt on every value and for WAVELET_DAUBECHIES_02
    //-> signs get lost since they come as x*x
    //-> columns and rows are swapped
    //[2,0]= +59.00  +52.50  +59.00  +37.00
    //[2,1]=  +7.00  +22.50   -7.00   -5.00
    //[2,2]=  -3.00   -5.50  -13.00  +10.00
    //[2,3]=  +1.00   +4.50   -3.00   +4.00

    for (std::size_t i = 0; i < localSettings.resolution; i++) {
        transformation->setNextInputSample(inputData[i]);
    }

    SpectralDataBuffer *spectralDataBuffer = transformation->getSpectralDataBuffer();
    int dataAvailableCounter = 1;
    while (transformation->isOutputAvailable()) {
        SpectralDataBuffer::ItemType spectralData;
        spectralDataBuffer->read(&spectralData);
        assert(spectralData.size() == (settings.resolution / 2 + 1));

        for (int i = 0; i < spectralData.size(); i++) {
            printf("[%1d,%1d]=%6.2lf ", dataAvailableCounter, i, sqrt(spectralData[i]));
        }
        printf("\n");
        dataAvailableCounter++;
    }

    printf("\nclosed\ntest successful!\n");
    fflush(stdout);

    //tear down
    TransformationFactory::getSingletonInstance().destruct();

    //TODO (JohT) Missing Test Assertion
    return 0;
}