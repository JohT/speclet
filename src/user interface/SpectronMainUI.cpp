/*
  ==============================================================================
  ==============================================================================
*/

#include "SpectronMainUI.h"
#include "SpectronDrawer.h"

//==============================================================================
SpectronMainUI::SpectronMainUI (SpectronAudioProcessor* ownerFilter)
	 : AudioProcessorEditor (ownerFilter)
{
	 // set our component's initial size to be the last one that was stored in the filter's settings
	 setSize (ownerFilter->lastUIWidth, ownerFilter->lastUIHeight);

	 //GUI, that holds all parameter-widgets and the spectralview
	 addAndMakeVisible (&spectralAnalyzerComponent);

	 // set our component's initial size to be the last one that was stored in the filter's settings
	 setSize (ownerFilter->lastUIWidth, ownerFilter->lastUIHeight);
}

SpectronMainUI::~SpectronMainUI()
{
}

//==============================================================================
void SpectronMainUI::paint (Graphics& g)
{
}

void SpectronMainUI::resized()
{
	spectralAnalyzerComponent.setBounds(0, 0, MAINGUI_SIZE_X, MAINGUI_SIZE_Y);
	getProcessor()->lastUIWidth = getWidth();
	getProcessor()->lastUIHeight = getHeight();
}