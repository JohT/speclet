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
#include "../windowing/WindowFunctions.h"
#include <assert.h>
#include <memory>
#include <queue>
#include <vector>

class TransformationListener;

class Transformation {
public:
    enum Constants {
        TIME_RESOLUTION_LIMIT = 8
    };

    using ResolutionType = unsigned long;
    
    Transformation() = delete;                                     //No default contructor
    Transformation(Transformation &) = delete;                     //No copy contructor
    Transformation(Transformation &&) = delete;                    //No move contructor
    auto operator=(Transformation &) -> Transformation & = delete; //No copy assignment
    auto operator=(Transformation &&) -> Transformation & = delete;//No move assignment

    Transformation(double newSamplingRate, ResolutionType newResolution, int newWindowFunctionNr);
    virtual ~Transformation();

    auto getWindowFunction() const -> WindowFunction *;
    void setWindowFunction(int windowFunctionNr);
    void setNextInputSample(double sample);
    auto isOutputAvailable() -> bool;
    auto getSpectralDataBuffer() -> SpectralDataBuffer *;
    auto getSpectralDataInfo() -> SpectralDataInfo * { return mSpectralDataInfo; }
    auto getTransformationNr() const -> int { return transformTypeNr; }
    void setTransformationNr(int newTransformTypeNr) { transformTypeNr = newTransformTypeNr; }
    void setTransformResultListener(TransformationListener *value);

    void getNextSpectrum(SpectralDataBuffer::ItemType *item);
    auto getSpectrumStatistics(SpectralDataBuffer::ItemType *item) -> SpectralDataBuffer::ItemStatisticsType;

protected:
     /**
     * @brief applies the transformation to the samples in the input queue and stores the result in the output queue
     */
   virtual void calculate() = 0;//abstract: must be implemented by inherited class!

    auto getResolution() const -> ResolutionType {
        return resolution;
    }

    void setReady(bool value = true) { ready = value; }
    void setCalculated(bool value = true) { calculated = value; }

    std::queue<double> *mInputQueue;
    SpectralDataBuffer *mOutputBuffer;
    SpectralDataInfo *mSpectralDataInfo;

private:
    int transformTypeNr;
    std::shared_ptr<WindowFunction> windowFunction;//Windowfunction-Interface for hanning, hamming, kaiser,...
    double samplingRate;

    ResolutionType resolution;

    bool ready;     //Signalizes internally "ready for new calculation"
    bool calculated;//Signalizes internally "calculation finished"

    void informListenersAboutTransformResults();
    void calculationFrame();

    TransformationListener *mTransformResultsListener;
    juce::CriticalSection criticalSection;
    juce::WaitableEvent *waitForDestruction;

    PerformanceTimer calculationFrameTimer, informListenersTimer, waitForDestructionTimer;
};

class TransformationListener {
public:
    virtual void onTransformationEvent(Transformation *value) = 0;//abstract
};