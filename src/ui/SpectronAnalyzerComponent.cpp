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
#include "SpectronDrawer.h"

//[/Headers]

#include "SpectronAnalyzerComponent.h"
#include <cstddef>


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SpectronAnalyzerComponent::SpectronAnalyzerComponent()
    : Component("SpectronAnalyzerComponent"),
      comboBoxResolution(0),
      spectralviewport(0),
      labelResolution(0),
      comboBoxTransformation(0),
      labelTransformation(0),
      comboBoxWindowing(0),
      labelWindowing(0),
      comboBoxWavelet(0),
      labelWavelet(0),
      comboBoxWaveletPaketBasis(0),
      labelWaveletPaketBasis(0),
      comboBoxSignalgenerator(0),
      labelSignalgenerator(0),
      comboBoxSignalquelle(0),
      labelSignalquelle(0),
      labelGeneratorfrequenz(0),
      sliderGeneratorFrequenz(0),
      labelLogF(0),
      labelLogA(0),
      comboBoxLogF(0),
      comboBoxLogA(0),
      labelColorMode(0),
      comboBoxColorMode(0) {
    addAndMakeVisible(comboBoxResolution = new ComboBox("comboBoxResolution"));
    comboBoxResolution->setTooltip("Resolution");
    comboBoxResolution->setEditableText(false);
    comboBoxResolution->setJustificationType(Justification::centredLeft);
    comboBoxResolution->setTextWhenNothingSelected(String());
    comboBoxResolution->setTextWhenNoChoicesAvailable(String());
    comboBoxResolution->addListener(this);

    addAndMakeVisible(spectralviewport = new Viewport("spectralviewport"));
    spectralviewport->setScrollBarsShown(false, true);
    spectralviewport->setScrollBarThickness(10);

    addAndMakeVisible(labelResolution = new Label("labelResolution",
                                                  "Resolution"));
    labelResolution->setFont(Font(15.0000f, Font::plain));
    labelResolution->setJustificationType(Justification::centredLeft);
    labelResolution->setEditable(false, false, false);
    labelResolution->setColour(Label::backgroundColourId, Colour(0x30007bfc));
    labelResolution->setColour(Label::textColourId, Colours::white);
    labelResolution->setColour(Label::outlineColourId, Colour(0xff0082f7));
    labelResolution->setColour(TextEditor::textColourId, Colours::black);
    labelResolution->setColour(TextEditor::backgroundColourId, Colour(0x0));

    addAndMakeVisible(comboBoxTransformation = new ComboBox("comboBoxTransformation"));
    comboBoxTransformation->setTooltip("Transformation");
    comboBoxTransformation->setEditableText(false);
    comboBoxTransformation->setJustificationType(Justification::centredLeft);
    comboBoxTransformation->setTextWhenNothingSelected(String());
    comboBoxTransformation->setTextWhenNoChoicesAvailable(String());
    comboBoxTransformation->addListener(this);

    addAndMakeVisible(labelTransformation = new Label("labelTransformation",
                                                      "Transformation"));
    labelTransformation->setFont(Font(15.0000f, Font::plain));
    labelTransformation->setJustificationType(Justification::centredLeft);
    labelTransformation->setEditable(false, false, false);
    labelTransformation->setColour(Label::backgroundColourId, Colour(0x30007bfc));
    labelTransformation->setColour(Label::textColourId, Colours::white);
    labelTransformation->setColour(Label::outlineColourId, Colour(0xff0082f7));
    labelTransformation->setColour(TextEditor::textColourId, Colours::black);
    labelTransformation->setColour(TextEditor::backgroundColourId, Colour(0x0));

    addAndMakeVisible(comboBoxWindowing = new ComboBox("comboBoxWindowing"));
    comboBoxWindowing->setTooltip("Window Function");
    comboBoxWindowing->setEditableText(false);
    comboBoxWindowing->setJustificationType(Justification::centredLeft);
    comboBoxWindowing->setTextWhenNothingSelected(String());
    comboBoxWindowing->setTextWhenNoChoicesAvailable(String());
    comboBoxWindowing->addListener(this);

    addAndMakeVisible(labelWindowing = new Label("labelWindowing",
                                                 "Window Function"));
    labelWindowing->setFont(Font(15.0000f, Font::plain));
    labelWindowing->setJustificationType(Justification::centredLeft);
    labelWindowing->setEditable(false, false, false);
    labelWindowing->setColour(Label::backgroundColourId, Colour(0x30007bfc));
    labelWindowing->setColour(Label::textColourId, Colours::white);
    labelWindowing->setColour(Label::outlineColourId, Colour(0xff0082f7));
    labelWindowing->setColour(TextEditor::textColourId, Colours::black);
    labelWindowing->setColour(TextEditor::backgroundColourId, Colour(0x0));

    addAndMakeVisible(comboBoxWavelet = new ComboBox("comboBoxWavelet"));
    comboBoxWavelet->setTooltip("Wavelet");
    comboBoxWavelet->setEditableText(false);
    comboBoxWavelet->setJustificationType(Justification::centredLeft);
    comboBoxWavelet->setTextWhenNothingSelected(String());
    comboBoxWavelet->setTextWhenNoChoicesAvailable(String());
    comboBoxWavelet->addListener(this);

    addAndMakeVisible(labelWavelet = new Label("labelWavelet",
                                               "Wavelet"));
    labelWavelet->setFont(Font(15.0000f, Font::plain));
    labelWavelet->setJustificationType(Justification::centredLeft);
    labelWavelet->setEditable(false, false, false);
    labelWavelet->setColour(Label::backgroundColourId, Colour(0x30007bfc));
    labelWavelet->setColour(Label::textColourId, Colours::white);
    labelWavelet->setColour(Label::outlineColourId, Colour(0xff0082f7));
    labelWavelet->setColour(TextEditor::textColourId, Colours::black);
    labelWavelet->setColour(TextEditor::backgroundColourId, Colour(0x0));

    addAndMakeVisible(comboBoxWaveletPaketBasis = new ComboBox("comboBoxWaveletPaketBasis"));
    comboBoxWaveletPaketBasis->setTooltip("Wavelet Packet Base");
    comboBoxWaveletPaketBasis->setEditableText(false);
    comboBoxWaveletPaketBasis->setJustificationType(Justification::centredLeft);
    comboBoxWaveletPaketBasis->setTextWhenNothingSelected(String());
    comboBoxWaveletPaketBasis->setTextWhenNoChoicesAvailable(String());
    comboBoxWaveletPaketBasis->addListener(this);

    addAndMakeVisible(labelWaveletPaketBasis = new Label("labelWaveletPaketBasis",
                                                         "Wavelet Packet Base"));
    labelWaveletPaketBasis->setFont(Font(15.0000f, Font::plain));
    labelWaveletPaketBasis->setJustificationType(Justification::centredLeft);
    labelWaveletPaketBasis->setEditable(false, false, false);
    labelWaveletPaketBasis->setColour(Label::backgroundColourId, Colour(0x30007bfc));
    labelWaveletPaketBasis->setColour(Label::textColourId, Colours::white);
    labelWaveletPaketBasis->setColour(Label::outlineColourId, Colour(0xff0082f7));
    labelWaveletPaketBasis->setColour(TextEditor::textColourId, Colours::black);
    labelWaveletPaketBasis->setColour(TextEditor::backgroundColourId, Colour(0x0));

    addAndMakeVisible(comboBoxSignalgenerator = new ComboBox("comboBoxSignalgenerator"));
    comboBoxSignalgenerator->setTooltip("Osciallator");
    comboBoxSignalgenerator->setEditableText(false);
    comboBoxSignalgenerator->setJustificationType(Justification::centredLeft);
    comboBoxSignalgenerator->setTextWhenNothingSelected(String());
    comboBoxSignalgenerator->setTextWhenNoChoicesAvailable(String());
    comboBoxSignalgenerator->addListener(this);

    addAndMakeVisible(labelSignalgenerator = new Label("labelSignalgenerator",
                                                       "Oscillator"));
    labelSignalgenerator->setFont(Font(15.0000f, Font::plain));
    labelSignalgenerator->setJustificationType(Justification::centredLeft);
    labelSignalgenerator->setEditable(false, false, false);
    labelSignalgenerator->setColour(Label::backgroundColourId, Colour(0x30007bfc));
    labelSignalgenerator->setColour(Label::textColourId, Colours::white);
    labelSignalgenerator->setColour(Label::outlineColourId, Colour(0xff0082f7));
    labelSignalgenerator->setColour(TextEditor::textColourId, Colours::black);
    labelSignalgenerator->setColour(TextEditor::backgroundColourId, Colour(0x0));

    addAndMakeVisible(comboBoxSignalquelle = new ComboBox("comboBoxSignalquelle"));
    comboBoxSignalquelle->setTooltip("Audio Source");
    comboBoxSignalquelle->setEditableText(false);
    comboBoxSignalquelle->setJustificationType(Justification::centredLeft);
    comboBoxSignalquelle->setTextWhenNothingSelected(String());
    comboBoxSignalquelle->setTextWhenNoChoicesAvailable(String());
    comboBoxSignalquelle->addListener(this);

    addAndMakeVisible(labelSignalquelle = new Label("labelSignalquelle",
                                                    "Audio Source"));
    labelSignalquelle->setFont(Font(15.0000f, Font::plain));
    labelSignalquelle->setJustificationType(Justification::centredLeft);
    labelSignalquelle->setEditable(false, false, false);
    labelSignalquelle->setColour(Label::backgroundColourId, Colour(0x30007bfc));
    labelSignalquelle->setColour(Label::textColourId, Colours::white);
    labelSignalquelle->setColour(Label::outlineColourId, Colour(0xff0082f7));
    labelSignalquelle->setColour(TextEditor::textColourId, Colours::black);
    labelSignalquelle->setColour(TextEditor::backgroundColourId, Colour(0x0));

    addAndMakeVisible(labelGeneratorfrequenz = new Label("labelGeneratorfrequenz",
                                                         "Oscillator Frequency"));
    labelGeneratorfrequenz->setFont(Font(15.0000f, Font::plain));
    labelGeneratorfrequenz->setJustificationType(Justification::centredLeft);
    labelGeneratorfrequenz->setEditable(false, false, false);
    labelGeneratorfrequenz->setColour(Label::backgroundColourId, Colour(0x30007bfc));
    labelGeneratorfrequenz->setColour(Label::textColourId, Colours::white);
    labelGeneratorfrequenz->setColour(Label::outlineColourId, Colour(0xff0082f7));
    labelGeneratorfrequenz->setColour(TextEditor::textColourId, Colours::black);
    labelGeneratorfrequenz->setColour(TextEditor::backgroundColourId, Colour(0x0));

    addAndMakeVisible(sliderGeneratorFrequenz = new Slider("sliderGeneratorFrequenz"));
    sliderGeneratorFrequenz->setTooltip("Oscillator Frequency");
    sliderGeneratorFrequenz->setExplicitFocusOrder(7);
    sliderGeneratorFrequenz->setRange(10, 22000, 1);
    sliderGeneratorFrequenz->setSliderStyle(Slider::LinearHorizontal);
    sliderGeneratorFrequenz->setTextBoxStyle(Slider::TextBoxLeft, false, 50, 25);
    sliderGeneratorFrequenz->addListener(this);

    addAndMakeVisible(labelLogF = new Label("labelLogF",
                                            "Frequency Scale"));
    labelLogF->setFont(Font(15.0000f, Font::plain));
    labelLogF->setJustificationType(Justification::centredLeft);
    labelLogF->setEditable(false, false, false);
    labelLogF->setColour(Label::backgroundColourId, Colour(0x30007bfc));
    labelLogF->setColour(Label::textColourId, Colours::white);
    labelLogF->setColour(Label::outlineColourId, Colour(0xff0082f7));
    labelLogF->setColour(TextEditor::textColourId, Colours::black);
    labelLogF->setColour(TextEditor::backgroundColourId, Colour(0x0));

    addAndMakeVisible(labelLogA = new Label("labelLogA",
                                            "Magnitude Scale"));
    labelLogA->setFont(Font(15.0000f, Font::plain));
    labelLogA->setJustificationType(Justification::centredLeft);
    labelLogA->setEditable(false, false, false);
    labelLogA->setColour(Label::backgroundColourId, Colour(0x30007bfc));
    labelLogA->setColour(Label::textColourId, Colours::white);
    labelLogA->setColour(Label::outlineColourId, Colour(0xff0082f7));
    labelLogA->setColour(TextEditor::textColourId, Colours::black);
    labelLogA->setColour(TextEditor::backgroundColourId, Colour(0x0));

    addAndMakeVisible(comboBoxLogF = new ComboBox("comboBoxLogF"));
    comboBoxLogF->setTooltip("Frequency Scale");
    comboBoxLogF->setEditableText(false);
    comboBoxLogF->setJustificationType(Justification::centredLeft);
    comboBoxLogF->setTextWhenNothingSelected(String());
    comboBoxLogF->setTextWhenNoChoicesAvailable(String());
    comboBoxLogF->addListener(this);

    addAndMakeVisible(comboBoxLogA = new ComboBox("comboBoxLogA"));
    comboBoxLogA->setTooltip("Magnitude Scale");
    comboBoxLogA->setEditableText(false);
    comboBoxLogA->setJustificationType(Justification::centredLeft);
    comboBoxLogA->setTextWhenNothingSelected(String());
    comboBoxLogA->setTextWhenNoChoicesAvailable(String());
    comboBoxLogA->addListener(this);

    addAndMakeVisible(labelColorMode = new Label("labelColorMode",
                                                 "Color Mode"));
    labelColorMode->setFont(Font(15.0000f, Font::plain));
    labelColorMode->setJustificationType(Justification::centredLeft);
    labelColorMode->setEditable(false, false, false);
    labelColorMode->setColour(Label::backgroundColourId, Colour(0x30007bfc));
    labelColorMode->setColour(Label::textColourId, Colours::white);
    labelColorMode->setColour(Label::outlineColourId, Colour(0xff0082f7));
    labelColorMode->setColour(TextEditor::textColourId, Colours::black);
    labelColorMode->setColour(TextEditor::backgroundColourId, Colour(0x0));

    addAndMakeVisible(comboBoxColorMode = new ComboBox("comboBoxColorMode"));
    comboBoxColorMode->setTooltip("Color Mode");
    comboBoxColorMode->setEditableText(false);
    comboBoxColorMode->setJustificationType(Justification::centredLeft);
    comboBoxColorMode->setTextWhenNothingSelected(String());
    comboBoxColorMode->setTextWhenNoChoicesAvailable(String());
    comboBoxColorMode->addListener(this);


    //[UserPreSize]
    fillComboBoxes();
    //[/UserPreSize]

    setSize(800, 360);

    //[Constructor] You can add your own custom stuff here..

    //gets the pointer to the parameters singelton - for a better readability
    parameters = &SpectronParameters::getSingletonInstance();
    //registeres itself as listener for parameter-changes
    SpectronParameters::getSingletonInstance().addListener(this);
    DBG("SpectronAnalyzerComponent as parameter listener added");
    LOG("SpectronAnalyzerComponent as parameter listener added");
    //adds spectrum drawing component to the scrollable view
    spectralviewport->setViewedComponent(new SpectronDrawer());

    //adds entries to the popup/context menu
    popupMenu.addItem(POPUPMENU_INDEX_1_ABOUT, "about");
    addMouseListener(this, true);
    //[/Constructor]
}

