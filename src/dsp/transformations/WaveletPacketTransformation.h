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
#include "../../plugin/SpectronParameters.h"
#include "AbstractWaveletTransformation.h"


class WaveletPacketTransformation : public AbstractWaveletTransformation {
public:
	WaveletPacketTransformation(
		double samplingRate, 
		ResolutionType newResolution, 
		int windowFunctionNr		= SpectronParameters::WINDOWING_DEFAULT,
		int waveletBaseTypeNr	= SpectronParameters::WAVELET_DEFAULT, 
		int resolutionRatioDWPT = SpectronParameters::RESOLUTION_RATIO_DEFAULT
	);
	virtual ~WaveletPacketTransformation(void);
	void	setResolutionRatioDWPT	(int resolutionRatioDWPT);

protected:
	virtual void calculate();		

	int	getWaveletPacketResultTreeLevel	(int maxLevel, int resolutionRatioDWPT);
	int	getFrequencyResolution				(int maxLevel);
	int	getTimeResolution						(void);

private:
	int	mDWPT_ResultTreeLevel;		//Wavelet packet transform result tree level, 
	int	mResolutionRatioDWPT;		//Wavelet packet transform time/freq resolution ratio (0=equal time&freq)
};
