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
namespace ColourGradients {
    auto forIndex(int index) -> juce::ColourGradient;

    auto fadeToBlack(const Colour &colour) noexcept -> juce::ColourGradient;
    auto fire() noexcept -> juce::ColourGradient;
    auto rainbow() noexcept -> juce::ColourGradient;

    const juce::ColourGradient GREEN = fadeToBlack(Colour::fromRGB(45, 255, 45));
    const juce::ColourGradient BLUE = fadeToBlack(Colour::fromRGB(45, 45, 255));
    const juce::ColourGradient FIRE = fire();
    const juce::ColourGradient RAINBOW = rainbow();

}
