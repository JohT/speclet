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
	------------------------------------------------------------------------------
  Most of the window function implementations in this class had been taken out of the fft c program 
  from George B. Moody (GNU General Public License, email: george@mit.edu, http://www.physionet.org/)
  ==============================================================================
*/
#pragma once
#include "WindowFunction.h"
#include <map>

//------------------------------------------------------------------------------------//
// Interface implementations                                                          //
//------------------------------------------------------------------------------------//
class WindowBartlett : public WindowFunction {
public:
    explicit WindowBartlett(unsigned long newResolution) : WindowFunction(newResolution) {}
    auto calculateFactor(unsigned long index) -> double override;//bartlett windowing function
    auto getName() -> const char * override { return "Barlett"; }
};

class WindowBlackman : public WindowFunction {
public:
    explicit WindowBlackman(unsigned long newResolution) : WindowFunction(newResolution) {}
    auto calculateFactor(unsigned long index) -> double override;//blackman windowing function
    auto getName() -> const char * override { return "Blackman"; }
};

class WindowBlackmanHarris : public WindowFunction {
public:
    explicit WindowBlackmanHarris(unsigned long newResolution) : WindowFunction(newResolution) {}
    auto calculateFactor(unsigned long index) -> double override;//blackman_harris windowing function
    auto getName() -> const char * override { return "Blackman Harris"; }
};

class WindowHamming : public WindowFunction {
public:
    explicit WindowHamming(unsigned long newResolution) : WindowFunction(newResolution) {}
    auto calculateFactor(unsigned long index) -> double override;//hamming windowing function
    auto getName() -> const char * override { return "Hamming"; }
};

class WindowHann : public WindowFunction {
public:
    explicit WindowHann(unsigned long newResolution) : WindowFunction(newResolution) {}
    auto calculateFactor(unsigned long index) -> double override;//hanning windowing function
    auto getName() -> const char * override { return "Hann"; }
};

class WindowParzen : public WindowFunction {
public:
    explicit WindowParzen(unsigned long newResolution) : WindowFunction(newResolution) {}
    auto calculateFactor(unsigned long index) -> double override;//parzen windowing function
    auto getName() -> const char * override { return "Parzen"; }
};

class WindowRectangular : public WindowFunction {
public:
    explicit WindowRectangular(unsigned long newResolution) : WindowFunction(newResolution){};
    auto calculateFactor(unsigned long index) -> double override;//square windowing function
    auto getName() -> const char * override { return "Rectangular"; };
};

class WindowWelch : public WindowFunction {
public:
    explicit WindowWelch(unsigned long newResolution) : WindowFunction(newResolution) {}
    auto calculateFactor(unsigned long index) -> double override;//welch windowing function
    auto getName() -> const char * override { return "Welch"; }
};