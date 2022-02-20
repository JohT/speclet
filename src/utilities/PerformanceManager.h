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
	#define LOG(logtext) {JUCE_NAMESPACE::String tempLogBuf; tempLogBuf << logtext; JUCE_NAMESPACE::Logger::writeToLog(tempLogBuf);}
#else	 
	#define LOG(logtext)
#endif

#include "..\..\libs\juce\JuceLibraryCode\JuceHeader.h"
#include <map>

/*
The class PerformanceManager is a global container for juce::PerformanceCounter-objects.
It's ment to be an easy way to start and stop performance measures without bothering about
creation and scope of the PerformanceCounter. 
Internally, every new PerformanceCounter is added to a std::map.
*/
class PerformanceManager {
public:
	static PerformanceManager* getSingletonInstance();
	void destruct();
	void start(juce::String name, int runsPerPrintout = 100, const juce::File &loggingFile = juce::File::nonexistent);
	void stop(juce::String name);

private:
	typedef std::map	<juce::String, juce::PerformanceCounter*>							TMap;
	typedef std::map	<juce::String, juce::PerformanceCounter*>::const_iterator	TIterator;
	typedef std::pair	<juce::String, juce::PerformanceCounter*>							TKeyValue;

	static PerformanceManager* singletonInstance;
	TMap map; 

	PerformanceManager(void) {};
	~PerformanceManager(void) {};
	PerformanceManager(const PerformanceManager&);

	void add(juce::String name, int runsPerPrintout = 100, const juce::File &loggingFile = juce::File::nonexistent);
	juce::PerformanceCounter* get(juce::String name);

	void deletePerformanceManager();
};