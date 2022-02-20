#pragma once
#include "WaveletTransformation.h"
#include "../../utilities/PerformanceManager.h"

WaveletTransformation::WaveletTransformation(
	double samplingRate, 
	long resolution, 
	int windowFunctionNr,
	int waveletBaseTypeNr) 
	: AbstractWaveletTransformation(samplingRate, resolution, windowFunctionNr, waveletBaseTypeNr) 
{
	mFrequencyResolution		= resolution;
	mTimeResolution			= resolution / 2;	
	mSpectralDataInfo			= new SpectralDataInfo(samplingRate, resolution, mFrequencyResolution, mTimeResolution);

	DBG(T("WaveletTransformation::initialize done with fs=")	+ 
								  juce::String(mSamplingRate)			+
		T(",res=")			+ juce::String(mResolution)	
	);

	ready			= true;
	calculated	= true;
};

WaveletTransformation::~WaveletTransformation() {
	ready	= false;
	DBG(T("WaveletTransformation destructed"));
}

void WaveletTransformation::calculate() {
	//fills the mDWT_Input with data from the inputQueue
	fillDWTInput();
	//output data container to hold the result of the wavelet transformation ("coefficients")
	Interval outDWT(0, mResolution-1);
	//fast wavelet transform
	PerformanceManager::getSingletonInstance()->start(T("fwt"));
	WaveTrans(*mDWT_Input, outDWT, mDWT_filter_H, mDWT_filter_G, ConvDecPer);
	PerformanceManager::getSingletonInstance()->stop(T("fwt"));
	//fills the outputQueue with the spectral data (in a ready to draw order)
	extractSpectrum(outDWT);
}