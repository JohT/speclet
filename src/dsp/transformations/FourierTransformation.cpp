#include "FourierTransformation.h"

FourierTransformation::FourierTransformation(double samplingRate, ResolutionType newResolution, int windowFunctionNr)
    : Transformation(samplingRate, newResolution, windowFunctionNr) 
    , fftExecutePlanTimer(PerformanceTimer("FourierTransformation::calculate (fftw execute)")),
      fftInputCopyTimer(PerformanceTimer("FourierTransformation::calculate (input copy)")),
      fftOutputCopyTimer(PerformanceTimer("FourierTransformation::calculate (output copy)"))    
    {
    long frequencyResolution = (long) ((newResolution / 2.0) + 1.0);
    mSpectralDataInfo = new SpectralDataInfo(samplingRate, newResolution, frequencyResolution, 1);

    in = (double *) fftw_malloc(sizeof(double) * newResolution);
    out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * ((newResolution / 2) + 1));
    plan = fftw_plan_dft_r2c_1d(newResolution, in, out, FFTW_ESTIMATE);

    assert(plan);

    DBG("FourierTransformation::initialize done with fs/res=" + juce::String(mSamplingRate) + ",resolution=" + juce::String(newResolution));

    setReady();
    setCalculated();
};

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
    fftInputCopyTimer.start();
    auto resolution = getResolution();
    for (int i = 0; i < resolution; i++) {
        double nextSamplePerChannel = mInputQueue->front();
        *(in + i) = nextSamplePerChannel * mWindowFunction->getFactor(i);

        mInputQueue->pop();
    }
    fftInputCopyTimer.stop();

    fftExecutePlanTimer.start();
    fftw_execute(plan);
    fftExecutePlanTimer.stop();

    fftOutputCopyTimer.start();
    SpectralDataBuffer::ItemType spectrum;

    for (int i = 0; i < ((resolution / 2) + 1); i++) {
        //Loop for copying every single fft result data into the output-queue
        double realValue = *(out + i)[0];
        double imagValue = *(out + i)[1];
        double magnitude = sqrt(realValue * realValue + imagValue * imagValue) / resolution; /*1/N = Normalisation*/
        spectrum.push_back(magnitude);
    }
    getSpectralDataBuffer()->write(spectrum);
    fftOutputCopyTimer.stop();
}