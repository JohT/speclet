#include "FourierTransformation.h"
#include "../../utilities/PerformanceManager.h"

FourierTransformation::FourierTransformation(double samplingRate, long resolution, int windowFunctionNr) 
	: Transformation(samplingRate, resolution, windowFunctionNr) 
{
	long frequencyResolution = (long)((mResolution/2.0)+1.0);
	mSpectralDataInfo = new SpectralDataInfo(samplingRate, resolution, frequencyResolution, 1);

	in			= (double*)			fftw_malloc(sizeof(double) * mResolution);
	out		= (fftw_complex*)	fftw_malloc(sizeof(fftw_complex) * ((mResolution/2)+1));
	plan		= fftw_plan_dft_r2c_1d(mResolution, in, out, FFTW_ESTIMATE);

	assert(plan);

	DBG(T("FourierTransformation::initialize done with fs/res=") + 
		juce::String(mSamplingRate) + ",resolution=" + juce::String(mResolution));	

	ready						= true;
	calculated				= true;
};

FourierTransformation::~FourierTransformation() {
	ready	= false;

	if (in)		fftw_free(in); 
	if (out)		fftw_free(out); 
	if (plan)	fftw_destroy_plan(plan);

	DBG(T("TransformFFT destructed"));
}

void FourierTransformation::calculate() {
	//Loop for copying every single sample from input-queue to fft inputarray
	for (int i = 0; i < mResolution; i++) {
		double nextSamplePerChannel = mInputQueue->front();
		*(in + i) = nextSamplePerChannel * mWindowFunction->getFactor(i);

		mInputQueue->pop();			
	}

	PerformanceManager::getSingletonInstance()->start(T("fftw_execute"));
	fftw_execute(plan);
	PerformanceManager::getSingletonInstance()->stop(T("fftw_execute"));

	SpectralDataBuffer::ItemType	spectrum;

	for (int i = 0; i < ((mResolution / 2) + 1); i++) {
	//Loop for copying every single fft result data into the output-queue
		double realValue = *(out + i)[0];
		double imagValue = *(out + i)[1];
		double magnitude = sqrt(realValue*realValue + imagValue*imagValue) / mResolution;  /*1/N = Normalisation*/
		spectrum.push_back(magnitude);
	}
	getSpectralDataBuffer()->write(spectrum);
}