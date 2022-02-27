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

//------------------------------------------------------------------------------------//
// Interface as abstract class                                                        //
//------------------------------------------------------------------------------------//
class WindowFunction {
public:
    WindowFunction() = delete;//explicit no default contructor;
    explicit WindowFunction(long resolution);
    ~WindowFunction();

    auto getFactor(int index) -> double;
    auto getWindow() -> double *;
    virtual auto getName() -> char * = 0;

    constexpr static const double PIx2 = 6.283185307179586476925286766559;

protected:
    virtual auto calculateFactor(int index) -> double = 0;//will be redefined by the derived windowing function
    auto getWindowSize() -> double;
    double *windowFunctionFactors;

private:
    WindowFunction(const WindowFunction &) = default;                    //default copy contructor;
    WindowFunction(WindowFunction &&) = default;                         //default move contructor;
    auto operator=(const WindowFunction &) -> WindowFunction & = default;//default copy assignment;
    auto operator=(WindowFunction &&) -> WindowFunction & = default;     //default move assignment;

    long resolution;//also known as N or FFT-size or length of signal piece array
};