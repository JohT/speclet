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

#if _DEBUG
#define _PERFORMACETEST 1
#endif
#if _LOGTOFILE
#define LOG(logtext)                                    \
    {                                                   \
        JUCE_NAMESPACE::String tempLogBuf;              \
        tempLogBuf << logtext;                          \
        JUCE_NAMESPACE::Logger::writeToLog(tempLogBuf); \
    }
#else
#define LOG(logtext)
#endif

#include "JuceHeader.h"
#include <map>

/*
The class PerformanceManager is a global container for juce::PerformanceCounter-objects.
It's meant to be an easy way to start and stop performance measures without bothering about
creation and scope of the PerformanceCounter. 
Internally, every new PerformanceCounter is added to a std::map.
*/
class PerformanceManager {
public:
    static auto getSingletonInstance() -> PerformanceManager &;

    void start(const juce::String &name, int runsPerPrintout = 100, const juce::File &loggingFile = juce::File());
    void stop(const juce::String &name);

    PerformanceManager(PerformanceManager const &) = default;
    PerformanceManager(PerformanceManager &&) = default;
    auto operator=(PerformanceManager const &) -> PerformanceManager & = default;

private:
    PerformanceManager() = default;
    ~PerformanceManager() = default;

    using TMap = std::map<juce::String, juce::PerformanceCounter &>;
    using TIterator = std::map<juce::String, juce::PerformanceCounter &>::const_iterator;
    using TKeyValue = std::pair<juce::String, juce::PerformanceCounter &>;
    TMap map;

    void add(const juce::String &name, int runsPerPrintout = 100, const juce::File &loggingFile = juce::File());
    auto get(const juce::String &name) -> juce::PerformanceCounter &;

    void deletePerformanceManager();
};