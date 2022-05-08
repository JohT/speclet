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
#include <vector>
class WindowFunction {
public:
    explicit WindowFunction(unsigned long resolution);

    virtual ~WindowFunction() = default;
    WindowFunction() = delete;                                          //explicit no default contructor;
    WindowFunction(const WindowFunction &) = default;                    //no copy contructor;
    WindowFunction(WindowFunction &&) = delete;                         //no move contructor;
    auto operator=(const WindowFunction &) -> WindowFunction & = default;//no copy assignment;
    auto operator=(WindowFunction &&) -> WindowFunction & = delete;     //no move assignment;

    auto getFactor(unsigned long index) -> double;
    auto getWindow() -> double *;
    virtual auto getName() -> const char * = 0;

protected:
    constexpr static const double PIx2 = 6.283185307179586476925286766559;
    
    virtual auto calculateFactor(unsigned long int index) -> double = 0;//will be redefined by the derived windowing function
    [[nodiscard]] auto getWindowSize() const -> double {
        return static_cast<double>(resolution - 1);
    }

private:
    unsigned long resolution;//also known as N or FFT-size or length of signal piece array or (window size + 1)
    std::vector<double> factors;
};