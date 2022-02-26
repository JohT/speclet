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
#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../dsp/transformations/Transformation.h"
#include "../plugin/SpectronParameters.h"
#include "JuceHeader.h"
#include "SpectronDrawer.h"

//[/Headers]


//==============================================================================
/**
                                                                    //[Comments]
	 An auto-generated component, created by the Jucer.

	 Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SpectronAnalyzerComponent : public Component,
                                  public juce::ValueTree::Listener,
                                  public juce::ComboBox::Listener,
                                  public juce::Slider::Listener {
public:
    //==============================================================================
    SpectronAnalyzerComponent();
    ~SpectronAnalyzerComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint(Graphics &g) override;
    void resized() override;
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    void sliderValueChanged(Slider *sliderThatWasMoved) override;
    void visibilityChanged() override;
    void parentSizeChanged() override;
    void broughtToFront() override;
    void childrenChanged() override;
    void enablementChanged() override;
    void mouseMove(const MouseEvent &e) override;
    void mouseDown(const MouseEvent &e) override;
    void mouseWheelMove(const MouseEvent &e, float wheelIncrementX, float wheelIncrementY);


    //==============================================================================
    juce_UseDebuggingNewOperator

            private :
        //[UserVariables]   -- You can add your own custom variables in this section.
        enum PopupMenuEntryIndizes {
            POPUPMENU_INDEX_1_ABOUT = 1
        };
    SpectronParameters *parameters;
    juce::CriticalSection criticalSection;
    juce::PopupMenu popupMenu;

    void fillComboBoxes();
    void valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier &property) override;
    void valueTreeChildrenChanged(ValueTree & /*unused*/) {}
    void valueTreeParentChanged(ValueTree & /*treeWhoseParentHasChanged*/) override {}

    void updateComboBox(const juce::String &parameterName, juce::ComboBox *comboBox, const ValueTree &treeWhosePropertyHasChanged);
    void updateSlider(const juce::String &parameterName, juce::Slider *slider, const ValueTree &treeWhosePropertyHasChanged);

    //[/UserVariables]

    //==============================================================================
    ComboBox *comboBoxResolution;
    Viewport *spectralviewport;
    Label *labelResolution;
    ComboBox *comboBoxTransformation;
    Label *labelTransformation;
    ComboBox *comboBoxWindowing;
    Label *labelWindowing;
    ComboBox *comboBoxWavelet;
    Label *labelWavelet;
    ComboBox *comboBoxWaveletPaketBasis;
    Label *labelWaveletPaketBasis;
    ComboBox *comboBoxSignalgenerator;
    Label *labelSignalgenerator;
    ComboBox *comboBoxSignalquelle;
    Label *labelSignalquelle;
    Label *labelGeneratorfrequenz;
    Slider *sliderGeneratorFrequenz;
    Label *labelLogF;
    Label *labelLogA;
    ComboBox *comboBoxLogF;
    ComboBox *comboBoxLogA;
    Label *labelColorMode;
    ComboBox *comboBoxColorMode;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    SpectronAnalyzerComponent(const SpectronAnalyzerComponent &);
    auto operator=(const SpectronAnalyzerComponent &) -> const SpectronAnalyzerComponent &;
};