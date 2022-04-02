/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  23 May 2011 11:58:55pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "../dsp/SignalGeneratorParameters.h"
#include "../dsp/transformations/TransformationParameters.h"
#include "../dsp/transformations/WaveletParameters.h"
#include "../dsp/windowing/WindowParameters.h"
#include "../ui/ColorGradientsParameters.h"
#include "../ui/SpecletDrawerParameters.h"
#include "SpecletDrawer.h"


//[/Headers]

#include "SpecletAnalyzerComponent.h"
#include <cstddef>
#include <type_traits>


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SpecletAnalyzerComponent::SpecletAnalyzerComponent()
    : Component("SpecletAnalyzerComponent"),
      comboBoxResolution(nullptr),
      spectralviewport(nullptr),
      labelResolution(nullptr),
      comboBoxTransformation(nullptr),
      labelTransformation(nullptr),
      comboBoxWindowing(nullptr),
      labelWindowing(nullptr),
      comboBoxWavelet(nullptr),
      labelWavelet(nullptr),
      comboBoxWaveletPacketBasis(nullptr),
      labelWaveletPacketBasis(nullptr),
      comboBoxSignalGenerator(nullptr),
      labelSignalGenerator(nullptr),
      comboBoxRouting(nullptr),
      labelRouting(nullptr),
      labelSignalGeneratorFrequency(nullptr),
      sliderSignalGeneratorFrequency(nullptr),
      labelLogF(nullptr),
      labelLogA(nullptr),
      comboBoxLogF(nullptr),
      comboBoxLogA(nullptr),
      labelColorMode(nullptr),
      comboBoxColorMode(nullptr) {
    addAndMakeVisible(comboBoxResolution = new juce::ComboBox("comboBoxResolution"));
    comboBoxResolution->setTooltip("Resolution");
    comboBoxResolution->setEditableText(false);
    comboBoxResolution->setJustificationType(juce::Justification::centredLeft);
    comboBoxResolution->setTextWhenNothingSelected(juce::String());
    comboBoxResolution->setTextWhenNoChoicesAvailable(juce::String());
    comboBoxResolution->addListener(this);

    addAndMakeVisible(spectralviewport = new juce::Viewport("spectralviewport"));
    spectralviewport->setScrollBarsShown(false, true);
    spectralviewport->setScrollBarThickness(10);

    addAndMakeVisible(labelResolution = new juce::Label("labelResolution",
                                                        "Resolution"));
    labelResolution->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelResolution->setJustificationType(juce::Justification::centredLeft);
    labelResolution->setEditable(false, false, false);
    labelResolution->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelResolution->setColour(juce::Label::textColourId, juce::Colours::white);
    labelResolution->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelResolution->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelResolution->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(comboBoxTransformation = new juce::ComboBox("comboBoxTransformation"));
    comboBoxTransformation->setTooltip("Transformation");
    comboBoxTransformation->setEditableText(false);
    comboBoxTransformation->setJustificationType(juce::Justification::centredLeft);
    comboBoxTransformation->setTextWhenNothingSelected(juce::String());
    comboBoxTransformation->setTextWhenNoChoicesAvailable(juce::String());
    comboBoxTransformation->addListener(this);

    addAndMakeVisible(labelTransformation = new juce::Label("labelTransformation",
                                                            "Transformation"));
    labelTransformation->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelTransformation->setJustificationType(juce::Justification::centredLeft);
    labelTransformation->setEditable(false, false, false);
    labelTransformation->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelTransformation->setColour(juce::Label::textColourId, juce::Colours::white);
    labelTransformation->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelTransformation->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelTransformation->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(comboBoxWindowing = new juce::ComboBox("comboBoxWindowing"));
    comboBoxWindowing->setTooltip("Window Function");
    comboBoxWindowing->setEditableText(false);
    comboBoxWindowing->setJustificationType(juce::Justification::centredLeft);
    comboBoxWindowing->setTextWhenNothingSelected(juce::String());
    comboBoxWindowing->setTextWhenNoChoicesAvailable(juce::String());
    comboBoxWindowing->addListener(this);

    addAndMakeVisible(labelWindowing = new juce::Label("labelWindowing",
                                                       "Window Function"));
    labelWindowing->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelWindowing->setJustificationType(juce::Justification::centredLeft);
    labelWindowing->setEditable(false, false, false);
    labelWindowing->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelWindowing->setColour(juce::Label::textColourId, juce::Colours::white);
    labelWindowing->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelWindowing->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelWindowing->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(comboBoxWavelet = new juce::ComboBox("comboBoxWavelet"));
    comboBoxWavelet->setTooltip("Wavelet");
    comboBoxWavelet->setEditableText(false);
    comboBoxWavelet->setJustificationType(juce::Justification::centredLeft);
    comboBoxWavelet->setTextWhenNothingSelected(juce::String());
    comboBoxWavelet->setTextWhenNoChoicesAvailable(juce::String());
    comboBoxWavelet->addListener(this);

    addAndMakeVisible(labelWavelet = new juce::Label("labelWavelet",
                                                     "Wavelet"));
    labelWavelet->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelWavelet->setJustificationType(juce::Justification::centredLeft);
    labelWavelet->setEditable(false, false, false);
    labelWavelet->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelWavelet->setColour(juce::Label::textColourId, juce::Colours::white);
    labelWavelet->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelWavelet->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelWavelet->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(comboBoxWaveletPacketBasis = new juce::ComboBox("comboBoxWaveletPacketBasis"));
    comboBoxWaveletPacketBasis->setTooltip("Wavelet Packet Basis");
    comboBoxWaveletPacketBasis->setEditableText(false);
    comboBoxWaveletPacketBasis->setJustificationType(juce::Justification::centredLeft);
    comboBoxWaveletPacketBasis->setTextWhenNothingSelected(juce::String());
    comboBoxWaveletPacketBasis->setTextWhenNoChoicesAvailable(juce::String());
    comboBoxWaveletPacketBasis->addListener(this);

    addAndMakeVisible(labelWaveletPacketBasis = new juce::Label("labelWaveletPacketBasis",
                                                               "Wavelet Packet Basis"));
    labelWaveletPacketBasis->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelWaveletPacketBasis->setJustificationType(juce::Justification::centredLeft);
    labelWaveletPacketBasis->setEditable(false, false, false);
    labelWaveletPacketBasis->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelWaveletPacketBasis->setColour(juce::Label::textColourId, juce::Colours::white);
    labelWaveletPacketBasis->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelWaveletPacketBasis->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelWaveletPacketBasis->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(comboBoxSignalGenerator = new juce::ComboBox("comboBoxSignalgenerator"));
    comboBoxSignalGenerator->setTooltip("Osciallator");
    comboBoxSignalGenerator->setEditableText(false);
    comboBoxSignalGenerator->setJustificationType(juce::Justification::centredLeft);
    comboBoxSignalGenerator->setTextWhenNothingSelected(juce::String());
    comboBoxSignalGenerator->setTextWhenNoChoicesAvailable(juce::String());
    comboBoxSignalGenerator->addListener(this);

    addAndMakeVisible(labelSignalGenerator = new juce::Label("labelSignalgenerator",
                                                             "Oscillator"));
    labelSignalGenerator->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelSignalGenerator->setJustificationType(juce::Justification::centredLeft);
    labelSignalGenerator->setEditable(false, false, false);
    labelSignalGenerator->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelSignalGenerator->setColour(juce::Label::textColourId, juce::Colours::white);
    labelSignalGenerator->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelSignalGenerator->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelSignalGenerator->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(comboBoxRouting = new juce::ComboBox("comboBoxRouting"));
    comboBoxRouting->setTooltip("Audio Source");
    comboBoxRouting->setEditableText(false);
    comboBoxRouting->setJustificationType(juce::Justification::centredLeft);
    comboBoxRouting->setTextWhenNothingSelected(juce::String());
    comboBoxRouting->setTextWhenNoChoicesAvailable(juce::String());
    comboBoxRouting->addListener(this);

    addAndMakeVisible(labelRouting = new juce::Label("labelRouting", "Audio Source"));
    labelRouting->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelRouting->setJustificationType(juce::Justification::centredLeft);
    labelRouting->setEditable(false, false, false);
    labelRouting->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelRouting->setColour(juce::Label::textColourId, juce::Colours::white);
    labelRouting->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelRouting->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelRouting->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(labelSignalGeneratorFrequency = new juce::Label("labelGeneratorfrequenz",
                                                               "Oscillator Frequency"));
    labelSignalGeneratorFrequency->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelSignalGeneratorFrequency->setJustificationType(juce::Justification::centredLeft);
    labelSignalGeneratorFrequency->setEditable(false, false, false);
    labelSignalGeneratorFrequency->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelSignalGeneratorFrequency->setColour(juce::Label::textColourId, juce::Colours::white);
    labelSignalGeneratorFrequency->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelSignalGeneratorFrequency->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelSignalGeneratorFrequency->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(sliderSignalGeneratorFrequency = new juce::Slider("sliderGeneratorFrequenz"));
    sliderSignalGeneratorFrequency->setTooltip("Oscillator Frequency");
    sliderSignalGeneratorFrequency->setExplicitFocusOrder(7);
    sliderSignalGeneratorFrequency->setRange(10, 22000, 1);
    sliderSignalGeneratorFrequency->setSliderStyle(juce::Slider::LinearHorizontal);
    sliderSignalGeneratorFrequency->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 25);
    sliderSignalGeneratorFrequency->addListener(this);

    addAndMakeVisible(labelLogF = new juce::Label("labelLogF",
                                                  "Frequency Scale"));
    labelLogF->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelLogF->setJustificationType(juce::Justification::centredLeft);
    labelLogF->setEditable(false, false, false);
    labelLogF->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelLogF->setColour(juce::Label::textColourId, juce::Colours::white);
    labelLogF->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelLogF->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelLogF->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(labelLogA = new juce::Label("labelLogA",
                                                  "Magnitude Scale"));
    labelLogA->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelLogA->setJustificationType(juce::Justification::centredLeft);
    labelLogA->setEditable(false, false, false);
    labelLogA->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelLogA->setColour(juce::Label::textColourId, juce::Colours::white);
    labelLogA->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelLogA->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelLogA->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(comboBoxLogF = new juce::ComboBox("comboBoxLogF"));
    comboBoxLogF->setTooltip("Frequency Scale");
    comboBoxLogF->setEditableText(false);
    comboBoxLogF->setJustificationType(juce::Justification::centredLeft);
    comboBoxLogF->setTextWhenNothingSelected(juce::String());
    comboBoxLogF->setTextWhenNoChoicesAvailable(juce::String());
    comboBoxLogF->addListener(this);

    addAndMakeVisible(comboBoxLogA = new juce::ComboBox("comboBoxLogA"));
    comboBoxLogA->setTooltip("Magnitude Scale");
    comboBoxLogA->setEditableText(false);
    comboBoxLogA->setJustificationType(juce::Justification::centredLeft);
    comboBoxLogA->setTextWhenNothingSelected(juce::String());
    comboBoxLogA->setTextWhenNoChoicesAvailable(juce::String());
    comboBoxLogA->addListener(this);

    addAndMakeVisible(labelColorMode = new juce::Label("labelColorMode",
                                                       "Color Mode"));
    labelColorMode->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelColorMode->setJustificationType(juce::Justification::centredLeft);
    labelColorMode->setEditable(false, false, false);
    labelColorMode->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelColorMode->setColour(juce::Label::textColourId, juce::Colours::white);
    labelColorMode->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelColorMode->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelColorMode->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(comboBoxColorMode = new juce::ComboBox("comboBoxColorMode"));
    comboBoxColorMode->setTooltip("Color Mode");
    comboBoxColorMode->setEditableText(false);
    comboBoxColorMode->setJustificationType(juce::Justification::centredLeft);
    comboBoxColorMode->setTextWhenNothingSelected(juce::String());
    comboBoxColorMode->setTextWhenNoChoicesAvailable(juce::String());
    comboBoxColorMode->addListener(this);

    
    //[UserPreSize]
    fillComboBoxes();
    //[/UserPreSize]

    setSize(800, 360);

    //[Constructor] You can add your own custom stuff here..

    //gets the pointer to the parameters singelton - for a better readability
    parameters = &SpecletParameters::getSingletonInstance();
    //registers itself as listener for parameter-changes
    SpecletParameters::getSingletonInstance().addListener(this);
    DBG("SpecletAnalyzerComponent as parameter listener added");
    //adds spectrum drawing component to the scrollable view
    spectralviewport->setViewedComponent(new SpecletDrawer());

    //adds entries to the popup/context menu
    popupMenu.addItem(POPUPMENU_INDEX_1_ABOUT, "about");
    addMouseListener(this, true);
    //[/Constructor]
}

