#include "FourierTransformation.h"

FourierTransformation::FourierTransformation(double samplingRate, long resolution, int windowFunctionNr)
    : Transformation(samplingRate, resolution, windowFunctionNr) 
    , fftExecutePlanTimer(PerformanceTimer("FourierTransformation::calculate (fftw execute)")),
      fftInputCopyTimer(PerformanceTimer("FourierTransformation::calculate (input copy)")),
      fftOutputCopyTimer(PerformanceTimer("FourierTransformation::calculate (output copy)"))    
    {
    long frequencyResolution = (long) ((mResolution / 2.0) + 1.0);
    mSpectralDataInfo = new SpectralDataInfo(samplingRate, resolution, frequencyResolution, 1);

    in = (double *) fftw_malloc(sizeof(double) * mResolution);
    out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * ((mResolution / 2) + 1));
    plan = fftw_plan_dft_r2c_1d(mResolution, in, out, FFTW_ESTIMATE);

    assert(plan);

    DBG("FourierTransformation::initialize done with fs/res=" + juce::String(mSamplingRate) + ",resolution=" + juce::String(mResolution));

    ready = true;
    calculated = true;
};

FourierTransformation::~FourierTransformation() {
    ready = false;

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
    for (int i = 0; i < mResolution; i++) {
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

    for (int i = 0; i < ((mResolution / 2) + 1); i++) {
        //Loop for copying every single fft result data into the output-queue
        double realValue = *(out + i)[0];
        double imagValue = *(out + i)[1];
        double magnitude = sqrt(realValue * realValue + imagValue * imagValue) / mResolution; /*1/N = Normalisation*/
        spectrum.push_back(magnitude);
    }
    getSpectralDataBuffer()->write(spectrum);
    fftOutputCopyTimer.stop();
}