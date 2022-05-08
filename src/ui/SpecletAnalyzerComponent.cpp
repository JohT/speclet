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
void SpecletTooltipWindowLookAndFeel::drawTooltip(juce::Graphics &g, const juce::String &text, int width, int height) {
    g.fillAll(juce::Colours::black.withAlpha(0.8F));
    g.setColour(juce::Colours::white);
    g.drawFittedText(text, 0, 0, width, height, juce::Justification::left, 1);
}
//[/MiscUserDefs]

//==============================================================================
SpecletAnalyzerComponent::SpecletAnalyzerComponent(SpecletParameters &parametersToAttach)
    : Component("SpecletAnalyzerComponent"),
      parameters(parametersToAttach),
      spectralviewport(new juce::Viewport("spectralviewport")),
      tooltipWindow(new juce::TooltipWindow(this)),

      comboBoxResolution(createComboBox("comboBoxResolution", SpecletParameters::PARAMETER_RESOLUTION)),
      labelResolution(createLabel("labelResolution", "Resolution")),
      comboBoxTransformation(createComboBox("comboBoxTransformation", SpecletParameters::PARAMETER_TRANSFORMATION)),
      labelTransformation(createLabel("labelTransformation", "Transformation")),
      comboBoxWindowing(createComboBox("comboBoxWindowing", SpecletParameters::PARAMETER_WINDOWING)),
      labelWindowing(createLabel("labelWindowing", "Window Function")),
      comboBoxWavelet(createComboBox("comboBoxWavelet", SpecletParameters::PARAMETER_WAVELET)),
      labelWavelet(createLabel("labelWavelet", "Wavelet")),
      comboBoxWaveletPacketBasis(createComboBox("comboBoxWaveletPacketBasis", SpecletParameters::PARAMETER_WAVELETPACKETBASIS)),
      labelWaveletPacketBasis(createLabel("labelWaveletPacketBasis", "Wavelet Packet Basis")),
      comboBoxSignalGenerator(createComboBox("comboBoxSignalgenerator", SpecletParameters::PARAMETER_GENERATOR)),
      labelSignalGenerator(createLabel("labelSignalGenerator", "Oscillator")),
      comboBoxRouting(createComboBox("comboBoxRouting", SpecletParameters::PARAMETER_ROUTING)),
      labelRouting(createLabel("labelRouting", "Audio Source")),
      labelSignalGeneratorFrequency(createLabel("labelGeneratorFrequency", "Oscillator Frequency")),
      sliderSignalGeneratorFrequency(createSlider("sliderGeneratorFrequency", SpecletParameters::PARAMETER_GENERATORFREQUENCY)),
      labelLogF(createLabel("labelLogF", "Frequency Scale")),
      labelLogA(createLabel("labelLogA", "Magnitude Scale")),
      comboBoxLogF(createComboBox("comboBoxLogF", SpecletParameters::PARAMETER_LOGFREQUENCY)),
      comboBoxLogA(createComboBox("comboBoxLogA", SpecletParameters::PARAMETER_LOGMAGNITUDE)),
      labelColorMode(createLabel("labelColorMode", "Color Mode")),
      comboBoxColorMode(createComboBox("comboBoxColorMode", SpecletParameters::PARAMETER_COLORMODE)),

      resolutionParameterAttachment(new ComboBoxAttachment(parametersToAttach.getParameters(), SpecletParameters::PARAMETER_RESOLUTION, *comboBoxResolution)),
      transformationParameterAttachment(new ComboBoxAttachment(parametersToAttach.getParameters(), SpecletParameters::PARAMETER_TRANSFORMATION, *comboBoxTransformation)),
      windowingParameterAttachment(new ComboBoxAttachment(parametersToAttach.getParameters(), SpecletParameters::PARAMETER_WINDOWING, *comboBoxWindowing)),
      waveletParameterAttachment(new ComboBoxAttachment(parametersToAttach.getParameters(), SpecletParameters::PARAMETER_WAVELET, *comboBoxWavelet)),
      waveletPacketBasisParameterAttachment(new ComboBoxAttachment(parametersToAttach.getParameters(), SpecletParameters::PARAMETER_WAVELETPACKETBASIS, *comboBoxWaveletPacketBasis)),
      signalGeneratorParameterAttachment(new ComboBoxAttachment(parametersToAttach.getParameters(), SpecletParameters::PARAMETER_GENERATOR, *comboBoxSignalGenerator)),
      routingParameterAttachment(new ComboBoxAttachment(parametersToAttach.getParameters(), SpecletParameters::PARAMETER_ROUTING, *comboBoxRouting)),
      logFParameterAttachment(new ComboBoxAttachment(parametersToAttach.getParameters(), SpecletParameters::PARAMETER_LOGFREQUENCY, *comboBoxLogF)),
      logAParameterAttachment(new ComboBoxAttachment(parametersToAttach.getParameters(), SpecletParameters::PARAMETER_LOGMAGNITUDE, *comboBoxLogA)),
      colorModeParameterAttachment(new ComboBoxAttachment(parametersToAttach.getParameters(), SpecletParameters::PARAMETER_COLORMODE, *comboBoxColorMode)),
      signalGeneratorFrequencyParameterAttachment(new SliderAttachment(parametersToAttach.getParameters(), SpecletParameters::PARAMETER_GENERATORFREQUENCY, *sliderSignalGeneratorFrequency)) {

    for (auto *component : getComponents()) {
        addAndMakeVisible(component);
    }
    spectralviewport->setScrollBarsShown(false, true);
    spectralviewport->setScrollBarThickness(10);

    comboBoxResolution->setTooltip(
            "Sets the block size for the analysis.\n"
            "Higher values lead to better frequency resolution, lower time resolution and more cpu consumption. "
            "For efficient calculation, all values are a power of 2.");
    comboBoxTransformation->setTooltip(
            "Transformation methods:\n\n"
            " - FFT: Fast Fourier Transform\n"
            " - FWT: Fast Wavelet Transform\n"
            " - WPT: Wavelet Packet Transform\n"
            " - WPT BestBasis: WPT with best basis\n");
    comboBoxWindowing->setTooltip(
            "Window functions are like fades at the beginning and end of the block of input samples.\n"
            "They reduce 'leakage' in the frequency spectrum. "
            "Choose a sine input and use different windows to see the impact. "
            "The rectangular window shows how the spectrum is affected when there is no smoothing.\n");
    comboBoxWavelet->setTooltip(
            "Wavelets are represented by their high- and low-pass filter coefficients.\n"
            "The number in the parentheses is the number of coefficients.\n"
            "More coefficients lead to more precise results, less aliasing and higher cpu consuption.\n");
    comboBoxWaveletPacketBasis->setTooltip(
            "The wavelet packet transform (WPT) also uses the output of the high pass filter (details) and splits it up further.\n"
            "This results in evenly/linear spaced frequency bands in contrast to the dyadic wavelet transform.\n"
            "More levels lead to finer frequency resolution and less time resolution.");
    comboBoxSignalGenerator->setTooltip("Sets the waveform of the build-in test signal generator/oscillator.");
    comboBoxRouting->setTooltip(
            "Sets the input audio source to\n"
            "either one of the input channels\n"
            "or the build-in signal generator/oscillator.");
    sliderSignalGeneratorFrequency->setTooltip(
            "Sets the frequency of the\n"
            "build-in signal generator/oscillator.");
    comboBoxLogF->setTooltip("Set the frequency scale to linear or logarithmic.");
    comboBoxLogA->setTooltip("Set the magnitude scale to linear or logarithmic.");
    comboBoxColorMode->setTooltip("Sets the colors that reflect the magnitude of the signal.");

    tooltipWindow->setLookAndFeel(&tooltipWindowLookAndFeel);
    //[UserPreSize]
    //[/UserPreSize]

    setSize(800, 360);

    //[Constructor] You can add your own custom stuff here..

    transformationChanged(parameters.getTransformation());
    routingChanged(parameters.getRouting());

    //registers itself as listener for parameter-changes
    parameters.addListener(this);
    DBG("SpecletAnalyzerComponent as parameter listener added");
    //adds spectrum drawing component to the scrollable view
    spectralviewport->setViewedComponent(specletDrawer);
    parameters.addListener(specletDrawer);

    //adds entries to the popup/context menu
    popupMenu.addItem(POPUPMENU_INDEX_1_ABOUT, "about");
    addMouseListener(this, true);
    //[/Constructor]
}

