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
#include "juce_audio_utils/juce_audio_utils.h"
#include <vector>

class RenderingHelper {
public:
    using TVerticalColorIndices = std::vector<long>;
    using TAnalyzerSettings = struct {
        bool logFrequency;//false=lin, true=log
        bool logMagnitude;//false=lin, true=log
    };

    RenderingHelper();
    ~RenderingHelper() = default;

    void setColourGradient(juce::ColourGradient value) { colourGradient = value; }

    void renderVerticalPoints(
            TransformationResult *transformationResult,
            TAnalyzerSettings settings,
            int currentXPos,
            juce::Image *spectralImage);

    //Normally, this method would be defined as "privat".
    //But since it is directly adressed in a unit-test, it remains
    //(until the test gets deprecated) "public"
    auto pixelToIndex(
            int pixel,
            int height,
            const SpectralDataInfo &spectralDataInfo,
            bool logFrequency) -> unsigned long;

private:
    auto getColorAmount(
            double magnitude,
            double minMagnitude,
            double maxMagnitude,
            bool logMagnitude) -> double;
    auto assureBorders(const juce::String &paramName, double value, double min, double max) -> double;

    std::vector<juce::Colour> mColorTable;//color table
    juce::ColourGradient colourGradient;  //color gradient object
};