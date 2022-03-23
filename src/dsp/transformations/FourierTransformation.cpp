#include "FourierTransformation.h"
#include "../../utilities/PerformanceLogger.h"
#include <cstddef>


FourierTransformation::FourierTransformation(double newSamplingRate, ResolutionType newResolution, WindowFunctionFactory::Method newWindowFunction)
    : Transformation(newSamplingRate, newResolution, newWindowFunction),
      in(static_cast<double *>(fftw_malloc(sizeof(double) * newResolution))),
      out(static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * ((newResolution / 2) + 1)))),
      fftExecutePlanTimer(PerformanceTimer("FourierTransformation::calculate (fftw execute)")),
      fftInputCopyTimer(PerformanceTimer("FourierTransformation::calculate (input copy)")),
      fftOutputCopyTimer(PerformanceTimer("FourierTransformation::calculate (output copy)")),
      spectralDataInfo(newSamplingRate, newResolution, (newResolution / 2 + 1), 1) {

    plan = fftw_plan_dft_r2c_1d(static_cast<int>(newResolution), in, out, FFTW_ESTIMATE);

    assert(plan);

    DBG("FourierTransformation::initialize done with fs/res=" + juce::String(newSamplingRate) + ",resolution=" + juce::String(newResolution));

    setReady();
    setCalculated();
}

FourierTransformation::~FourierTransformation() {
    setReady(false);

    if (in != nullptr) {
        fftw_free(in);
    }
    if (out != nullptr) {
        fftw_free(out);
    }
    if (plan != nullptr) {
        fftw_destroy_plan(plan);
    }

    DBG("TransformFFT destructed");
}

void FourierTransformation::calculate() {
    //Loop for copying every single sample from input-queue to fft inputarray
    auto resolution = getResolution();
    fftInputCopyTimer.start();
    {
        LOG_PERFORMANCE_OF_SCOPE("FourierTransformation calculate fftInputCopy");
        for (std::size_t i = 0; i < resolution; i++) {
            auto nextSamplePerChannel = getInputQueue().front();
            *(in + i) = nextSamplePerChannel * getWindowFunction()->getFactor(i);

            getInputQueue().pop();
        }
    }
    fftInputCopyTimer.stop();
    fftExecutePlanTimer.start();
    {
        LOG_PERFORMANCE_OF_SCOPE("FourierTransformation calculate fftw_execute");
        fftw_execute(plan);
    }
    fftExecutePlanTimer.stop();
    fftOutputCopyTimer.start();
    {
        LOG_PERFORMANCE_OF_SCOPE("FourierTransformation calculate fftOutputCopy");
        SpectralDataBuffer::ItemType spectrum;

        for (std::size_t i = 0; i < ((resolution / 2) + 1); i++) {
            //Loop for copying every single fft result data into the output-queue
            double realValue = *(out + i)[0];
            double imaginaryValue = *(out + i)[1];
            double magnitude = sqrt(realValue * realValue + imaginaryValue * imaginaryValue);
            double normalizedMagnitude = magnitude / static_cast<double>(resolution);// 1/N = Normalisation
            spectrum.push_back(static_cast<SpectralDataBuffer::ValueType>(normalizedMagnitude));
        }
        getSpectralDataBuffer()->write(spectrum);
    }
    fftOutputCopyTimer.stop();
}