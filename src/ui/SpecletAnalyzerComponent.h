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
#include "../plugin/SpecletParameters.h"
#include "juce_core/juce_core.h"

//[/Headers]


//==============================================================================
/**
                                                                    //[Comments]
	 An auto-generated component, created by the Jucer.

	 Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SpecletAnalyzerComponent : public juce::Component,
                                 public juce::ValueTree::Listener,
                                 public juce::ComboBox::Listener,
                                 public juce::Slider::Listener {
public:
    //==============================================================================
    SpecletAnalyzerComponent();
    ~SpecletAnalyzerComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) override;
    void sliderValueChanged(juce::Slider *sliderThatWasMoved) override;
    void visibilityChanged() override;
    void parentSizeChanged() override;
    void broughtToFront() override;
    void childrenChanged() override;
    void enablementChanged() override;
    void mouseMove(const juce::MouseEvent &e) override;
    void mouseDown(const juce::MouseEvent &e) override;
    void mouseWheelMove(const juce::MouseEvent &event, const juce::MouseWheelDetails &wheel) override;

    //==============================================================================
    juce_UseDebuggingNewOperator

private :
    //[UserVariables]   -- You can add your own custom variables in this section.
    enum PopupMenuEntryIndizes {
        POPUPMENU_INDEX_1_ABOUT = 1
    };
    
    SpecletParameters *parameters;
    juce::CriticalSection criticalSection;
    juce::PopupMenu popupMenu;

    void fillComboBoxes();
    void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override;
    void valueTreeChildrenChanged(juce::ValueTree & /*unused*/) {}
    void valueTreeParentChanged(juce::ValueTree & /*treeWhoseParentHasChanged*/) override {}

    void updateComboBox(const juce::String &parameterName, juce::ComboBox *comboBox, const juce::ValueTree &treeWhosePropertyHasChanged);
    static void updateSlider(const juce::String &parameterName, juce::Slider *slider, const juce::ValueTree &treeWhosePropertyHasChanged);
    void transformationChanged(auto selectedOption);
    
    template<class _Tp>
    auto enumOptionToFloat(const _Tp& enumType) const -> float;
    
    //[/UserVariables]

    //==============================================================================
    juce::ComboBox *comboBoxResolution;
    juce::Viewport *spectralviewport;
    juce::Label *labelResolution;
    juce::ComboBox *comboBoxTransformation;
    juce::Label *labelTransformation;
    juce::ComboBox *comboBoxWindowing;
    juce::Label *labelWindowing;
    juce::ComboBox *comboBoxWavelet;
    juce::Label *labelWavelet;
    juce::ComboBox *comboBoxWaveletPaketBasis;
    juce::Label *labelWaveletPaketBasis;
    juce::ComboBox *comboBoxSignalgenerator;
    juce::Label *labelSignalgenerator;
    juce::ComboBox *comboBoxSignalquelle;
    juce::Label *labelSignalquelle;
    juce::Label *labelGeneratorfrequenz;
    juce::Slider *sliderGeneratorFrequenz;
    juce::Label *labelLogF;
    juce::Label *labelLogA;
    juce::ComboBox *comboBoxLogF;
    juce::ComboBox *comboBoxLogA;
    juce::Label *labelColorMode;
    juce::ComboBox *comboBoxColorMode;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    SpecletAnalyzerComponent(const SpecletAnalyzerComponent &);
    auto operator=(const SpecletAnalyzerComponent &) -> const SpecletAnalyzerComponent &;
};