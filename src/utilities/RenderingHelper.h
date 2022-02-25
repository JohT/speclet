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
#include "../data/SpectralDataBuffer.h"
#include "../dsp/transformations/Transformation.h"
#include "JuceHeader.h"
#include <vector>

class RenderingHelper {
public:
	typedef std::vector<long> TVerticalColorIndizes;
	typedef struct {
		bool		logFrequency;	//false=lin, true=log
		bool		logMagnitude;	//false=lin, true=log
	} TAnalyzerSettings;

	RenderingHelper(void);
	~RenderingHelper(void);

	void setColourGradient(juce::ColourGradient value) {colourGradient = value;};

	void renderVerticalPoints(
		Transformation*									transformation,		
		TAnalyzerSettings									settings,
		long													currentXPos,
		juce::Image*										spectralImage
	);				

private:
	double getColorAmount(
		double	magnitude, 
		double	minMagnitude, 
		double	maxMagnitude, 
		bool		logMagnitude
	);
	double assureBorders(const juce::String &paramName, double value, double min, double max);
	int roundToInt(double d);

public:
//Normally, this method would be defined as "privat". 
//But since it is directly adressed in a unit-test, it remains 
//(until the test gets deprecated) "public"
    long pixelToIndex(
            int pixel,
            int height,
            SpectralDataInfo *spectralDataInfo,
            bool logFrequency);

private:
	std::vector<juce::Colour>	mColorTable;	//color table
	juce::ColourGradient			colourGradient;	//color gradient object
};