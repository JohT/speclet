/*
  ==============================================================================
  ==============================================================================
*/

#include "SpectronMainUI.h"
#include "SpectronDrawer.h"

//==============================================================================
SpectronMainUI::SpectronMainUI(SpectronAudioProcessor &ownerAudioProcessor)
    : AudioProcessorEditor(ownerAudioProcessor), audioProcessor(ownerAudioProcessor) {
    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize(ownerAudioProcessor.lastUIWidth, ownerAudioProcessor.lastUIHeight);

    //GUI, that holds all parameter-widgets and the spectralview
    addAndMakeVisible(&spectralAnalyzerComponent);

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize(ownerAudioProcessor.lastUIWidth, ownerAudioProcessor.lastUIHeight);
}

//==============================================================================
void SpectronMainUI::paint(juce::Graphics &) {
}

void SpectronMainUI::resized() {
    spectralAnalyzerComponent.setBounds(0, 0, MAINGUI_SIZE_X, MAINGUI_SIZE_Y);
    audioProcessor.lastUIWidth = getWidth();
    audioProcessor.lastUIHeight = getHeight();
}