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
// Factory                                                                            //
//------------------------------------------------------------------------------------//
class WindowFunctionFactory {
public:
    static auto getSingletonInstance() -> WindowFunctionFactory &;
    
    // Copy-constructors and move- and assignment-operator are deleted, because this class is a singleton.
    WindowFunctionFactory(WindowFunctionFactory const &) = delete;
    WindowFunctionFactory(WindowFunctionFactory &&) = delete;
    auto operator=(WindowFunctionFactory const &) -> WindowFunctionFactory & = delete;
    auto operator=(WindowFunctionFactory const &&) -> WindowFunctionFactory & = delete;

    auto createWindowFunction(int windowFunctionNr, long resolution) -> WindowFunction *;

private:
    WindowFunctionFactory() = default;
    ~WindowFunctionFactory();

    using TResolutionsMap = std::map<long, WindowFunction *>;
    using TResolutionsMapIterator = std::map<long, WindowFunction *>::const_iterator;
    using TResolutionsKeyValue = std::pair<long, WindowFunction *>;
    using TWindowFunctionsMap = std::map<int, TResolutionsMap>;
    using TWindowFunctionsMapIterator = std::map<int, TResolutionsMap>::const_iterator;
    using TWindowFunctionsKeyValue = std::pair<int, TResolutionsMap>;

    TWindowFunctionsMap windowingFunctions;

    auto readWindowFunctionFromMap(int windowFunctionNr, long resolution) -> WindowFunction *;
    void writeWindowFunctionIntoMap(int windowFunctionNr, long resolution, WindowFunction *pWindowFunction);
    void deleteWindowFunctionsMap();
};