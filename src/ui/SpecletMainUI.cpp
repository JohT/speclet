/*
  ==============================================================================
  ==============================================================================
*/

#include "SpecletMainUI.h"
#include "SpecletDrawer.h"

//==============================================================================
SpecletMainUI::SpecletMainUI(SpecletAudioProcessor &ownerAudioProcessor)
    : AudioProcessorEditor(ownerAudioProcessor), audioProcessor(ownerAudioProcessor) {
    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize(ownerAudioProcessor.lastUIWidth, ownerAudioProcessor.lastUIHeight);

    //GUI, that holds all parameter-widgets and the spectralview
    addAndMakeVisible(&spectralAnalyzerComponent);

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize(ownerAudioProcessor.lastUIWidth, ownerAudioProcessor.lastUIHeight);
}

//==============================================================================
void SpecletMainUI::paint(juce::Graphics &) {
}

void SpecletMainUI::resized() {
    spectralAnalyzerComponent.setBounds(0, 0, MAINGUI_SIZE_X, MAINGUI_SIZE_Y);
    audioProcessor.lastUIWidth = getWidth();
    audioProcessor.lastUIHeight = getHeight();
}