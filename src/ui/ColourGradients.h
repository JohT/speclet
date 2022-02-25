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
#define COLOURGRADIENT ColourGradients::getSingletonInstance()
#define GRADIENT_BLUE ColourGradients::getSingletonInstance()->getBlue()
#define GRADIENT_GREEN ColourGradients::getSingletonInstance()->getGreen()
#define GRADIENT_FIRE ColourGradients::getSingletonInstance()->getFire()
#define GRADIENT_RAINBOW ColourGradients::getSingletonInstance()->getRainbow()

class ColourGradients {
public:
    static ColourGradients *getSingletonInstance();
    void destruct();

    juce::ColourGradient get(int index);

    juce::ColourGradient getBlue();
    juce::ColourGradient getGreen();
    juce::ColourGradient getFire();
    juce::ColourGradient getRainbow();

private:
    static ColourGradients *singletonInstance;

    ColourGradients(void){};
    ~ColourGradients(void){};
    ColourGradients(const ColourGradients &);
};
