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
#include "juce_audio_utils/juce_audio_utils.h"
namespace ColourGradients {
    /**
     * @brief A colour gradient for the spectrum analyzer for the given index.
     * 
     * @param index int starting with 1 for the first colour gradient (blue)
     * @return juce::ColourGradient 
     */
    auto forIndex(int index) -> juce::ColourGradient;

    auto fadeToBlack(const juce::Colour &colour) noexcept -> juce::ColourGradient;
    auto fire() noexcept -> juce::ColourGradient;
    auto rainbow() noexcept -> juce::ColourGradient;

    const juce::ColourGradient GREEN = fadeToBlack(juce::Colour::fromRGB(45, 255, 45));
    const juce::ColourGradient BLUE = fadeToBlack(juce::Colour::fromRGB(10, 80, 255));
    const juce::ColourGradient FIRE = fire();
    const juce::ColourGradient RAINBOW = rainbow();

}
