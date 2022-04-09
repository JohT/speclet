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
#include "WindowParameters.h"
#include <map>
#include <memory>
#include <tuple>

/**
 * @brief Factory class for window functions
 */
class WindowFunctionFactory {
public:
    static auto getSingletonInstance() -> WindowFunctionFactory &;
    
    // Copy-constructors and move- and assignment-operator are deleted, because this class is a singleton.
    WindowFunctionFactory(WindowFunctionFactory const &) = delete;
    WindowFunctionFactory(WindowFunctionFactory &&) = delete;
    auto operator=(WindowFunctionFactory const &) -> WindowFunctionFactory & = delete;
    auto operator=(WindowFunctionFactory const &&) -> WindowFunctionFactory & = delete;

    auto getWindow(const WindowParameters::WindowFunction &newWindowFunction, unsigned long resolution) -> std::shared_ptr<WindowFunction>;
    void clearCache();

private:
    WindowFunctionFactory() = default;
    ~WindowFunctionFactory() = default;

    struct Key {
        WindowParameters::WindowFunction windowFunction;
        unsigned long resolution;
        auto operator < (const Key& other) const -> bool {
            return std::tie(windowFunction, resolution) < std::tie(other.windowFunction, other.resolution);
        }
    };

    using WindowFunctionMapType = std::map<Key, std::shared_ptr<WindowFunction>>;
    WindowFunctionMapType windowFunctionsCache;

    static auto createWindow(const WindowParameters::WindowFunction& newWindowFunction, unsigned long resolution) -> std::shared_ptr<WindowFunction>;
};