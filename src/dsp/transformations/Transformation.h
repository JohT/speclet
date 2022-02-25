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

#include "../../data/SpectralDataBuffer.h"
#include "../../data/SpectralDataInfo.h"
#include "../../plugin/SpectronParameters.h"
#include "../WindowFunctions.h"
#include <assert.h>
#include <queue>
#include <vector>


class TransformationListener;

class Transformation {
public:
    friend class TransformationFactory;
    enum Constants {
        TIME_RESOLUTION_LIMIT = 8
    };
    Transformation() = delete;                //No default contructor
    Transformation(Transformation &) = delete;//No copy contructor
    Transformation(double samplingRate, long resolution, int windowFunctionNr = SpectronParameters::WINDOWING_DEFAULT);
    virtual ~Transformation();

    void setWindowFunction(int windowFunctionNr);
    void setNextInputSample(double sample);
    auto isOutputAvailable() -> bool;
    auto getSpectralDataBuffer() -> SpectralDataBuffer *;
    auto getSpectralDataInfo() -> SpectralDataInfo * { return mSpectralDataInfo; };
    auto getTransformationNr() const -> int { return mTransformTypeNr; }
    void setTransformationNr(int transformTypeNr) { mTransformTypeNr = transformTypeNr; };
    void setTransformResultListener(TransformationListener *value);

    void getNextSpectrum(SpectralDataBuffer::ItemType *item);
    auto getSpectrumStatistics(SpectralDataBuffer::ItemType *item) -> SpectralDataBuffer::ItemStatisticsType;

private:
    void informListenersAboutTransformResults();
    void calculationFrame();

    TransformationListener *mTransformResultsListener;
    juce::CriticalSection criticalSection;
    juce::WaitableEvent *waitForDestruction;

protected:
    virtual void calculate() = 0;//abstract: must be implemented by inherited class!

    long mResolution;
    long mFrequencyResolution;
    long mTimeResolution;
    double mSamplingRate;
    int mTransformTypeNr;

    bool ready;     //Signalizes internally "ready for new calculation"
    bool calculated;//Signalizes internally "calculation finished"

    std::queue<double> *mInputQueue;
    SpectralDataBuffer *mOutputBuffer;
    SpectralDataInfo *mSpectralDataInfo;
    WindowFunction *mWindowFunction;//Windowfunction-Interface for hanning, hamming, kaiser,...
};

class TransformationListener {
public:
    virtual void onTransformationEvent(Transformation *value) = 0;//abstract
};