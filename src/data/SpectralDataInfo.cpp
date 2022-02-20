#include "SpectralDataInfo.h"
#include "../dsp/transformations/Transformation.h"
#include <math.h>
#include <assert.h>
#include "../../libs/juce/JuceLibraryCode/JuceHeader.h"

SpectralDataInfo::SpectralDataInfo(
	double	samplingRate, 
	long		resolution, 
	long		frequencyResolution, 
	long		timeResolution,
	double	frequencyPartitionSize)

:	mSamplingFrequency(samplingRate),	
	mResolution(resolution),
	mFrequencyResolution(frequencyResolution),
	mTimeResolution(timeResolution),
	mFrequencyPartitionSize(frequencyPartitionSize),
	mMaxFrequency(samplingRate / 2.0),
	mTimeResolutionMs(mResolution / mTimeResolution / mSamplingFrequency * 1000)
{
	if (frequencyResolution == 0) {
	//default for frequencyResolution (fits for FFT)
		mFrequencyResolution = (long)((samplingRate/2.0)+1.0);
	}
	if (frequencyPartitionSize == 0) {
		//default for frequencyResolution (fits for FFT)
		mFrequencyPartitionSize = 1.0/(double)frequencyResolution;
	}
	if (mTimeResolution > Transformation::TIME_RESOLUTION_LIMIT) {
		mTimeResolution = Transformation::TIME_RESOLUTION_LIMIT;
	}

	assert(samplingRate > 100);
	assert(frequencyResolution >= 1);
	assert(mFrequencyPartitionSize > 0.0000001);

	DBG(T("SpectralDataInfo constructed with res=")		+ 
							juce::String(resolution)			+
		T(",fres=") + juce::String(frequencyResolution) +
		T(",tres=")	+ juce::String(timeResolution)		+
		T(",part=")	+ juce::String(mFrequencyPartitionSize)
	);
};

SpectralDataInfo::~SpectralDataInfo(void) {
}

bool SpectralDataInfo::operator == (SpectralDataInfo& compareObject) {
	if (!&compareObject) return false;
	if (mSamplingFrequency					!= compareObject.getSamplingFrequency())					return false;
	if (mResolution							!= compareObject.getResolution())							return false;
	if (mFrequencyResolution				!= compareObject.getFrequencyResolution())				return false;
	if (mTimeResolution						!= compareObject.getTimeResolution())						return false;
	if (mFrequencyPartitionSize			!= compareObject.getFrequencyPartitionSize())			return false;

	return true;
}

double SpectralDataInfo::getSamplingFrequency(void) {
	return mSamplingFrequency;
};

double SpectralDataInfo::getMaxFrequency(void) {
	return mMaxFrequency;
};

long SpectralDataInfo::getResolution(void) {
	return mResolution;
};

//gets the spectral line count within mSamplingFrequency/2 Hz
long SpectralDataInfo::getFrequencyResolution(void) {
	return mFrequencyResolution;
};

//gets the spectral line count within 1/mResolution ms
long SpectralDataInfo::getTimeResolution(void) {
	return mTimeResolution;
};	

//gets the time resolution in ms (best resolution in case of non lin time res.)
double SpectralDataInfo::getTimeResolutionMs(void) {
	return mTimeResolutionMs;
};	

//gets the freq. partition (e.g. 1/32 -> partition [Hz] = mSamplingFrequency/2 split into 1/32)
double SpectralDataInfo::getFrequencyPartitionSize(void) {
	return mFrequencyPartitionSize;
};	

double SpectralDataInfo::getSpectralLineFrequencyPartitionSize (long spectralLineNr) {
//e.g. FFT mFrequencyPartitionSize=1/16: 1/16, 1/16, 1/16....
//e.g. DWT mFrequencyPartitionSize=1/16, Progression=2: 1/16, 1/16, 1/8, 1/4, 1/2
	assert(spectralLineNr >= 0);
	return mFrequencyPartitionSize;
}