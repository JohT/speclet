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

#ifndef __JUCER_HEADER_SPECTRONDRAWER_SPECTRONDRAWER_742E0E1E__
#define __JUCER_HEADER_SPECTRONDRAWER_SPECTRONDRAWER_742E0E1E__

//[Headers]     -- You can add your own extra header files here --
#include "..\..\libs\juce\JuceLibraryCode\JuceHeader.h"
#include "..\dsp\transformations\Transformation.h"
#include "..\utilities\RenderingHelper.h"
//[/Headers]



//==============================================================================
/**
																						  //[Comments]
	 An auto-generated component, created by the Jucer.

	 Describe your class and how it works here!
																						  //[/Comments]
*/
class SpectronDrawer  : public Component,
								public TransformationListener,
								public Timer,
								public juce::ValueTree::Listener
{
public:
	 //==============================================================================
	 SpectronDrawer ();
	 ~SpectronDrawer();

	 //==============================================================================
	 //[UserMethods]     -- You can add your own custom methods in this section.
	 static enum  Constants {
		 SIZE_X = 528,
		 TIMER = 20
	 };

	 void timerCallback();
	 void appendSpectralImage(Transformation* value);
	 void onTransformationEvent(Transformation* value);
	 void valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &property);
	 void valueTreeChildrenChanged (ValueTree &treeWhoseChildHasChanged) {};
	 void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged) {};
	 //==============================================================================
	 //[/UserMethods]

	 void paint (Graphics& g);
	 void resized();


	 //==============================================================================
	 juce_UseDebuggingNewOperator

private:
	 //[UserVariables]   -- You can add your own custom variables in this section.
		static const juce::Colour AXIS_COLOR;

		void updateFrequencyAxisImage();
		void updateTimeAxisImage();
		juce::Viewport* getParentViewPort() const {return static_cast <juce::Viewport*> (getParentComponent());};

		int sizeX, sizeY;
		double currentTimeResolution;
		RenderingHelper::TAnalyzerSettings	settings;
		RenderingHelper			renderingHelper;
		long							currentCursorXPos;
		juce::Image					spectrumImage;
		juce::Image					axisImage;
		juce::CriticalSection	criticalSection;

	 //[/UserVariables]

	 //==============================================================================


	 //==============================================================================
	 // (prevent copy constructor and operator= being generated..)
	 SpectronDrawer (const SpectronDrawer&);
	 const SpectronDrawer& operator= (const SpectronDrawer&);
};


#endif   // __JUCER_HEADER_SPECTRONDRAWER_SPECTRONDRAWER_742E0E1E__
