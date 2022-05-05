/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  4 May 2011 11:36:36pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/
#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../dsp/transformations/Transformation.h"
#include "../utilities/RenderingHelper.h"
#include "ColourGradients.h"
#include "juce_core/juce_core.h"

//[/Headers]


//==============================================================================
/**
																						  //[Comments]
	 An auto-generated component, created by the Jucer.

	 Describe your class and how it works here!
																						  //[/Comments]
*/
class SpecletDrawer : public juce::Component,
                      public TransformationListener,
                      public juce::Timer,
                      public juce::AudioProcessorValueTreeState::Listener {
public:
    //==============================================================================
    SpecletDrawer(bool logFrequency = true, bool logMagnitude = true, const juce::ColourGradient & initialColourGradient = ColourGradients::BLUE);
    ~SpecletDrawer() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback() override;
    void onTransformationEvent(TransformationResult *result) override;
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    //==============================================================================
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;

    //==============================================================================
    juce_UseDebuggingNewOperator

private :
    //[UserVariables]   -- You can add your own custom variables in this section.
    static const juce::Colour AXIS_COLOR;
    enum Constants {
        SIZE_X = 528,
        SIZE_Y = 360,
        TIMER = 20,
        WAIT_FOR_DESTRUCTION_TIMEOUT = 3000
    };
    void updateFrequencyAxisImage();
    void updateTimeAxisImage(double timeresolution);
    void appendSpectralImage(TransformationResult *result);
    juce::Viewport *getParentViewPort() const { return static_cast<juce::Viewport *>(getParentComponent()); }

    int sizeX = Constants::SIZE_X;
    int sizeY = Constants::SIZE_Y;
    double currentTimeResolution = 0;
    double currentSamplingFrequency = 0.0;
    RenderingHelper::TAnalyzerSettings settings;
    RenderingHelper renderingHelper;
    int currentCursorXPos = 0;
    juce::Image spectrumImage = {juce::Image::RGB, sizeX, sizeY, true};
    juce::Image axisImage = {juce::Image::PixelFormat::ARGB, sizeX, sizeY, true};
    juce::CriticalSection criticalSection = {};
    juce::WaitableEvent waitForDestruction{true};
    bool ready = false;

    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    SpecletDrawer(const SpecletDrawer &);
    const SpecletDrawer &operator=(const SpecletDrawer &);
};