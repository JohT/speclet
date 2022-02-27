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
class WindowBartlett : WindowFunction {
public:
    explicit WindowBartlett(long newResolution) : WindowFunction(newResolution) {}
    auto calculateFactor(int index) -> double override;//bartlett windowing function
    auto getName() -> char * override { return "Barlett"; }
};

class WindowBlackman : WindowFunction {
public:
    explicit WindowBlackman(long newResolution) : WindowFunction(newResolution) {}
    auto calculateFactor(int index) -> double override;//blackman windowing function
    auto getName() -> char * override { return "Blackman"; }
};

class WindowBlackmanHarris : WindowFunction {
public:
    explicit WindowBlackmanHarris(long newResolution) : WindowFunction(newResolution) {}
    auto calculateFactor(int index) -> double override;//blackman_harris windowing function
    auto getName() -> char * override { return "Blackman Harris"; }
};

class WindowHamming : WindowFunction {
public:
    explicit WindowHamming(long newResolution) : WindowFunction(newResolution) {}
    auto calculateFactor(int index) -> double override;//hamming windowing function
    auto getName() -> char * override { return "Hamming"; }
};

class WindowHanning : WindowFunction {
public:
    explicit WindowHanning(long newResolution) : WindowFunction(newResolution) {}
    auto calculateFactor(int index) -> double override;//hanning windowing function
    auto getName() -> char * override { return "Hanning"; }
};

class WindowParzen : WindowFunction {
public:
    explicit WindowParzen(long newResolution) : WindowFunction(newResolution) {}
    auto calculateFactor(int index) -> double override;//parzen windowing function
    auto getName() -> char * override { return "Parzen"; }
};

class WindowSquare : WindowFunction {
public:
    explicit WindowSquare(long newResolution) : WindowFunction(newResolution){};
    auto calculateFactor(int index) -> double override;//square windowing function
    auto getName() -> char * override { return "Square"; };
};

class WindowWelch : WindowFunction {
public:
    explicit WindowWelch(long resnewResolutionolution) : WindowFunction(resnewResolutionolution) {}
    auto calculateFactor(int index) -> double override;//welch windowing function
    auto getName() -> char * override { return "Welch"; }
};