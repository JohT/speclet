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
#include "ColourGradients.h"
#include "SpecletDrawer.h"
#include "juce_core/juce_core.h"

//[/Headers]

class SpecletTooltipWindowLookAndFeel : public juce::LookAndFeel_V4 {
public:
    SpecletTooltipWindowLookAndFeel() = default;
    ~SpecletTooltipWindowLookAndFeel() override = default;
    void drawTooltip(juce::Graphics &g, const juce::String &text, int width, int height) override;
};

//==============================================================================
/**
                                                                    //[Comments]
	 An auto-generated component, created by the Jucer.

	 Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SpecletAnalyzerComponent : public juce::Component,
                                 public juce::AudioProcessorValueTreeState::Listener,
                                 public juce::SettableTooltipClient {
public:
    //==============================================================================
    explicit SpecletAnalyzerComponent(SpecletParameters & parametersToAttach);
    ~SpecletAnalyzerComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    auto createComboBox(const juce::String& componentName, const juce::String& parameterName) -> juce::ComboBox *;
    auto createLabel(const juce::String& componentName, const juce::String& labelText) -> juce::Label *;
    auto createSlider(const juce::String& componentName, const juce::String& parameterName) -> juce::Slider *;
    auto getComponents() -> std::vector<juce::Component *>;
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;
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
        enum PopupMenuEntryIndices {
            POPUPMENU_INDEX_1_ABOUT = 1
        };

    SpecletParameters &parameters;
    juce::CriticalSection criticalSection;
    juce::PopupMenu popupMenu;

    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void transformationChanged(int selectedOption);
    void routingChanged(int selectedOption);

    template<class _Tp>
    auto enumOptionIntValue(const _Tp &enumType) const -> int;

    SpecletDrawer *specletDrawer = new SpecletDrawer(parameters.getLogFrequency(), parameters.getLogMagnitude(), ColourGradients::forIndex(parameters.getColorMode()));
    //[/UserVariables]

    //==============================================================================
    juce::Viewport *spectralviewport = nullptr;

    juce::TooltipWindow *tooltipWindow = nullptr;
    SpecletTooltipWindowLookAndFeel tooltipWindowLookAndFeel;

    juce::ComboBox *comboBoxResolution = nullptr;
    juce::Label *labelResolution = nullptr;
    juce::ComboBox *comboBoxTransformation = nullptr;
    juce::Label *labelTransformation = nullptr;
    juce::ComboBox *comboBoxWindowing = nullptr;
    juce::Label *labelWindowing = nullptr;
    juce::ComboBox *comboBoxWavelet = nullptr;
    juce::Label *labelWavelet = nullptr;
    juce::ComboBox *comboBoxWaveletPacketBasis = nullptr;
    juce::Label *labelWaveletPacketBasis = nullptr;
    juce::ComboBox *comboBoxSignalGenerator = nullptr;
    juce::Label *labelSignalGenerator = nullptr;
    juce::ComboBox *comboBoxRouting = nullptr;
    juce::Label *labelRouting = nullptr;
    juce::Label *labelSignalGeneratorFrequency = nullptr;
    juce::Slider *sliderSignalGeneratorFrequency = nullptr;
    juce::Label *labelLogF = nullptr;
    juce::Label *labelLogA = nullptr;
    juce::ComboBox *comboBoxLogF = nullptr;
    juce::ComboBox *comboBoxLogA = nullptr;
    juce::Label *labelColorMode = nullptr;
    juce::ComboBox *comboBoxColorMode = nullptr;

    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    ComboBoxAttachment * resolutionParameterAttachment;
    ComboBoxAttachment * transformationParameterAttachment;
    ComboBoxAttachment * windowingParameterAttachment;
    ComboBoxAttachment * waveletParameterAttachment;
    ComboBoxAttachment * waveletPacketBasisParameterAttachment;
    ComboBoxAttachment * signalGeneratorParameterAttachment;
    ComboBoxAttachment * routingParameterAttachment;
    ComboBoxAttachment * logFParameterAttachment;
    ComboBoxAttachment * logAParameterAttachment;
    ComboBoxAttachment * colorModeParameterAttachment;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    SliderAttachment * signalGeneratorFrequencyParameterAttachment;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    SpecletAnalyzerComponent(const SpecletAnalyzerComponent &) = delete;
    auto operator=(const SpecletAnalyzerComponent &) -> SpecletAnalyzerComponent & = delete;
};