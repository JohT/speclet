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
#include "SpecletDrawer.h"

//[/Headers]

#include "SpecletAnalyzerComponent.h"
#include <cstddef>


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
      comboBoxWaveletPaketBasis(nullptr),
      labelWaveletPaketBasis(nullptr),
      comboBoxSignalgenerator(nullptr),
      labelSignalgenerator(nullptr),
      comboBoxSignalquelle(nullptr),
      labelSignalquelle(nullptr),
      labelGeneratorfrequenz(nullptr),
      sliderGeneratorFrequenz(nullptr),
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

    addAndMakeVisible(comboBoxWaveletPaketBasis = new juce::ComboBox("comboBoxWaveletPaketBasis"));
    comboBoxWaveletPaketBasis->setTooltip("Wavelet Packet Base");
    comboBoxWaveletPaketBasis->setEditableText(false);
    comboBoxWaveletPaketBasis->setJustificationType(juce::Justification::centredLeft);
    comboBoxWaveletPaketBasis->setTextWhenNothingSelected(juce::String());
    comboBoxWaveletPaketBasis->setTextWhenNoChoicesAvailable(juce::String());
    comboBoxWaveletPaketBasis->addListener(this);

    addAndMakeVisible(labelWaveletPaketBasis = new juce::Label("labelWaveletPaketBasis",
                                                         "Wavelet Packet Base"));
    labelWaveletPaketBasis->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelWaveletPaketBasis->setJustificationType(juce::Justification::centredLeft);
    labelWaveletPaketBasis->setEditable(false, false, false);
    labelWaveletPaketBasis->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelWaveletPaketBasis->setColour(juce::Label::textColourId, juce::Colours::white);
    labelWaveletPaketBasis->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelWaveletPaketBasis->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelWaveletPaketBasis->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(comboBoxSignalgenerator = new juce::ComboBox("comboBoxSignalgenerator"));
    comboBoxSignalgenerator->setTooltip("Osciallator");
    comboBoxSignalgenerator->setEditableText(false);
    comboBoxSignalgenerator->setJustificationType(juce::Justification::centredLeft);
    comboBoxSignalgenerator->setTextWhenNothingSelected(juce::String());
    comboBoxSignalgenerator->setTextWhenNoChoicesAvailable(juce::String());
    comboBoxSignalgenerator->addListener(this);

    addAndMakeVisible(labelSignalgenerator = new juce::Label("labelSignalgenerator",
                                                       "Oscillator"));
    labelSignalgenerator->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelSignalgenerator->setJustificationType(juce::Justification::centredLeft);
    labelSignalgenerator->setEditable(false, false, false);
    labelSignalgenerator->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelSignalgenerator->setColour(juce::Label::textColourId, juce::Colours::white);
    labelSignalgenerator->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelSignalgenerator->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelSignalgenerator->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(comboBoxSignalquelle = new juce::ComboBox("comboBoxSignalquelle"));
    comboBoxSignalquelle->setTooltip("Audio Source");
    comboBoxSignalquelle->setEditableText(false);
    comboBoxSignalquelle->setJustificationType(juce::Justification::centredLeft);
    comboBoxSignalquelle->setTextWhenNothingSelected(juce::String());
    comboBoxSignalquelle->setTextWhenNoChoicesAvailable(juce::String());
    comboBoxSignalquelle->addListener(this);

    addAndMakeVisible(labelSignalquelle = new juce::Label("labelSignalquelle",
                                                    "Audio Source"));
    labelSignalquelle->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelSignalquelle->setJustificationType(juce::Justification::centredLeft);
    labelSignalquelle->setEditable(false, false, false);
    labelSignalquelle->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelSignalquelle->setColour(juce::Label::textColourId, juce::Colours::white);
    labelSignalquelle->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelSignalquelle->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelSignalquelle->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(labelGeneratorfrequenz = new juce::Label("labelGeneratorfrequenz",
                                                         "Oscillator Frequency"));
    labelGeneratorfrequenz->setFont(juce::Font(15.0000f, juce::Font::plain));
    labelGeneratorfrequenz->setJustificationType(juce::Justification::centredLeft);
    labelGeneratorfrequenz->setEditable(false, false, false);
    labelGeneratorfrequenz->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    labelGeneratorfrequenz->setColour(juce::Label::textColourId, juce::Colours::white);
    labelGeneratorfrequenz->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    labelGeneratorfrequenz->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    labelGeneratorfrequenz->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));

    addAndMakeVisible(sliderGeneratorFrequenz = new juce::Slider("sliderGeneratorFrequenz"));
    sliderGeneratorFrequenz->setTooltip("Oscillator Frequency");
    sliderGeneratorFrequenz->setExplicitFocusOrder(7);
    sliderGeneratorFrequenz->setRange(10, 22000, 1);
    sliderGeneratorFrequenz->setSliderStyle(juce::Slider::LinearHorizontal);
    sliderGeneratorFrequenz->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 25);
    sliderGeneratorFrequenz->addListener(this);

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
    //registeres itself as listener for parameter-changes
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
    deleteAndZero(comboBoxWaveletPaketBasis);
    deleteAndZero(labelWaveletPaketBasis);
    deleteAndZero(comboBoxSignalgenerator);
    deleteAndZero(labelSignalgenerator);
    deleteAndZero(comboBoxSignalquelle);
    deleteAndZero(labelSignalquelle);
    deleteAndZero(labelGeneratorfrequenz);
    deleteAndZero(sliderGeneratorFrequenz);
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
    comboBoxWaveletPaketBasis->setBounds((128) + 0, (8) + 128, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    labelWaveletPaketBasis->setBounds((8) + 0, (8) + 128, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    comboBoxSignalgenerator->setBounds((128) + 0, (8) + 288, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    labelSignalgenerator->setBounds((8) + 0, (8) + 288, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    comboBoxSignalquelle->setBounds((128) + 0, (8) + 160, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    labelSignalquelle->setBounds((8) + 0, (8) + 160, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    labelGeneratorfrequenz->setBounds((8) + 0, (8) + 320, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
    sliderGeneratorFrequenz->setBounds((128) + 0, (8) + 320, juce::roundToInt((120) * 1.0000f), juce::roundToInt((24) * 1.0000f));
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
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_Transformation, comboBoxTransformation->getSelectedId());
        //[/UserComboBoxCode_comboBoxTransformation]
    } else if (comboBoxThatHasChanged == comboBoxWindowing) {
        //[UserComboBoxCode_comboBoxWindowing] -- add your combo box handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_Windowing, comboBoxWindowing->getSelectedId());
        //[/UserComboBoxCode_comboBoxWindowing]
    } else if (comboBoxThatHasChanged == comboBoxWavelet) {
        //[UserComboBoxCode_comboBoxWavelet] -- add your combo box handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_Wavelet, comboBoxWavelet->getSelectedId());
        //[/UserComboBoxCode_comboBoxWavelet]
    } else if (comboBoxThatHasChanged == comboBoxWaveletPaketBasis) {
        //[UserComboBoxCode_comboBoxWaveletPaketBasis] -- add your combo box handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_WaveletPacketBase, comboBoxWaveletPaketBasis->getSelectedId());
        //[/UserComboBoxCode_comboBoxWaveletPaketBasis]
    } else if (comboBoxThatHasChanged == comboBoxSignalgenerator) {
        //[UserComboBoxCode_comboBoxSignalgenerator] -- add your combo box handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_Generator, comboBoxSignalgenerator->getSelectedId());
        //[/UserComboBoxCode_comboBoxSignalgenerator]
    } else if (comboBoxThatHasChanged == comboBoxSignalquelle) {
        //[UserComboBoxCode_comboBoxSignalquelle] -- add your combo box handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_Routing, comboBoxSignalquelle->getSelectedId());
        //[/UserComboBoxCode_comboBoxSignalquelle]
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

    if (sliderThatWasMoved == sliderGeneratorFrequenz) {
        //[UserSliderCode_sliderGeneratorFrequenz] -- add your slider handling code here..
        parameters->setParameter(SpecletParameters::PARAMETER_INDEX_GeneratorFrequency, sliderGeneratorFrequenz->getValue());
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
    if ((e.mouseWasClicked()) && (e.mods.isRightButtonDown())) {
        popupMenu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this), [this](int index) {
            if (index == POPUPMENU_INDEX_1_ABOUT) {
                juce::AlertWindow::AlertIconType icon = juce::AlertWindow::InfoIcon;
                juce::String message;
                message += ("Written by Johannes Troppacher (c)2011\n");
                message += ("\n");
                message += ("Framework 'JUCE' by Raw Material Software\n");
                message += ("VST-Interface 'VST SDK 2.4 rev2' by Steinberg\n");
                message += ("FFT-Library 'FFTW' by MIT (Matteo Frigo and Steven G. Johnson)\n");
                message += ("Wavelet-Library 'wave++' by Ryerson Computrational Signal Analysis Group");
                message += (" (S. E. Ferrando, L. A. Kolasa and N. Kovacevic)");
                juce::AlertWindow::showOkCancelBox(icon, "About Speclet", message, "OK", "", this, nullptr);
            }
        });
    }
    //[/UserCode_mouseDown]
}

void SpecletAnalyzerComponent::mouseWheelMove (const juce::MouseEvent& /* event */, const juce::MouseWheelDetails& /* wheel */) {
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

    comboBoxTransformation->addItem("FFT", SpecletParameters::TRANSFORM_FFT);
    comboBoxTransformation->addItem("FWT", SpecletParameters::TRANSFORM_FWT);
    comboBoxTransformation->addItem("WPT", SpecletParameters::TRANSFORM_FWPT);
    comboBoxTransformation->addItem("WPT BestBase", SpecletParameters::TRANSFORM_FWPT_BB);
    comboBoxTransformation->addItem("Off", SpecletParameters::TRANSFORM_OFF);

    comboBoxWindowing->addItem("Barlett", SpecletParameters::WINDOWING_BARTLETT);
    comboBoxWindowing->addItem("Blackman", SpecletParameters::WINDOWING_BLACKMAN);
    comboBoxWindowing->addItem("Blackman-Harris", SpecletParameters::WINDOWING_BLACKMAN_HARRIS);
    comboBoxWindowing->addItem("Hamming", SpecletParameters::WINDOWING_HAMMING);
    comboBoxWindowing->addItem("Hann", SpecletParameters::WINDOWING_HANN);
    comboBoxWindowing->addItem("Welch", SpecletParameters::WINDOWING_WELCH);
    comboBoxWindowing->addItem("Parzen", SpecletParameters::WINDOWING_PARZEN);
    comboBoxWindowing->addItem("Rectangular", SpecletParameters::WINDOWING_RECTANGULAR);

    comboBoxWavelet->addItem("Haar (2)", SpecletParameters::WAVELET_HAAR);
    comboBoxWavelet->addItem("Daubechies (4)", SpecletParameters::WAVELET_DAUBECHIES_04);
    comboBoxWavelet->addItem("Daubechies (6)", SpecletParameters::WAVELET_DAUBECHIES_06);
    comboBoxWavelet->addItem("Daubechies (8)", SpecletParameters::WAVELET_DAUBECHIES_08);
    comboBoxWavelet->addItem("Daubechies (10)", SpecletParameters::WAVELET_DAUBECHIES_10);
    comboBoxWavelet->addItem("Daubechies (12)", SpecletParameters::WAVELET_DAUBECHIES_12);
    comboBoxWavelet->addItem("Daubechies (14)", SpecletParameters::WAVELET_DAUBECHIES_14);
    comboBoxWavelet->addItem("Daubechies (16)", SpecletParameters::WAVELET_DAUBECHIES_16);
    comboBoxWavelet->addItem("Daubechies (18)", SpecletParameters::WAVELET_DAUBECHIES_18);
    comboBoxWavelet->addItem("Daubechies (20)", SpecletParameters::WAVELET_DAUBECHIES_20);
    comboBoxWavelet->addItem("Coifman (6)", SpecletParameters::WAVELET_COIFMAN_06);
    comboBoxWavelet->addItem("Coifman (12)", SpecletParameters::WAVELET_COIFMAN_12);
    comboBoxWavelet->addItem("Coifman (18)", SpecletParameters::WAVELET_COIFMAN_18);
    comboBoxWavelet->addItem("Coifman (24)", SpecletParameters::WAVELET_COIFMAN_24);
    comboBoxWavelet->addItem("Coifman (30)", SpecletParameters::WAVELET_COIFMAN_30);
    comboBoxWavelet->addItem("Beylkin (18)", SpecletParameters::WAVELET_BEYLKIN_18);
    comboBoxWavelet->addItem("Vaidyanathan (18)", SpecletParameters::WAVELET_VAIDYANATHAN_18);

    comboBoxWaveletPaketBasis->addItem("freq/time x1", SpecletParameters::RESOLUTION_RATIO_Equal);
    comboBoxWaveletPaketBasis->addItem("freq x2", SpecletParameters::RESOLUTION_RATIO_FreqX2);
    comboBoxWaveletPaketBasis->addItem("freq x4", SpecletParameters::RESOLUTION_RATIO_FreqX4);
    comboBoxWaveletPaketBasis->addItem("time x2", SpecletParameters::RESOLUTION_RATIO_TimeX2);
    comboBoxWaveletPaketBasis->addItem("time x4", SpecletParameters::RESOLUTION_RATIO_TimeX4);

    comboBoxSignalgenerator->addItem("Sine", SpecletParameters::GENERATOR_SINE);
    comboBoxSignalgenerator->addItem("Triangle", SpecletParameters::GENERATOR_TRIANGLE);
    comboBoxSignalgenerator->addItem("Sawtooth", SpecletParameters::GENERATOR_RAMP);
    comboBoxSignalgenerator->addItem("Rectangle", SpecletParameters::GENERATOR_SQUARE);
    comboBoxSignalgenerator->addItem("Noise", SpecletParameters::GENERATOR_NOISE);

    comboBoxSignalquelle->addItem("Mid", SpecletParameters::ROUTING_MID);
    comboBoxSignalquelle->addItem("Side", SpecletParameters::ROUTING_SIDE);
    comboBoxSignalquelle->addItem("Left", SpecletParameters::ROUTING_L);
    comboBoxSignalquelle->addItem("Right", SpecletParameters::ROUTING_R);
    comboBoxSignalquelle->addItem("Oscillator", SpecletParameters::ROUTING_GENERATOR);

    comboBoxLogF->addItem("linear", SpecletParameters::PLOT_AXIS_LINEAR);
    comboBoxLogF->addItem("logarithmic", SpecletParameters::PLOT_AXIS_LOGARITHMIC);

    comboBoxLogA->addItem("linear", SpecletParameters::PLOT_AXIS_LINEAR);
    comboBoxLogA->addItem("logarithmic", SpecletParameters::PLOT_AXIS_LOGARITHMIC);

    comboBoxColorMode->addItem("Blue", SpecletParameters::COLORMODE_BLUE);
    comboBoxColorMode->addItem("Green", SpecletParameters::COLORMODE_GREEN);
    comboBoxColorMode->addItem("Fire", SpecletParameters::COLORMODE_FIRE);
    comboBoxColorMode->addItem("Rainbow", SpecletParameters::COLORMODE_RAINBOW);
}

void SpecletAnalyzerComponent::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &  /*property*/) {
    const juce::ScopedLock myScopedLock(criticalSection);

    updateComboBox(SpecletParameters::PARAMETER_COLORMODE, comboBoxColorMode, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_GENERATOR, comboBoxSignalgenerator, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_LOGFREQUENCY, comboBoxLogF, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_LOGMAGNITUDE, comboBoxLogA, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_RESOLUTION, comboBoxResolution, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_ROUTING, comboBoxSignalquelle, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_TRANSFORMATION, comboBoxTransformation, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_WAVELET, comboBoxWavelet, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_WAVELETPACKETBASE, comboBoxWaveletPaketBasis, treeWhosePropertyHasChanged);
    updateComboBox(SpecletParameters::PARAMETER_WINDOWING, comboBoxWindowing, treeWhosePropertyHasChanged);
    updateSlider(SpecletParameters::PARAMETER_GENERATORFREQUENCY, sliderGeneratorFrequenz, treeWhosePropertyHasChanged);
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
    comboBox->setSelectedId((int) changedParameterValue, true);
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
  <COMBOBOX name="comboBoxWaveletPaketBasis" id="ceb2b5bc318d589" memberName="comboBoxWaveletPaketBasis"
            virtualName="" explicitFocusOrder="0" pos="0 128 100% 100%" posRelativeX="9966dea6e9dc5423"
            posRelativeY="9966dea6e9dc5423" posRelativeW="9966dea6e9dc5423"
            posRelativeH="9966dea6e9dc5423" tooltip="Wavelet Paket Basis"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <LABEL name="labelWaveletPaketBasis" id="4b1ed4f77ecd0904" memberName="labelWaveletPaketBasis"
         virtualName="" explicitFocusOrder="0" pos="0 128 100% 100%" posRelativeX="6c1a8307cfceedac"
         posRelativeY="6c1a8307cfceedac" posRelativeW="6c1a8307cfceedac"
         posRelativeH="6c1a8307cfceedac" bkgCol="30007bfc" textCol="ffffffff"
         outlineCol="ff0082f7" edTextCol="ff000000" edBkgCol="0" labelText="Wavelet Paketbasis"
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
  <COMBOBOX name="comboBoxSignalquelle" id="bac855bba2b60061" memberName="comboBoxSignalquelle"
            virtualName="" explicitFocusOrder="0" pos="0 160 100% 100%" posRelativeX="9966dea6e9dc5423"
            posRelativeY="9966dea6e9dc5423" posRelativeW="9966dea6e9dc5423"
            posRelativeH="9966dea6e9dc5423" tooltip="Signalquelle" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <LABEL name="labelSignalquelle" id="a1fd5c3b9d2287a4" memberName="labelSignalquelle"
         virtualName="" explicitFocusOrder="0" pos="0 160 100% 100%" posRelativeX="6c1a8307cfceedac"
         posRelativeY="6c1a8307cfceedac" posRelativeW="6c1a8307cfceedac"
         posRelativeH="6c1a8307cfceedac" bkgCol="30007bfc" textCol="ffffffff"
         outlineCol="ff0082f7" edTextCol="ff000000" edBkgCol="0" labelText="Signalquelle"
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