SpecletAnalyzerComponent::~SpecletAnalyzerComponent() {
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero(comboBoxResolution);
    deleteAndZero(spectralviewport);
    deleteAndZero(labelResolution);
    deleteAndZero(comboBoxTransformation);
    deleteAndZero(labelTransformation);
    deleteAndZero(comboBoxWindowing);
    deleteAndZero(labelWindowing);
    deleteAndZero(comboBoxWavelet);
    deleteAndZero(labelWavelet);
    deleteAndZero(comboBoxWaveletPacketBasis);
    deleteAndZero(labelWaveletPacketBasis);
    deleteAndZero(comboBoxSignalGenerator);
    deleteAndZero(labelSignalGenerator);
    deleteAndZero(comboBoxRouting);
    deleteAndZero(labelRouting);
    deleteAndZero(labelSignalGeneratorFrequency);
    deleteAndZero(sliderSignalGeneratorFrequency);
    deleteAndZero(labelLogF);
    deleteAndZero(labelLogA);
    deleteAndZero(comboBoxLogF);
    deleteAndZero(comboBoxLogA);
    deleteAndZero(labelColorMode);
    deleteAndZero(comboBoxColorMode);

    //[Destructor]. You can add your own custom destruction code here..
    //unregisteres itself as listener for parameter-changes
    SpecletParameters::getSingletonInstance().removeListener(this);
    DBG("SpecletAnalyzerComponent as parameter listener removed");
    parameters = nullptr;
    //[/Destructor]
}

