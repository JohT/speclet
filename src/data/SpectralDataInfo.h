/*
  ==============================================================================
  This file is part of the VST spectrum analyzer plugin "speclet" (working title)
  Copyright 2011 by Johannes Troppacher
	------------------------------------------------------------------------------
  This file may use parts of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.
	------------------------------------------------------------------------------
  This file may use parts of the fftw library
  Copyright 2003 Matteo Frigo, Copyright 2003 Massachusetts Institute of Technology
	------------------------------------------------------------------------------
  This file may use parts of the wave++ library
  Copyright 1999 by S. E. Ferrando, L. A. Kolasa and N. Kovacevic
  ==============================================================================
*/
#pragma once
#include <vector>

class SpectralDataInfo {
public: 
	SpectralDataInfo(
		double	samplingRate, 
		long		resolution, 
		long		frequencyResolution = 0, 
		long		timeResolution = 1,
		double	frequencyPartitionRatio = 0.0
	);
	~SpectralDataInfo(void);
	bool operator == (SpectralDataInfo& compareObject);

	double	getSamplingFrequency(void);
	double	getMaxFrequency(void);
	long		getResolution(void);
	long		getFrequencyResolution(void);				//spectral line count within mSamplingFrequency/2 Hz ("vertical resolution")
	long		getTimeResolution(void);					//spectral line count within 1/mResolution ms ("horizontal resolution")
	double	getTimeResolutionMs(void);					//time resolution in ms (best resolution in case of non lin time res.)
	double	getFrequencyPartitionSize(void);			//(e.g. 1/32 -> partition [Hz] = mSamplingFrequency/2 split into 1/32)	
	double	getSpectralLineFrequencyPartitionSize (long spectralLineNr);	

private:
	SpectralDataInfo(const SpectralDataInfo&);						// no copy constructor
	SpectralDataInfo& operator = (const SpectralDataInfo&);		// no assign operator
	SpectralDataInfo(void) {};												// no default constructor
	
	double	mSamplingFrequency;
	long		mResolution;
	long		mFrequencyResolution;	
	long		mTimeResolution;				
	//fields for non constant frequency partitions
	double	mFrequencyPartitionSize;	
	//calculated fields
	double	mMaxFrequency;
	double	mTimeResolutionMs;	
};