SpecletAnalyzerComponent::~SpecletAnalyzerComponent() {
    //[Destructor_pre]. You can add your own custom destruction code here..
    parameters.removeListener(specletDrawer);
    parameters.removeListener(this);
    
    deleteAndZero(resolutionParameterAttachment);
    deleteAndZero(transformationParameterAttachment);
    deleteAndZero(windowingParameterAttachment);
    deleteAndZero(waveletParameterAttachment);
    deleteAndZero(waveletPacketBasisParameterAttachment);
    deleteAndZero(signalGeneratorParameterAttachment);
    deleteAndZero(routingParameterAttachment);
    deleteAndZero(logFParameterAttachment);
    deleteAndZero(logAParameterAttachment);
    deleteAndZero(colorModeParameterAttachment);
    deleteAndZero(signalGeneratorFrequencyParameterAttachment);

    //[/Destructor_pre]
    deleteAndZero(comboBoxResolution);
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
    deleteAndZero(tooltipWindow);
    deleteAndZero(specletDrawer);
    deleteAndZero(spectralviewport);

    //[Destructor]. You can add your own custom destruction code here..
    //unregisters itself as listener for parameter-changes
    parameters.removeListener(this);
    DBG("SpecletAnalyzerComponent as parameter listener removed");
    //[/Destructor]
}

