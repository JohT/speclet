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

#include <assert.h>
#include <queue>
#include <vector>
#include "../../data/SpectralDataInfo.h"
#include "../../data/SpectralDataBuffer.h" 
#include "../WindowFunctions.h"
#include "../../plugin/SpectronParameters.h"

class TransformationListener;

class Transformation {
public:
    friend class TransformationFactory;
    enum Constants {
        TIME_RESOLUTION_LIMIT = 8
    };
    Transformation(double samplingRate, long resolution, int windowFunctionNr = SpectronParameters::WINDOWING_DEFAULT);
	virtual ~Transformation(void);

	void						setWindowFunction		(int windowFunctionNr);
	void						setNextInputSample	(double sample);
	bool						isOutputAvailable		(void);	
	SpectralDataBuffer*	getSpectralDataBuffer(void);
	SpectralDataInfo*		getSpectralDataInfo	(void) {return mSpectralDataInfo;};
	int						getTransformationNr	(void) {return mTransformTypeNr;};
	void						setTransformationNr	(int transformTypeNr) {mTransformTypeNr = transformTypeNr;};
	void						setTransformResultListener	(TransformationListener* value);

	void getNextSpectrum (SpectralDataBuffer::ItemType* item);
	SpectralDataBuffer::ItemStatisticsType	getSpectrumStatistics (SpectralDataBuffer::ItemType* item);

private:
	void	informListenersAboutTransformResults();
	void	calculationFrame();

	Transformation(void);					//No default contructor
	Transformation(Transformation &);	//No copy contructor
	TransformationListener*	mTransformResultsListener;
	juce::CriticalSection	criticalSection;
	juce::WaitableEvent*		waitForDestruction;

protected:
	virtual void calculate()= 0;			//abstract: must be implemented by inherited class!

	long			mResolution;
	long			mFrequencyResolution;
	long			mTimeResolution;
	double		mSamplingRate;
	int			mTransformTypeNr;

	bool			ready;				//Signalizes internally "ready for new calculation"
	bool			calculated;			//Signalizes internally "calculation finished"
	
	std::queue<double>*		mInputQueue;
	SpectralDataBuffer*		mOutputBuffer;
	SpectralDataInfo*			mSpectralDataInfo;
	WindowFunction*			mWindowFunction; //Windowfunction-Interface for hanning, hamming, kaiser,...
};

class TransformationListener {
public:
	virtual void onTransformationEvent(Transformation* value) = 0;	//abstract
};