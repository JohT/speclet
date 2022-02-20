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
/*
  ==============================================================================

  ==============================================================================
*/

#ifndef __PLUGINPROCESSOR_H_526ED7A9__
#define __PLUGINPROCESSOR_H_526ED7A9__

//#include <assert.h>
#include "..\..\libs\juce\JuceLibraryCode\JuceHeader.h"
#include "..\config\JucePluginCharacteristics.h"
#include "..\dsp\SignalGenerator.h"
#include "..\dsp\transformations\TransformationFactory.h"
#include "SpectronParameters.h"

//==============================================================================
/**
*/
class SpectronAudioProcessor  :	public AudioProcessor,
											public juce::ValueTree::Listener
{
public:
	 //==============================================================================
	 SpectronAudioProcessor();
	 ~SpectronAudioProcessor();
	 //==============================================================================
	 void prepareToPlay (double sampleRate, int samplesPerBlock);
	 void releaseResources();
	 void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);
	 //==============================================================================
	 bool hasEditor() const                  { return true; }
	 AudioProcessorEditor* createEditor();
	 //==============================================================================
	 const String getName() const            { return JucePlugin_Name; }

	 int getNumParameters();
	 float getParameter (int index);
	 void setParameter (int index, float newValue);
	 const String getParameterName (int index);
	 const String getParameterText (int index);

	 const String getInputChannelName (int channelIndex) const	{return String (channelIndex + 1);}
	 const String getOutputChannelName (int channelIndex) const {return String (channelIndex + 1);}
	 bool isInputChannelStereoPair (int index) const				{return true;}
	 bool isOutputChannelStereoPair (int index) const				{return true;}

	 bool acceptsMidi() const												{return false;}
	 bool producesMidi() const												{return false;}
	 //==============================================================================
	 int getNumPrograms()                                                { return 0; }
	 int getCurrentProgram()                                             { return 0; }
	 void setCurrentProgram (int /*index*/)                              { }
	 const String getProgramName (int /*index*/)                         { return String::empty; }
	 void changeProgramName (int /*index*/, const String& /*newName*/)   { }
	 //==============================================================================
	 //these methods are called, when parameter changes were recognised
	 void valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &property);
	 void valueTreeChildrenChanged (ValueTree &treeWhoseChildHasChanged) {};
	 void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged) {};
	 //==============================================================================
	 void getStateInformation (MemoryBlock& destData);
	 void setStateInformation (const void* data, int sizeInBytes);
	 //==============================================================================
	 void updateTransformation();
	 void updateSignalGenerator();
	 //==============================================================================	 
	 // these are used to persist the UI's size - the values are stored along with the
	 // filter's other parameters, and the UI component will update them when it gets
	 // resized.
	 int lastUIWidth, lastUIHeight;
	//==============================================================================

private:
	 //A pointer to the parameters-singleton is kept here
	 //as local member, since it is easier to read it that way
	 SpectronParameters* parameters;

	 //Some parameter need to be kept local (as copy), 
	 //since they are called in critical sections 
	 //e.g. during Audioprocessing on every sample
	 int					parameter_routing;
	 Transformation*	currentTransformation;
	 SignalGenerator*	signalGenerator;
	 juce::CriticalSection criticalSection;
	 //==============================================================================
	 float getSampleFromRouting(float* inL, float* inR);
	 //==============================================================================


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectronAudioProcessor);
};

#endif  // __PLUGINPROCESSOR_H_526ED7A9__