auto SpecletAnalyzerComponent::createComboBox(const juce::String &componentName, const juce::String &parameterName) -> juce::ComboBox * {
    auto comboBox = new juce::ComboBox(componentName);
    comboBox->addItemList(parameters.getParameterList(parameterName), 1);
    comboBox->setSelectedItemIndex(parameters.getParameterAsSelection(parameterName), juce::dontSendNotification);
    comboBox->setEditableText(false);
    comboBox->setJustificationType(juce::Justification::centredLeft);
    comboBox->setTextWhenNothingSelected(juce::String());
    comboBox->setTextWhenNoChoicesAvailable(juce::String());
    return comboBox;
}

auto SpecletAnalyzerComponent::createLabel(const juce::String &componentName, const juce::String &labelText) -> juce::Label * {
    auto label = new juce::Label(componentName, labelText);
    label->setFont(juce::Font(15.0000f, juce::Font::plain));
    label->setJustificationType(juce::Justification::centredLeft);
    label->setEditable(false, false, false);
    label->setColour(juce::Label::backgroundColourId, juce::Colour(0x30007bfc));
    label->setColour(juce::Label::textColourId, juce::Colours::white);
    label->setColour(juce::Label::outlineColourId, juce::Colour(0xff0082f7));
    label->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x0));
    return label;
}

auto SpecletAnalyzerComponent::createSlider(const juce::String &componentName, const juce::String &parameterName) -> juce::Slider * {
    auto slider = new juce::Slider(componentName);
    slider->setValue(parameters.getParameter(parameterName), juce::dontSendNotification);
    slider->setExplicitFocusOrder(7);
    slider->setRange(10, 22000, 1);
    slider->setSliderStyle(juce::Slider::LinearHorizontal);
    slider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 25);
    return slider;
}

auto SpecletAnalyzerComponent::getComponents() -> std::vector<juce::Component *> {
    return {
            spectralviewport,
            tooltipWindow,
            comboBoxResolution,
            labelResolution,
            comboBoxTransformation,
            labelTransformation,
            comboBoxWindowing,
            labelWindowing,
            comboBoxWavelet,
            labelWavelet,
            comboBoxWaveletPacketBasis,
            labelWaveletPacketBasis,
            comboBoxSignalGenerator,
            labelSignalGenerator,
            comboBoxRouting,
            labelRouting,
            labelSignalGeneratorFrequency,
            sliderSignalGeneratorFrequency,
            labelLogF,
            labelLogA,
            comboBoxLogF,
            comboBoxLogA,
            labelColorMode,
            comboBoxColorMode,

    };
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
    //TODO (JohT) Had been generated by "Projucer". Could be adapted to be resizeable.
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
                        "   (S. E. Ferrando, L. A. Kolasa and N. Kovacevic)\n"
                        "- std::span for C++11 by Tristan Brindle (2019)\n";

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

void SpecletAnalyzerComponent::parameterChanged(const juce::String &parameterID, float newValue) {
    const juce::ScopedLock myScopedLock(criticalSection);
    auto newIndex = static_cast<int>(newValue) + 1;// +1 because of 0 is not a valid combo box selection index
    if (parameterID == SpecletParameters::PARAMETER_TRANSFORMATION) {
        transformationChanged(newIndex);
    }
    if (parameterID == SpecletParameters::PARAMETER_ROUTING) {
        routingChanged(newIndex);
    }
}

void SpecletAnalyzerComponent::transformationChanged(int selectedOption) {
    if (selectedOption == enumOptionIntValue(TransformationParameters::Type::BYPASS)) {
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

    if (selectedOption == enumOptionIntValue(TransformationParameters::Type::FAST_WAVELET_TRANSFORM) ||
        selectedOption == enumOptionIntValue(TransformationParameters::Type::FAST_WAVELET_PACKET_BEST_BASIS_TRANSFORM)) {
        //Disable Wavelet Tree/Packet related control for the dyadic Fast Wavelet Transform.
        comboBoxWavelet->setEnabled(true);
        labelWavelet->setEnabled(true);
        comboBoxWaveletPacketBasis->setEnabled(false);
        labelWaveletPacketBasis->setEnabled(false);
    } else if (selectedOption == enumOptionIntValue(TransformationParameters::Type::FAST_WAVELET_PACKET_TRANSFORM)) {
        //Enable all controls for the Fast Wavelet Packet Transform.
        comboBoxWavelet->setEnabled(true);
        labelWavelet->setEnabled(true);
        comboBoxWaveletPacketBasis->setEnabled(true);
        labelWaveletPacketBasis->setEnabled(true);
    } else {
        //Disable all wavelet related controls for FFT or otherwise (e.g. when analyzer is bypassed)
        comboBoxWavelet->setEnabled(false);
        labelWavelet->setEnabled(false);
        comboBoxWaveletPacketBasis->setEnabled(false);
        labelWaveletPacketBasis->setEnabled(false);
    }
}

void SpecletAnalyzerComponent::routingChanged(int selectedOption) {
    if (selectedOption == enumOptionIntValue(SpecletParameters::OptionsRouting::ROUTING_GENERATOR)) {
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
auto SpecletAnalyzerComponent::enumOptionIntValue(const _Tp &enumType) const -> int {
    auto enumValue = static_cast<typename std::underlying_type_t<_Tp>>(enumType);
    return static_cast<int>(enumValue);
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
