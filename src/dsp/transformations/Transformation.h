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
#include "../windowing/WindowFunctionFactory.h"
#include "../windowing/WindowFunctions.h"
#include "juce_core/juce_core.h"
#include "juce_dsp/juce_dsp.h"
#include <memory>
#include <queue>

class TransformationListener;
class TransformationResult {
public:
    virtual auto isOutputAvailable() -> bool = 0;
    virtual auto getSpectralDataBuffer() -> SpectralDataBuffer * = 0;
    virtual void getNextSpectrum(SpectralDataBuffer::ItemType *item) = 0;
    /**
     * @brief Get the spectral data info that contains amongst others details about the time and frequency resolution.
     * 
     * @return const SpectralDataInfo& 
     */
    virtual auto getSpectralDataInfo() -> const SpectralDataInfo & = 0;
};

class Transformation : public TransformationResult {
public:
    enum Constants {
        TIME_RESOLUTION_LIMIT = 8,
        WAIT_FOR_DESTRUCTION_TIMEOUT = 3000
    };

    using ResolutionType = unsigned long;

    Transformation() = delete;                                     //No default contructor
    Transformation(Transformation &) = delete;                     //No copy contructor
    Transformation(Transformation &&) = delete;                    //No move contructor
    auto operator=(Transformation &) -> Transformation & = delete; //No copy assignment
    auto operator=(Transformation &&) -> Transformation & = delete;//No move assignment

    Transformation(double newSamplingRate, ResolutionType newResolution, WindowFunctionFactory::Method newWindowFunction = WindowFunctionFactory::Method::DEFAULT);
    virtual ~Transformation();

    auto getWindowFunction() const -> WindowFunction *;

    /**
     * @brief Loads or replaces the window function with the given number (see class WindowFunctionsFactory)
     * 
     * @param newWindowFunction 
     */
    void setWindowFunction(WindowFunctionFactory::Method newWindowFunction);

    /**
     * @brief Get the input queue containing the next samples to be transformed
     * 
     * @return const std::queue<double>& 
     */
    auto getInputQueue() -> std::queue<double> &;

    /**
     * @brief Gathers the next input sample
     * 
     * @param sample 
     */
    void setNextInputSample(const double &sample);
    auto isOutputAvailable() -> bool;
    auto getSpectralDataBuffer() -> SpectralDataBuffer *;
    auto getTransformationNr() const -> int { return transformTypeNr; }
    void setTransformationNr(int newTransformTypeNr) { transformTypeNr = newTransformTypeNr; }
    void setTransformResultListener(TransformationListener *value);
    void getNextSpectrum(SpectralDataBuffer::ItemType *item);

    /**
     * @brief Gets the name of the transformation
     * 
     * @return const char* 
     */
    virtual auto getName() -> const char * = 0;

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

private:
    int transformTypeNr;
    std::shared_ptr<WindowFunction> windowFunction;//Windowfunction-Interface for hanning, hamming, kaiser,...

    ResolutionType resolution;

    std::queue<double> inputQueue;
    SpectralDataBuffer outputBuffer;

    bool ready;     //Signalizes internally "ready for new calculation"
    bool calculated;//Signalizes internally "calculation finished"

    TransformationListener *transformResultsListener;
    juce::CriticalSection criticalSection;
    juce::WaitableEvent waitForDestruction{true};

    void informListenersAboutTransformResults();

    /**
     * @brief Meant to be called on every new sample to be transformed.
     * Contains the sequence of actions associated with the calculation, 
     * e.g.: enough data? - ready? - calculate - informListeners,...
     */
    void calculationFrame();
};

class TransformationListener {
public:
    virtual void onTransformationEvent(TransformationResult *result) = 0;
};