SpectronAnalyzerComponent::~SpectronAnalyzerComponent() {
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
    SpectronParameters::getSingletonInstance().removeListener(this);
    DBG("SpectronAnalyzerComponent as parameter listener removed");
    LOG("SpectronAnalyzerComponent as parameter listener removed");
    parameters = NULL;
    //[/Destructor]
}

//==============================================================================
void SpectronAnalyzerComponent::paint(Graphics &g) {
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(Colours::black);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SpectronAnalyzerComponent::resized() {
    comboBoxResolution->setBounds((128) + 0, (8) + 32, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    spectralviewport->setBounds(256, 8, 528, 344);
    labelResolution->setBounds((8) + 0, (8) + 32, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    comboBoxTransformation->setBounds(128, 8, 120, 24);
    labelTransformation->setBounds(8, 8, 120, 24);
    comboBoxWindowing->setBounds((128) + 0, (8) + 64, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    labelWindowing->setBounds((8) + 0, (8) + 64, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    comboBoxWavelet->setBounds((128) + 0, (8) + 96, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    labelWavelet->setBounds((8) + 0, (8) + 96, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    comboBoxWaveletPaketBasis->setBounds((128) + 0, (8) + 128, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    labelWaveletPaketBasis->setBounds((8) + 0, (8) + 128, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    comboBoxSignalgenerator->setBounds((128) + 0, (8) + 288, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    labelSignalgenerator->setBounds((8) + 0, (8) + 288, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    comboBoxSignalquelle->setBounds((128) + 0, (8) + 160, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    labelSignalquelle->setBounds((8) + 0, (8) + 160, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    labelGeneratorfrequenz->setBounds((8) + 0, (8) + 320, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    sliderGeneratorFrequenz->setBounds((128) + 0, (8) + 320, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    labelLogF->setBounds((8) + 0, (8) + 192, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    labelLogA->setBounds((8) + 0, (8) + 224, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    comboBoxLogF->setBounds((128) + 0, (8) + 192, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    comboBoxLogA->setBounds((128) + 0, (8) + 224, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    labelColorMode->setBounds((8) + 1, (8) + 256, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    comboBoxColorMode->setBounds((128) + 1, (8) + 256, roundFloatToInt((120) * 1.0000f), roundFloatToInt((24) * 1.0000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SpectronAnalyzerComponent::comboBoxChanged(ComboBox *comboBoxThatHasChanged) {
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxResolution) {
        //[UserComboBoxCode_comboBoxResolution] -- add your combo box handling code here..
        parameters->setParameter(SpectronParameters::PARAMETER_INDEX_Resolution, comboBoxResolution->getText().getIntValue());
        //[/UserComboBoxCode_comboBoxResolution]
    } else if (comboBoxThatHasChanged == comboBoxTransformation) {
        //[UserComboBoxCode_comboBoxTransformation] -- add your combo box handling code here..
        parameters->setParameter(SpectronParameters::PARAMETER_INDEX_Transformation, comboBoxTransformation->getSelectedId());
        //[/UserComboBoxCode_comboBoxTransformation]
    } else if (comboBoxThatHasChanged == comboBoxWindowing) {
        //[UserComboBoxCode_comboBoxWindowing] -- add your combo box handling code here..
        parameters->setParameter(SpectronParameters::PARAMETER_INDEX_Windowing, comboBoxWindowing->getSelectedId());
        //[/UserComboBoxCode_comboBoxWindowing]
    } else if (comboBoxThatHasChanged == comboBoxWavelet) {
        //[UserComboBoxCode_comboBoxWavelet] -- add your combo box handling code here..
        parameters->setParameter(SpectronParameters::PARAMETER_INDEX_Wavelet, comboBoxWavelet->getSelectedId());
        //[/UserComboBoxCode_comboBoxWavelet]
    } else if (comboBoxThatHasChanged == comboBoxWaveletPaketBasis) {
        //[UserComboBoxCode_comboBoxWaveletPaketBasis] -- add your combo box handling code here..
        parameters->setParameter(SpectronParameters::PARAMETER_INDEX_WaveletPacketBase, comboBoxWaveletPaketBasis->getSelectedId());
        //[/UserComboBoxCode_comboBoxWaveletPaketBasis]
    } else if (comboBoxThatHasChanged == comboBoxSignalgenerator) {
        //[UserComboBoxCode_comboBoxSignalgenerator] -- add your combo box handling code here..
        parameters->setParameter(SpectronParameters::PARAMETER_INDEX_Generator, comboBoxSignalgenerator->getSelectedId());
        //[/UserComboBoxCode_comboBoxSignalgenerator]
    } else if (comboBoxThatHasChanged == comboBoxSignalquelle) {
        //[UserComboBoxCode_comboBoxSignalquelle] -- add your combo box handling code here..
        parameters->setParameter(SpectronParameters::PARAMETER_INDEX_Routing, comboBoxSignalquelle->getSelectedId());
        //[/UserComboBoxCode_comboBoxSignalquelle]
    } else if (comboBoxThatHasChanged == comboBoxLogF) {
        //[UserComboBoxCode_comboBoxLogF] -- add your combo box handling code here..
        parameters->setParameter(SpectronParameters::PARAMETER_INDEX_LogFrequency, comboBoxLogF->getSelectedId());
        //[/UserComboBoxCode_comboBoxLogF]
    } else if (comboBoxThatHasChanged == comboBoxLogA) {
        //[UserComboBoxCode_comboBoxLogA] -- add your combo box handling code here..
        parameters->setParameter(SpectronParameters::PARAMETER_INDEX_LogMagnitude, comboBoxLogA->getSelectedId());
        //[/UserComboBoxCode_comboBoxLogA]
    } else if (comboBoxThatHasChanged == comboBoxColorMode) {
        //[UserComboBoxCode_comboBoxColorMode] -- add your combo box handling code here..
        parameters->setParameter(SpectronParameters::PARAMETER_INDEX_ColorMode, comboBoxColorMode->getSelectedId());
        //[/UserComboBoxCode_comboBoxColorMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void SpectronAnalyzerComponent::sliderValueChanged(Slider *sliderThatWasMoved) {
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderGeneratorFrequenz) {
        //[UserSliderCode_sliderGeneratorFrequenz] -- add your slider handling code here..
        parameters->setParameter(SpectronParameters::PARAMETER_INDEX_GeneratorFrequency, sliderGeneratorFrequenz->getValue());
        //[/UserSliderCode_sliderGeneratorFrequenz]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SpectronAnalyzerComponent::visibilityChanged() {
    //[UserCode_visibilityChanged] -- Add your code here...
    //[/UserCode_visibilityChanged]
}

void SpectronAnalyzerComponent::parentSizeChanged() {
    //[UserCode_parentSizeChanged] -- Add your code here...
    //[/UserCode_parentSizeChanged]
}

void SpectronAnalyzerComponent::broughtToFront() {
    //[UserCode_broughtToFront] -- Add your code here...
    //[/UserCode_broughtToFront]
}

void SpectronAnalyzerComponent::childrenChanged() {
    //[UserCode_childrenChanged] -- Add your code here...
    //[/UserCode_childrenChanged]
}

void SpectronAnalyzerComponent::enablementChanged() {
    //[UserCode_enablementChanged] -- Add your code here...
    //[/UserCode_enablementChanged]
}

void SpectronAnalyzerComponent::mouseMove(const MouseEvent &e) {
    //[UserCode_mouseMove] -- Add your code here...
    //[/UserCode_mouseMove]
}

void SpectronAnalyzerComponent::mouseDown(const MouseEvent &e) {
    //[UserCode_mouseDown] -- Add your code here...
    if ((e.mouseWasClicked()) && (e.mods.isRightButtonDown())) {
        popupMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(this), [this](int index) {
            if (index == POPUPMENU_INDEX_1_ABOUT) {
                juce::AlertWindow::AlertIconType icon = AlertWindow::InfoIcon;
                juce::String message;
                message += ("Written by Johannes Troppacher (c)2011\n");
                message += ("\n");
                message += ("Framework 'JUCE' by Raw Material Software\n");
                message += ("VST-Interface 'VST SDK 2.4 rev2' by Steinberg\n");
                message += ("FFT-Library 'FFTW' by MIT (Matteo Frigo and Steven G. Johnson)\n");
                message += ("Wavelet-Library 'wave++' by Ryerson Computrational Signal Analysis Group");
                message += (" (S. E. Ferrando, L. A. Kolasa and N. Kovacevic)");
                AlertWindow::showOkCancelBox(icon, "About Spectron", message, "OK", "", this, nullptr);
            }
        });
    }
    //[/UserCode_mouseDown]
}

void SpectronAnalyzerComponent::mouseWheelMove(const MouseEvent &e, float wheelIncrementX, float wheelIncrementY) {
    //[UserCode_mouseWheelMove] -- Add your code here...
    //[/UserCode_mouseWheelMove]
}


//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void SpectronAnalyzerComponent::fillComboBoxes() {
    comboBoxResolution->addItem("256", SpectronParameters::RESOLUTION_256);
    comboBoxResolution->addItem("512", SpectronParameters::RESOLUTION_512);
    comboBoxResolution->addItem("1024", SpectronParameters::RESOLUTION_1024);
    comboBoxResolution->addItem("2048", SpectronParameters::RESOLUTION_2048);
    comboBoxResolution->addItem("4096", SpectronParameters::RESOLUTION_4096);
    comboBoxResolution->addItem("8192", SpectronParameters::RESOLUTION_8192);
    comboBoxResolution->addItem("16384", SpectronParameters::RESOLUTION_16384);
    comboBoxResolution->addItem("32768", SpectronParameters::RESOLUTION_32768);
    comboBoxResolution->addItem("65536", SpectronParameters::RESOLUTION_65536);

    comboBoxTransformation->addItem("FFT", SpectronParameters::TRANSFORM_FFT);
    comboBoxTransformation->addItem("FWT", SpectronParameters::TRANSFORM_FWT);
    comboBoxTransformation->addItem("WPT", SpectronParameters::TRANSFORM_FWPT);
    comboBoxTransformation->addItem("WPT BestBase", SpectronParameters::TRANSFORM_FWPT_BB);
    comboBoxTransformation->addItem("Off", SpectronParameters::TRANSFORM_OFF);

    comboBoxWindowing->addItem("Barlett", SpectronParameters::WINDOWING_BARTLETT);
    comboBoxWindowing->addItem("Blackman", SpectronParameters::WINDOWING_BLACKMAN);
    comboBoxWindowing->addItem("Blackman-Harris", SpectronParameters::WINDOWING_BLACKMAN_HARRIS);
    comboBoxWindowing->addItem("Hamming", SpectronParameters::WINDOWING_HAMMING);
    comboBoxWindowing->addItem("Hann", SpectronParameters::WINDOWING_HANN);
    comboBoxWindowing->addItem("Welch", SpectronParameters::WINDOWING_WELCH);
    comboBoxWindowing->addItem("Parzen", SpectronParameters::WINDOWING_PARZEN);
    comboBoxWindowing->addItem("Rectangular", SpectronParameters::WINDOWING_RECTANGULAR);

    comboBoxWavelet->addItem("Haar (2)", SpectronParameters::WAVELET_HAAR);
    comboBoxWavelet->addItem("Daubechies (4)", SpectronParameters::WAVELET_DAUBECHIES_04);
    comboBoxWavelet->addItem("Daubechies (6)", SpectronParameters::WAVELET_DAUBECHIES_06);
    comboBoxWavelet->addItem("Daubechies (8)", SpectronParameters::WAVELET_DAUBECHIES_08);
    comboBoxWavelet->addItem("Daubechies (10)", SpectronParameters::WAVELET_DAUBECHIES_10);
    comboBoxWavelet->addItem("Daubechies (12)", SpectronParameters::WAVELET_DAUBECHIES_12);
    comboBoxWavelet->addItem("Daubechies (14)", SpectronParameters::WAVELET_DAUBECHIES_14);
    comboBoxWavelet->addItem("Daubechies (16)", SpectronParameters::WAVELET_DAUBECHIES_16);
    comboBoxWavelet->addItem("Daubechies (18)", SpectronParameters::WAVELET_DAUBECHIES_18);
    comboBoxWavelet->addItem("Daubechies (20)", SpectronParameters::WAVELET_DAUBECHIES_20);
    comboBoxWavelet->addItem("Coifman (6)", SpectronParameters::WAVELET_COIFMAN_06);
    comboBoxWavelet->addItem("Coifman (12)", SpectronParameters::WAVELET_COIFMAN_12);
    comboBoxWavelet->addItem("Coifman (18)", SpectronParameters::WAVELET_COIFMAN_18);
    comboBoxWavelet->addItem("Coifman (24)", SpectronParameters::WAVELET_COIFMAN_24);
    comboBoxWavelet->addItem("Coifman (30)", SpectronParameters::WAVELET_COIFMAN_30);
    comboBoxWavelet->addItem("Beylkin (18)", SpectronParameters::WAVELET_BEYLKIN_18);
    comboBoxWavelet->addItem("Vaidyanathan (18)", SpectronParameters::WAVELET_VAIDYANATHAN_18);

    comboBoxWaveletPaketBasis->addItem("freq/time x1", SpectronParameters::RESOLUTION_RATIO_Equal);
    comboBoxWaveletPaketBasis->addItem("freq x2", SpectronParameters::RESOLUTION_RATIO_FreqX2);
    comboBoxWaveletPaketBasis->addItem("freq x4", SpectronParameters::RESOLUTION_RATIO_FreqX4);
    comboBoxWaveletPaketBasis->addItem("time x2", SpectronParameters::RESOLUTION_RATIO_TimeX2);
    comboBoxWaveletPaketBasis->addItem("time x4", SpectronParameters::RESOLUTION_RATIO_TimeX4);

    comboBoxSignalgenerator->addItem("Sine", SpectronParameters::GENERATOR_SINE);
    comboBoxSignalgenerator->addItem("Triangle", SpectronParameters::GENERATOR_TRIANGLE);
    comboBoxSignalgenerator->addItem("Sawtooth", SpectronParameters::GENERATOR_RAMP);
    comboBoxSignalgenerator->addItem("Rectangle", SpectronParameters::GENERATOR_SQUARE);
    comboBoxSignalgenerator->addItem("Noise", SpectronParameters::GENERATOR_NOISE);

    comboBoxSignalquelle->addItem("Mid", SpectronParameters::ROUTING_MID);
    comboBoxSignalquelle->addItem("Side", SpectronParameters::ROUTING_SIDE);
    comboBoxSignalquelle->addItem("Left", SpectronParameters::ROUTING_L);
    comboBoxSignalquelle->addItem("Right", SpectronParameters::ROUTING_R);
    comboBoxSignalquelle->addItem("Oscillator", SpectronParameters::ROUTING_GENERATOR);

    comboBoxLogF->addItem("linear", SpectronParameters::PLOT_AXIS_LINEAR);
    comboBoxLogF->addItem("logarithmic", SpectronParameters::PLOT_AXIS_LOGARITHMIC);

    comboBoxLogA->addItem("linear", SpectronParameters::PLOT_AXIS_LINEAR);
    comboBoxLogA->addItem("logarithmic", SpectronParameters::PLOT_AXIS_LOGARITHMIC);

    comboBoxColorMode->addItem("Blue", SpectronParameters::COLORMODE_BLUE);
    comboBoxColorMode->addItem("Green", SpectronParameters::COLORMODE_GREEN);
    comboBoxColorMode->addItem("Fire", SpectronParameters::COLORMODE_FIRE);
    comboBoxColorMode->addItem("Rainbow", SpectronParameters::COLORMODE_RAINBOW);
}

void SpectronAnalyzerComponent::valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier & /*property*/) {
    const ScopedLock myScopedLock(criticalSection);

    updateComboBox(SpectronParameters::PARAMETER_COLORMODE, comboBoxColorMode, treeWhosePropertyHasChanged);
    updateComboBox(SpectronParameters::PARAMETER_GENERATOR, comboBoxSignalgenerator, treeWhosePropertyHasChanged);
    updateComboBox(SpectronParameters::PARAMETER_LOGFREQUENCY, comboBoxLogF, treeWhosePropertyHasChanged);
    updateComboBox(SpectronParameters::PARAMETER_LOGMAGNITUDE, comboBoxLogA, treeWhosePropertyHasChanged);
    updateComboBox(SpectronParameters::PARAMETER_RESOLUTION, comboBoxResolution, treeWhosePropertyHasChanged);
    updateComboBox(SpectronParameters::PARAMETER_ROUTING, comboBoxSignalquelle, treeWhosePropertyHasChanged);
    updateComboBox(SpectronParameters::PARAMETER_TRANSFORMATION, comboBoxTransformation, treeWhosePropertyHasChanged);
    updateComboBox(SpectronParameters::PARAMETER_WAVELET, comboBoxWavelet, treeWhosePropertyHasChanged);
    updateComboBox(SpectronParameters::PARAMETER_WAVELETPACKETBASE, comboBoxWaveletPaketBasis, treeWhosePropertyHasChanged);
    updateComboBox(SpectronParameters::PARAMETER_WINDOWING, comboBoxWindowing, treeWhosePropertyHasChanged);
    updateSlider(SpectronParameters::PARAMETER_GENERATORFREQUENCY, sliderGeneratorFrequenz, treeWhosePropertyHasChanged);
}

//This method updates a combobox-index within an parameter-change-event
void SpectronAnalyzerComponent::updateComboBox(
        const juce::String &parameterName,
        juce::ComboBox *comboBox,
        const ValueTree &treeWhosePropertyHasChanged) {
    juce::String changedParameterName = treeWhosePropertyHasChanged.getType().toString();
    juce::var changedParameterValue = treeWhosePropertyHasChanged.getProperty(SpectronParameters::PROPERTY_VALUE);

    if (!changedParameterName.equalsIgnoreCase(parameterName)) {
        return;
    }

    jassert(comboBox);
    comboBox->setSelectedId((int) changedParameterValue, true);
}

//This method updates a slider-value within an parameter-change-event
void SpectronAnalyzerComponent::updateSlider(
        const juce::String &parameterName,
        juce::Slider *slider,
        const ValueTree &treeWhosePropertyHasChanged) {
    juce::String changedParameterName = treeWhosePropertyHasChanged.getType().toString();
    juce::var changedParameterValue = treeWhosePropertyHasChanged.getProperty(SpectronParameters::PROPERTY_VALUE);

    if (!changedParameterName.equalsIgnoreCase(parameterName)) {
        return;
    }

    jassert(slider);
    slider->setValue(changedParameterValue, dontSendNotification);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SpectronAnalyzerComponent"
                 componentName="SpectronAnalyzerComponent" parentClasses="public Component, public juce::ValueTree::Listener"
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
            hscroll="1" scrollbarThickness="10" contentType="1" jucerFile="SpectronDrawer.h"
            contentClass="SpectronDrawer" constructorParams=""/>
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
