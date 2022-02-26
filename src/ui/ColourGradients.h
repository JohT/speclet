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
#include "JuceHeader.h"
class ColourGradients {
public:
    static auto getSingletonInstance() -> ColourGradients &;
    ~ColourGradients() = default;

    static auto forIndex(int index) -> const juce::ColourGradient;

    static auto blue() -> const juce::ColourGradient;
    static auto green() -> const juce::ColourGradient;
    static auto fire() -> const juce::ColourGradient;
    static auto rainbow() -> const juce::ColourGradient;

private:
    ColourGradients() = default;
    ColourGradients(const ColourGradients &);
};
