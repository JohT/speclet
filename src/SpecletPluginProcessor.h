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

#include "dsp/SignalGenerator.h"
#include "dsp/transformations/TransformationFactory.h"
#include "plugin/SpecletParameters.h"
#include <array>
#include <juce_core/juce_core.h>
#include <type_traits>


enum Channel {
    Left = 0,
    Right = 1
};

//==============================================================================
class SpecletAudioProcessor : public juce::AudioProcessor, public juce::ValueTree::Listener {
public:
    //==============================================================================
    SpecletAudioProcessor();
    ~SpecletAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    auto isBusesLayoutSupported(const BusesLayout &layouts) const -> bool override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    auto createEditor() -> juce::AudioProcessorEditor * override;
    auto hasEditor() const -> bool override;

    //==============================================================================
    //these methods are called, when parameter changes were recognised
    auto getNumParameters() -> int override;
    auto getParameter(int index) -> float override;
    void setParameter(int index, float newValue) override;
    auto getParameterName(int index) -> const juce::String override;
    auto getParameterText(int index) -> const juce::String override;

    //==============================================================================
    auto getName() const -> const juce::String override;

    auto acceptsMidi() const -> bool override;
    auto producesMidi() const -> bool override;
    auto isMidiEffect() const -> bool override;
    auto getTailLengthSeconds() const -> double override;

    //==============================================================================
    auto getNumPrograms() -> int override;
    auto getCurrentProgram() -> int override;
    void setCurrentProgram(int index) override;
    auto getProgramName(int index) -> const juce::String override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    //these methods are called, when parameter changes were recognised
    void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override;
    void valueTreeChildrenChanged(juce::ValueTree &) {}
    void valueTreeParentChanged(juce::ValueTree &) override {}

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;
    //==============================================================================
    void updateTransformation();
    void updateSignalGenerator();
    //==============================================================================
    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth = 800; //NOLINT(readability-magic-numbers)
    int lastUIHeight = 360;//NOLINT(readability-magic-numbers)
    //==============================================================================

private:
    SpecletParameters &parameters = SpecletParameters::getSingletonInstance();

    //Some parameter need to be kept local (as copy),
    //since they are called in critical sections
    //e.g. during Audioprocessing on every sample
    int parameterRouting;
    Transformation *currentTransformation = nullptr;
    SignalGenerator signalGenerator;
    juce::CriticalSection criticalSection;
    //==============================================================================
    auto getSampleFromRouting(const float *inL, const float *inR) -> float;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpecletAudioProcessor)
};