//==============================================================================
void SpecletAnalyzerComponent::paint(juce::Graphics &g) {
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(juce::Colours::black);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SpecletAnalyzerComponent::resized() {
    comboBoxResolution->setBounds((128) + 0, (8) + 32, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    spectralviewport->setBounds(256, 8, 528, 344);
    labelResolution->setBounds((8) + 0, (8) + 32, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    comboBoxTransformation->setBounds(128, 8, 120, 24);
    labelTransformation->setBounds(8, 8, 120, 24);
    comboBoxWindowing->setBounds((128) + 0, (8) + 64, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    labelWindowing->setBounds((8) + 0, (8) + 64, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    comboBoxWavelet->setBounds((128) + 0, (8) + 96, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    labelWavelet->setBounds((8) + 0, (8) + 96, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    comboBoxWaveletPacketBasis->setBounds((128) + 0, (8) + 128, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    labelWaveletPacketBasis->setBounds((8) + 0, (8) + 128, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    comboBoxSignalGenerator->setBounds((128) + 0, (8) + 288, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    labelSignalGenerator->setBounds((8) + 0, (8) + 288, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    comboBoxRouting->setBounds((128) + 0, (8) + 160, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    labelRouting->setBounds((8) + 0, (8) + 160, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    labelSignalGeneratorFrequency->setBounds((8) + 0, (8) + 320, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    sliderSignalGeneratorFrequency->setBounds((128) + 0, (8) + 320, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    labelLogF->setBounds((8) + 0, (8) + 192, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    labelLogA->setBounds((8) + 0, (8) + 224, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    comboBoxLogF->setBounds((128) + 0, (8) + 192, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    comboBoxLogA->setBounds((128) + 0, (8) + 224, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    labelColorMode->setBounds((8) + 1, (8) + 256, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    comboBoxColorMode->setBounds((128) + 1, (8) + 256, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SpecletAnalyzerComponent::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) {
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxResolution) {
        //[UserComboBoxCode_comboBoxResolution] -- add your combo box handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_Resolution, comboBoxResolution->getText().getIntValue());
        //[/UserComboBoxCode_comboBoxResolution]
    } else if (comboBoxThatHasChanged == comboBoxTransformation) {
        //[UserComboBoxCode_comboBoxTransformation] -- add your combo box handling code here..
        float selectedOption = static_cast<float>(comboBoxTransformation->getSelectedId());
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_Transformation, selectedOption);
        transformationChanged(selectedOption);
        //[/UserComboBoxCode_comboBoxTransformation]
    } else if (comboBoxThatHasChanged == comboBoxWindowing) {
        //[UserComboBoxCode_comboBoxWindowing] -- add your combo box handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_Windowing, comboBoxWindowing->getSelectedId());
        //[/UserComboBoxCode_comboBoxWindowing]
    } else if (comboBoxThatHasChanged == comboBoxWavelet) {
        //[UserComboBoxCode_comboBoxWavelet] -- add your combo box handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_Wavelet, comboBoxWavelet->getSelectedId());
        //[/UserComboBoxCode_comboBoxWavelet]
    } else if (comboBoxThatHasChanged == comboBoxWaveletPacketBasis) {
        //[UserComboBoxCode_comboBoxWaveletPacketBasis] -- add your combo box handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_WaveletPacketBasis, comboBoxWaveletPacketBasis->getSelectedId());
        //[/UserComboBoxCode_comboBoxWaveletPacketBasis]
    } else if (comboBoxThatHasChanged == comboBoxSignalGenerator) {
        //[UserComboBoxCode_comboBoxSignalgenerator] -- add your combo box handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_Generator, comboBoxSignalGenerator->getSelectedId());
        //[/UserComboBoxCode_comboBoxSignalgenerator]
    } else if (comboBoxThatHasChanged == comboBoxRouting) {
        //[UserComboBoxCode_comboBoxRouting] -- add your combo box handling code here..
        float selectedOption = static_cast<float>(comboBoxRouting->getSelectedId());
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_Routing, selectedOption);
        routingChanged(selectedOption);
        //[/UserComboBoxCode_comboBoxRouting]
    } else if (comboBoxThatHasChanged == comboBoxLogF) {
        //[UserComboBoxCode_comboBoxLogF] -- add your combo box handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_LogFrequency, comboBoxLogF->getSelectedId());
        //[/UserComboBoxCode_comboBoxLogF]
    } else if (comboBoxThatHasChanged == comboBoxLogA) {
        //[UserComboBoxCode_comboBoxLogA] -- add your combo box handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_LogMagnitude, comboBoxLogA->getSelectedId());
        //[/UserComboBoxCode_comboBoxLogA]
    } else if (comboBoxThatHasChanged == comboBoxColorMode) {
        //[UserComboBoxCode_comboBoxColorMode] -- add your combo box handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_ColorMode, comboBoxColorMode->getSelectedId());
        //[/UserComboBoxCode_comboBoxColorMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void SpecletAnalyzerComponent::sliderValueChanged(juce::Slider *sliderThatWasMoved) {
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderSignalGeneratorFrequency) {
        //[UserSliderCode_sliderGeneratorFrequenz] -- add your slider handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_GeneratorFrequency, sliderSignalGeneratorFrequency->getValue());
        //[/UserSliderCode_sliderGeneratorFrequenz]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SpecletAnalyzerComponent::visibilityChanged() {
    //[UserCode_visibilityChanged] -- Add your code here...
    //[/UserCode_visibilityChanged]
}

void SpecletAnalyzerComponent::parentSizeChanged() {
    //[UserCode_parentSizeChanged] -- Add your code here...
    //[/UserCode_parentSizeChanged]
}

void SpecletAnalyzerComponent::broughtToFront() {
    //[UserCode_broughtToFront] -- Add your code here...
    //[/UserCode_broughtToFront]
}

void SpecletAnalyzerComponent::childrenChanged() {
    //[UserCode_childrenChanged] -- Add your code here...
    //[/UserCode_childrenChanged]
}

void SpecletAnalyzerComponent::enablementChanged() {
    //[UserCode_enablementChanged] -- Add your code here...
    //[/UserCode_enablementChanged]
}

void SpecletAnalyzerComponent::mouseMove(const juce::MouseEvent &e) {
    //[UserCode_mouseMove] -- Add your code here...
    //[/UserCode_mouseMove]
}

void SpecletAnalyzerComponent::mouseDown(const juce::MouseEvent &e) {
    //[UserCode_mouseDown] -- Add your code here...
    if (e.mods.isRightButtonDown()) {
        auto options = juce::PopupMenu::Options().withTargetComponent(this).withMousePosition();
        popupMenu.showMenuAsync(options, [](auto index) {
            if (index == POPUPMENU_INDEX_1_ABOUT) {
                juce::AlertWindow::AlertIconType icon = juce::AlertWindow::InfoIcon;

                juce::StringRef message =
                        "Written by Johannes Troppacher (c)2011\n"
                        "Modernized 2022\n"
                        "\n"
                        "Audio Spectrum Analyzer Plugin using\n"
                        "Fourier- and Wavelet-Transformation\n"
                        "\n"
                        "Made with:\n"
                        "- Framework 'JUCE' originally (2011) by Raw Material Software\n"
                        "- VST-Interface 'VST SDK 2.4 rev2' by Steinberg(2011)\n"
                        "- VST 3 Audio Plug-Ins SDK by Steinberg (2022)\n"
                        "- FFT-Library 'FFTW' by MIT (Matteo Frigo and Steven G. Johnson)\n"
                        "- Wavelet-Library 'wave++'\n"
                        "   by Ryerson Computrational Signal Analysis Group\n"
                        "   (S. E. Ferrando, L. A. Kolasa and N. Kovacevic)";

                juce::AlertWindow::showMessageBoxAsync(icon, "About Speclet", message, "OK");
            }
        });
    }
    //[/UserCode_mouseDown]
}

void SpecletAnalyzerComponent::mouseWheelMove(const juce::MouseEvent & /* event */, const juce::MouseWheelDetails & /* wheel */) {
    //[UserCode_mouseWheelMove] -- Add your code here...
    //[/UserCode_mouseWheelMove]
}


//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void SpecletAnalyzerComponent::fillComboBoxes() {
    comboBoxResolution->addItem("256", SpecletParameters::RESOLUTION_256);
    comboBoxResolution->addItem("512", SpecletParameters::RESOLUTION_512);
    comboBoxResolution->addItem("1024", SpecletParameters::RESOLUTION_1024);
    comboBoxResolution->addItem("2048", SpecletParameters::RESOLUTION_2048);
    comboBoxResolution->addItem("4096", SpecletParameters::RESOLUTION_4096);
    comboBoxResolution->addItem("8192", SpecletParameters::RESOLUTION_8192);
    comboBoxResolution->addItem("16384", SpecletParameters::RESOLUTION_16384);
    comboBoxResolution->addItem("32768", SpecletParameters::RESOLUTION_32768);
    comboBoxResolution->addItem("65536", SpecletParameters::RESOLUTION_65536);

    using TransformationTypeValue = std::underlying_type<TransformationParameters::Type>::type;
    for (auto entry : TransformationParameters::typeNames) {
        comboBoxTransformation->addItem(std::string(entry.second), static_cast<TransformationTypeValue>(entry.first));
    }

    using WindowFunctionValue = std::underlying_type<WindowParameters::WindowFunction>::type;
    for (auto entry : WindowParameters::windowFunctionNames) {
        comboBoxWindowing->addItem(std::string(entry.second), static_cast<WindowFunctionValue>(entry.first));
    }

    using WaveletBaseValue = std::underlying_type<WaveletParameters::WaveletBase>::type;
    for (auto entry : WaveletParameters::waveletBaseNames) {
        comboBoxWavelet->addItem(std::string(entry.second), static_cast<WaveletBaseValue>(entry.first));
    }

    using ResolutionRatioValue = std::underlying_type<WaveletParameters::ResolutionRatioOption>::type;
    for (auto entry : WaveletParameters::resolutionRatioOptionNames) {
        comboBoxWaveletPacketBasis->addItem(std::string(entry.second), static_cast<ResolutionRatioValue>(entry.first));
    }

    using SignalGeneratorValue = std::underlying_type<SignalGeneratorParameters::Waveform>::type;
    for (auto entry : SignalGeneratorParameters::typeNames) {
        comboBoxSignalGenerator->addItem(std::string(entry.second), static_cast<SignalGeneratorValue>(entry.first));
    }

    comboBoxRouting->addItem("Mid", SpecletParameters::ROUTING_MID);
    comboBoxRouting->addItem("Side", SpecletParameters::ROUTING_SIDE);
    comboBoxRouting->addItem("Left", SpecletParameters::ROUTING_L);
    comboBoxRouting->addItem("Right", SpecletParameters::ROUTING_R);
    comboBoxRouting->addItem("Oscillator", SpecletParameters::ROUTING_GENERATOR);

    using AxisValue = std::underlying_type<SpecletDrawerParameters::Axis>::type;
    for (auto entry : SpecletDrawerParameters::axisNames) {
        comboBoxLogF->addItem(std::string(entry.second), static_cast<AxisValue>(entry.first));
        comboBoxLogA->addItem(std::string(entry.second), static_cast<AxisValue>(entry.first));
    }

    using ColorGradientValue = std::underlying_type<ColorGradientsParameters::ColorMode>::type;
    for (auto entry : ColorGradientsParameters::colorModeNames) {
        comboBoxColorMode->addItem(std::string(entry.second), static_cast<ColorGradientValue>(entry.first));
    }
}

void SpecletAnalyzerComponent::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier & /*property*/) {
    const juce::ScopedLock myScopedLock(criticalSection);

    updateComboBox(SpecletParameters::PARAMETER_COLORMODE, comboBoxColorMode, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_GENERATOR, comboBoxSignalGenerator, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_LOGFREQUENCY, comboBoxLogF, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_LOGMAGNITUDE, comboBoxLogA, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_RESOLUTION, comboBoxResolution, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_ROUTING, comboBoxRouting, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_TRANSFORMATION, comboBoxTransformation, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_WAVELET, comboBoxWavelet, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_WAVELETPACKETBASIS, comboBoxWaveletPacketBasis, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_WINDOWING, comboBoxWindowing, treeWhosePropertyHasChanged);
    updateSlider(SpecletParameters::PARAMETER_GENERATORFREQUENCY, sliderSignalGeneratorFrequency, treeWhosePropertyHasChanged);
}

//This method updates a combobox-index within an parameter-change-event
void SpecletAnalyzerComponent::updateComboBox(
        const juce::String &parameterName,
        juce::ComboBox *comboBox,
        const juce::ValueTree &treeWhosePropertyHasChanged) {
    juce::String changedParameterName = treeWhosePropertyHasChanged.getType().toString();
    juce::var changedParameterValue = treeWhosePropertyHasChanged.getProperty(SpecletParameters::PROPERTY_VALUE);

    if (!changedParameterName.equalsIgnoreCase(parameterName)) {
        return;
    }

    jassert(comboBox);
    comboBox->setSelectedId(static_cast<int>(changedParameterValue));

    if (parameterName == SpecletParameters::PARAMETER_TRANSFORMATION) {
        transformationChanged(static_cast<float>(changedParameterValue));
    }
    if (parameterName == SpecletParameters::PARAMETER_ROUTING) {
        routingChanged(static_cast<float>(changedParameterValue));
    }
}

//This method updates a slider-value within an parameter-change-event
void SpecletAnalyzerComponent::updateSlider(
        const juce::String &parameterName,
        juce::Slider *slider,
        const juce::ValueTree &treeWhosePropertyHasChanged) {
    juce::String changedParameterName = treeWhosePropertyHasChanged.getType().toString();
    juce::var changedParameterValue = treeWhosePropertyHasChanged.getProperty(SpecletParameters::PROPERTY_VALUE);

    if (!changedParameterName.equalsIgnoreCase(parameterName)) {
        return;
    }

    jassert(slider);
    slider->setValue(changedParameterValue, juce::dontSendNotification);
}

void SpecletAnalyzerComponent::transformationChanged(auto selectedOption) {
    if (selectedOption == enumOptionToFloat(TransformationParameters::Type::BYPASS)) {
        //Analyzer is turned off (bypass). Disable all related controls.
        comboBoxResolution->setEnabled(false);
        labelResolution->setEnabled(false);
        comboBoxWindowing->setEnabled(false);
        labelWindowing->setEnabled(false);
        comboBoxSignalGenerator->setEnabled(false);
        labelSignalGenerator->setEnabled(false);
        comboBoxRouting->setEnabled(false);
        labelRouting->setEnabled(false);
        labelSignalGeneratorFrequency->setEnabled(false);
        sliderSignalGeneratorFrequency->setEnabled(false);
        labelLogF->setEnabled(false);
        labelLogA->setEnabled(false);
        comboBoxLogF->setEnabled(false);
        comboBoxLogA->setEnabled(false);
        labelColorMode->setEnabled(false);
        comboBoxColorMode->setEnabled(false);
    } else {
        //Analyzer is turned on. Enable all related controls.
        comboBoxResolution->setEnabled(true);
        labelResolution->setEnabled(true);
        comboBoxWindowing->setEnabled(true);
        labelWindowing->setEnabled(true);
        comboBoxRouting->setEnabled(true);
        labelRouting->setEnabled(true);
        labelLogF->setEnabled(true);
        labelLogA->setEnabled(true);
        comboBoxLogF->setEnabled(true);
        comboBoxLogA->setEnabled(true);
        labelColorMode->setEnabled(true);
        comboBoxColorMode->setEnabled(true);

        routingChanged(comboBoxRouting->getSelectedId());
    }

    if (selectedOption == enumOptionToFloat(TransformationParameters::Type::FAST_FOURIER_TRANSFORM)) {
        //Disable all wavelet related controls for the Fast Fourier Transform (FFT).
        comboBoxWavelet->setEnabled(false);
        labelWavelet->setEnabled(false);
        comboBoxWaveletPacketBasis->setEnabled(false);
        labelWaveletPacketBasis->setEnabled(false);
    } else if (selectedOption == enumOptionToFloat(TransformationParameters::Type::FAST_WAVELET_TRANSFORM)) {
        //Disable Wavelet Tree/Packet related control for the dyadic Fast Wavelet Transform.
        comboBoxWavelet->setEnabled(true);
        labelWavelet->setEnabled(true);
        comboBoxWaveletPacketBasis->setEnabled(false);
        labelWaveletPacketBasis->setEnabled(false);
    } else if (selectedOption == enumOptionToFloat(TransformationParameters::Type::FAST_WAVELET_PACKET_TRANSFORM)) {
        //Enable all controls for the Fast Wavelet Packet Transform.
        comboBoxWavelet->setEnabled(true);
        labelWavelet->setEnabled(true);
        comboBoxWaveletPacketBasis->setEnabled(true);
        labelWaveletPacketBasis->setEnabled(true);
    } else if (selectedOption == enumOptionToFloat(TransformationParameters::Type::FAST_WAVELET_PACKET_BEST_BASIS_TRANSFORM)) {
        //Disable Wavelet Packet Basis controls when the best basis (based on a cost function) is chosen dynamically for the Fast Wavelet Packet Best Basis Transform
        comboBoxWavelet->setEnabled(true);
        labelWavelet->setEnabled(true);
        comboBoxWaveletPacketBasis->setEnabled(false);
        labelWaveletPacketBasis->setEnabled(false);
    } else {
        //Disable all wavelet related controls otherwise (e.g. when analyzer is bypassed)
        comboBoxWavelet->setEnabled(false);
        labelWavelet->setEnabled(false);
        comboBoxWaveletPacketBasis->setEnabled(false);
        labelWaveletPacketBasis->setEnabled(false);
    }
}

void SpecletAnalyzerComponent::routingChanged(auto selectedOption) {
    if (selectedOption == enumOptionToFloat(SpecletParameters::OptionsRouting::ROUTING_GENERATOR)) {
        //Audio source is set to generator. Enable all related controls.
        comboBoxSignalGenerator->setEnabled(true);
        labelSignalGenerator->setEnabled(true);
        labelSignalGeneratorFrequency->setEnabled(true);
        sliderSignalGeneratorFrequency->setEnabled(true);
    } else {
        //Audio source is set to input signal. Disable all related controls.
        comboBoxSignalGenerator->setEnabled(false);
        labelSignalGenerator->setEnabled(false);
        labelSignalGeneratorFrequency->setEnabled(false);
        sliderSignalGeneratorFrequency->setEnabled(false);
    }
}

template<class _Tp>
auto SpecletAnalyzerComponent::enumOptionToFloat(const _Tp &enumType) const -> float {
    auto enumValue = static_cast<typename std::underlying_type<_Tp>::type>(enumType);
    return static_cast<float>(enumValue);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SpecletAnalyzerComponent"
                 componentName="SpecletAnalyzerComponent" parentClasses="public Component, public juce::ValueTree::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330000013" fixedSize="0" initialWidth="800"
                 initialHeight="360">
  <METHODS>
    <METHOD name="visibilityChanged()"/>
    <METHOD name="parentSizeChanged()"/>
    <METHOD name="broughtToFront()"/>
    <METHOD name="childrenChanged()"/>
    <METHOD name="enablementChanged()"/>
    <METHOD name="mouseMove (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="mouseWheelMove (const MouseEvent&amp; e, float wheelIncrementX, float wheelIncrementY)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff000000"/>
  <COMBOBOX name="comboBoxResolution" id="823fcae912247fb4" memberName="comboBoxResolution"
            virtualName="" explicitFocusOrder="0" pos="0 32 100% 100%" posRelativeX="9966dea6e9dc5423"
            posRelativeY="9966dea6e9dc5423" posRelativeW="9966dea6e9dc5423"
            posRelativeH="9966dea6e9dc5423" tooltip="Aufl&#246;sung" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <VIEWPORT name="spectralviewport" id="f4608981cae94ca1" memberName="spectralviewport"
            virtualName="" explicitFocusOrder="0" pos="256 8 528 344" vscroll="0"
            hscroll="1" scrollbarThickness="10" contentType="1" jucerFile="SpecletDrawer.h"
            contentClass="SpecletDrawer" constructorParams=""/>
  <LABEL name="labelResolution" id="53146a9a1f994b4f" memberName="labelResolution"
         virtualName="" explicitFocusOrder="0" pos="0 32 100% 100%" posRelativeX="6c1a8307cfceedac"
         posRelativeY="6c1a8307cfceedac" posRelativeW="6c1a8307cfceedac"
         posRelativeH="6c1a8307cfceedac" bkgCol="30007bfc" textCol="ffffffff"
         outlineCol="ff0082f7" edTextCol="ff000000" edBkgCol="0" labelText="Aufl&#246;sung"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="comboBoxTransformation" id="9966dea6e9dc5423" memberName="comboBoxTransformation"
            virtualName="" explicitFocusOrder="0" pos="128 8 120 24" tooltip="Transformation"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <LABEL name="labelTransformation" id="6c1a8307cfceedac" memberName="labelTransformation"
         virtualName="" explicitFocusOrder="0" pos="8 8 120 24" posRelativeW="823fcae912247fb4"
         posRelativeH="823fcae912247fb4" bkgCol="30007bfc" textCol="ffffffff"
         outlineCol="ff0082f7" edTextCol="ff000000" edBkgCol="0" labelText="Transformation"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="comboBoxWindowing" id="711f655341781d5d" memberName="comboBoxWindowing"
            virtualName="" explicitFocusOrder="0" pos="0 64 100% 100%" posRelativeX="9966dea6e9dc5423"
            posRelativeY="9966dea6e9dc5423" posRelativeW="9966dea6e9dc5423"
            posRelativeH="9966dea6e9dc5423" tooltip="Fensterfunktion" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <LABEL name="labelWindowing" id="9fc54fdb4a27bff3" memberName="labelWindowing"
         virtualName="" explicitFocusOrder="0" pos="0 64 100% 100%" posRelativeX="6c1a8307cfceedac"
         posRelativeY="6c1a8307cfceedac" posRelativeW="6c1a8307cfceedac"
         posRelativeH="6c1a8307cfceedac" bkgCol="30007bfc" textCol="ffffffff"
         outlineCol="ff0082f7" edTextCol="ff000000" edBkgCol="0" labelText="Fensterfunktion"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="comboBoxWavelet" id="b2fb01da2266f006" memberName="comboBoxWavelet"
            virtualName="" explicitFocusOrder="0" pos="0 96 100% 100%" posRelativeX="9966dea6e9dc5423"
            posRelativeY="6c1a8307cfceedac" posRelativeW="9966dea6e9dc5423"
            posRelativeH="9966dea6e9dc5423" tooltip="Wavelet (+Filterordnung)"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <LABEL name="labelWavelet" id="bc6ae121f76e890b" memberName="labelWavelet"
         virtualName="" explicitFocusOrder="0" pos="0 96 100% 100%" posRelativeX="6c1a8307cfceedac"
         posRelativeY="6c1a8307cfceedac" posRelativeW="6c1a8307cfceedac"
         posRelativeH="6c1a8307cfceedac" bkgCol="30007bfc" textCol="ffffffff"
         outlineCol="ff0082f7" edTextCol="ff000000" edBkgCol="0" labelText="Wavelet"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="comboBoxWaveletPacketBasis" id="ceb2b5bc318d589" memberName="comboBoxWaveletPacketBasis"
            virtualName="" explicitFocusOrder="0" pos="0 128 100% 100%" posRelativeX="9966dea6e9dc5423"
            posRelativeY="9966dea6e9dc5423" posRelativeW="9966dea6e9dc5423"
            posRelativeH="9966dea6e9dc5423" tooltip="Wavelet Packet Basis"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <LABEL name="labelWaveletPacketBasis" id="4b1ed4f77ecd0904" memberName="labelWaveletPacketBasis"
         virtualName="" explicitFocusOrder="0" pos="0 128 100% 100%" posRelativeX="6c1a8307cfceedac"
         posRelativeY="6c1a8307cfceedac" posRelativeW="6c1a8307cfceedac"
         posRelativeH="6c1a8307cfceedac" bkgCol="30007bfc" textCol="ffffffff"
         outlineCol="ff0082f7" edTextCol="ff000000" edBkgCol="0" labelText="Wavelet Packetbasis"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="comboBoxSignalgenerator" id="db30338a07b7384f" memberName="comboBoxSignalgenerator"
            virtualName="" explicitFocusOrder="0" pos="0 288 100% 100%" posRelativeX="9966dea6e9dc5423"
            posRelativeY="9966dea6e9dc5423" posRelativeW="9966dea6e9dc5423"
            posRelativeH="9966dea6e9dc5423" tooltip="Signalgenerator" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <LABEL name="labelSignalgenerator" id="26a4e1dabfa8aed6" memberName="labelSignalgenerator"
         virtualName="" explicitFocusOrder="0" pos="0 288 100% 100%" posRelativeX="6c1a8307cfceedac"
         posRelativeY="6c1a8307cfceedac" posRelativeW="6c1a8307cfceedac"
         posRelativeH="6c1a8307cfceedac" bkgCol="30007bfc" textCol="ffffffff"
         outlineCol="ff0082f7" edTextCol="ff000000" edBkgCol="0" labelText="Signalgenerator"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="comboBoxRouting" id="bac855bba2b60061" memberName="comboBoxRouting"
            virtualName="" explicitFocusOrder="0" pos="0 160 100% 100%" posRelativeX="9966dea6e9dc5423"
            posRelativeY="9966dea6e9dc5423" posRelativeW="9966dea6e9dc5423"
            posRelativeH="9966dea6e9dc5423" tooltip="Routing" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <LABEL name="labelRouting" id="a1fd5c3b9d2287a4" memberName="labelRouting"
         virtualName="" explicitFocusOrder="0" pos="0 160 100% 100%" posRelativeX="6c1a8307cfceedac"
         posRelativeY="6c1a8307cfceedac" posRelativeW="6c1a8307cfceedac"
         posRelativeH="6c1a8307cfceedac" bkgCol="30007bfc" textCol="ffffffff"
         outlineCol="ff0082f7" edTextCol="ff000000" edBkgCol="0" labelText="Routing"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="labelGeneratorfrequenz" id="1c0a811f824ca806" memberName="labelGeneratorfrequenz"
         virtualName="" explicitFocusOrder="0" pos="0 320 100% 100%" posRelativeX="6c1a8307cfceedac"
         posRelativeY="6c1a8307cfceedac" posRelativeW="6c1a8307cfceedac"
         posRelativeH="6c1a8307cfceedac" bkgCol="30007bfc" textCol="ffffffff"
         outlineCol="ff0082f7" edTextCol="ff000000" edBkgCol="0" labelText="Generatorfrequenz"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderGeneratorFrequenz" id="cb30d09188430ead" memberName="sliderGeneratorFrequenz"
          virtualName="" explicitFocusOrder="7" pos="0 320 100% 100%" posRelativeX="9966dea6e9dc5423"
          posRelativeY="9966dea6e9dc5423" posRelativeW="9966dea6e9dc5423"
          posRelativeH="9966dea6e9dc5423" tooltip="Frequenz des Signalgenerators"
          min="10" max="22000" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="50" textBoxHeight="25" skewFactor="1"/>
  <LABEL name="labelLogF" id="5cd8a3422f31e3f9" memberName="labelLogF"
         virtualName="" explicitFocusOrder="0" pos="0 192 100% 100%" posRelativeX="6c1a8307cfceedac"
         posRelativeY="6c1a8307cfceedac" posRelativeW="6c1a8307cfceedac"
         posRelativeH="6c1a8307cfceedac" bkgCol="30007bfc" textCol="ffffffff"
         outlineCol="ff0082f7" edTextCol="ff000000" edBkgCol="0" labelText="Frequenzachse"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="labelLogA" id="d3d719bacacd6960" memberName="labelLogA"
         virtualName="" explicitFocusOrder="0" pos="0 224 100% 100%" posRelativeX="6c1a8307cfceedac"
         posRelativeY="6c1a8307cfceedac" posRelativeW="6c1a8307cfceedac"
         posRelativeH="6c1a8307cfceedac" bkgCol="30007bfc" textCol="ffffffff"
         outlineCol="ff0082f7" edTextCol="ff000000" edBkgCol="0" labelText="Amplitudenachse"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="comboBoxLogF" id="c7002823f6acf7c7" memberName="comboBoxLogF"
            virtualName="" explicitFocusOrder="0" pos="0 192 100% 100%" posRelativeX="9966dea6e9dc5423"
            posRelativeY="9966dea6e9dc5423" posRelativeW="9966dea6e9dc5423"
            posRelativeH="9966dea6e9dc5423" tooltip="Frequenzachse" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <COMBOBOX name="comboBoxLogA" id="49fc8ee401d57810" memberName="comboBoxLogA"
            virtualName="" explicitFocusOrder="0" pos="0 224 100% 100%" posRelativeX="9966dea6e9dc5423"
            posRelativeY="9966dea6e9dc5423" posRelativeW="9966dea6e9dc5423"
            posRelativeH="9966dea6e9dc5423" tooltip="Logarithmische Amplitudenachse"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <LABEL name="labelColorMode" id="ce127858e1693c03" memberName="labelColorMode"
         virtualName="" explicitFocusOrder="0" pos="1 256 100% 100%" posRelativeX="6c1a8307cfceedac"
         posRelativeY="6c1a8307cfceedac" posRelativeW="6c1a8307cfceedac"
         posRelativeH="6c1a8307cfceedac" bkgCol="30007bfc" textCol="ffffffff"
         outlineCol="ff0082f7" edTextCol="ff000000" edBkgCol="0" labelText="Farbmodus"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="comboBoxColorMode" id="2ba4ef95fad9e3ee" memberName="comboBoxColorMode"
            virtualName="" explicitFocusOrder="0" pos="1 256 100% 100%" posRelativeX="9966dea6e9dc5423"
            posRelativeY="9966dea6e9dc5423" posRelativeW="9966dea6e9dc5423"
            posRelativeH="9966dea6e9dc5423" tooltip="Farbmodus" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
