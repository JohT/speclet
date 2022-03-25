/*
  ==============================================================================
  This file is part of the VST spectrum analyzer plugin "speclet" (working title)
  Copyright 2011 by Johannes Troppacher
	------------------------------------------------------------------------------
  This file may use parts of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.
	------------------------------------------------------------------------------
  This file may use parts of the fftw library
  Copyright 2003 Matteo Frigo, Copyright 2003 Massachusetts Institute of Technology
	------------------------------------------------------------------------------
  This file may use parts of the wave++ library
  Copyright 1999 by S. E. Ferrando, L. A. Kolasa and N. Kovacevic
  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>
#include <string>

class SpectronParameters {
    // --------------- constants --------------- //
public:
    //---------------------------------------------------------------------------
    //IMPORTANT: Since juce components like combo boxes can't handle index = 0,
    //           enums must not contain a zero value
    //           Only exception: PARAMETER_INDEX (since it has to do with VST SDK)
    //---------------------------------------------------------------------------

    enum Constants {
        TIMEOUT_WAIT_BEFORE_SET = 5000
    };
    enum Parameters {
        //Has to start with index=0. TOTAL_NUMBER_OF_PARAMS needs to be the last parameter.
        PARAMETER_INDEX_Routing = 0,
        PARAMETER_INDEX_Transformation,
        PARAMETER_INDEX_Resolution,
        PARAMETER_INDEX_WaveletPacketBase,
        PARAMETER_INDEX_Windowing,
        PARAMETER_INDEX_Wavelet,
        PARAMETER_INDEX_Generator,
        PARAMETER_INDEX_GeneratorFrequency,
        PARAMETER_INDEX_LogFrequency,
        PARAMETER_INDEX_LogMagnitude,
        PARAMETER_INDEX_ColorMode,

        TOTAL_NUMBER_OF_PARAMS
    };
    enum OptionsColormode {
        COLORMODE_BLUE = 1,
        COLORMODE_GREEN,
        COLORMODE_RAINBOW,
        COLORMODE_FIRE,

        COLORMODE_NumOptions,
        COLORMODE_DEFAULT = COLORMODE_BLUE
    };
    enum OptionsGenerator {
        GENERATOR_SINE = 1,
        GENERATOR_TRIANGLE,
        GENERATOR_RAMP,
        GENERATOR_SQUARE,
        GENERATOR_NOISE,

        GENERATOR_NumOptions,
        GENERATOR_DEFAULT = GENERATOR_SINE
    };
    enum OptionsPlotAxis {
        PLOT_AXIS_LINEAR = 1,
        PLOT_AXIS_LOGARITHMIC,

        PLOT_AXIS_NumOptions,
        PLOT_AXIS_DEFAULT = PLOT_AXIS_LOGARITHMIC
    };
    enum OptionsResolution {
        RESOLUTION_256 = 256,
        RESOLUTION_512 = 512,
        RESOLUTION_1024 = 1024,
        RESOLUTION_2048 = 2048,
        RESOLUTION_4096 = 4096,
        RESOLUTION_8192 = 8192,
        RESOLUTION_16384 = 16384,
        RESOLUTION_32768 = 32768,
        RESOLUTION_65536 = 65536,

        RESOLUTION_MAX = RESOLUTION_65536,
        RESOLUTION_DEFAULT = RESOLUTION_4096
    };
    enum OptionsResolutionRatio {
        RESOLUTION_RATIO_Equal = 99,
        RESOLUTION_RATIO_TimeX4 = -2,
        RESOLUTION_RATIO_TimeX2 = -1,
        RESOLUTION_RATIO_FreqX2 = 1,
        RESOLUTION_RATIO_FreqX4 = 2,

        RESOLUTION_RATIO_NumOptions = 5,
        RESOLUTION_RATIO_DEFAULT = RESOLUTION_RATIO_Equal
    };
    enum OptionsRouting {
        ROUTING_GENERATOR = 1,
        ROUTING_L,
        ROUTING_R,
        ROUTING_MID,
        ROUTING_SIDE,

        ROUTING_NumOptions,
        ROUTING_DEFAUTL = ROUTING_MID
    };
    enum OptionsTransform {
        TRANSFORM_FFT = 1,
        TRANSFORM_FWT,
        TRANSFORM_FWPT,
        TRANSFORM_FWPT_BB,
        TRANSFORM_OFF,

        TRANSFORM_NumOptions,
        TRANSFORM_DEFAULT = TRANSFORM_FFT
    };
    enum OptionsWavelet {
        WAVELET_DAUBECHIES_02 = 1,
        WAVELET_DAUBECHIES_04,
        WAVELET_DAUBECHIES_06,
        WAVELET_DAUBECHIES_08,
        WAVELET_DAUBECHIES_10,
        WAVELET_DAUBECHIES_12,
        WAVELET_DAUBECHIES_14,
        WAVELET_DAUBECHIES_16,
        WAVELET_DAUBECHIES_18,
        WAVELET_DAUBECHIES_20,
        WAVELET_COIFMAN_06,
        WAVELET_COIFMAN_12,
        WAVELET_COIFMAN_18,
        WAVELET_COIFMAN_24,
        WAVELET_COIFMAN_30,
        WAVELET_BEYLKIN_18,
        WAVELET_VAIDYANATHAN_18,

        WAVELET_NumOptions,
        WAVELET_HAAR = WAVELET_DAUBECHIES_02,
        WAVELET_DEFAULT = WAVELET_DAUBECHIES_08
    };
    enum OptionsWindowing {
        WINDOWING_BARTLETT = 1,
        WINDOWING_BLACKMAN,
        WINDOWING_BLACKMAN_HARRIS,
        WINDOWING_HAMMING,
        WINDOWING_HANN,
        WINDOWING_PARZEN,
        WINDOWING_WELCH,
        WINDOWING_RECTANGULAR,
        WINDOWING_NumOptions,
        WINDOWING_DEFAULT = WINDOWING_BLACKMAN_HARRIS
    };

    //"Initialization ... with static storage duration may throw an exception that cannot be caught"
    //Juce doesn't support string_view yet, constexpr doesn't work with string literals and the exception is very unlikely to happen
    //NOLINTBEGIN
    inline const static juce::String PROPERTY_VALUE{"value"};
    inline const static juce::String PARAMETER_COLORMODE{"colormode"};
    inline const static juce::String PARAMETER_LOGMAGNITUDE{"logmagnitude"};
    inline const static juce::String PARAMETER_LOGFREQUENCY{"logfrequency"};
    inline const static juce::String PARAMETER_RESOLUTION{"resolution"};
    inline const static juce::String PARAMETER_ROUTING{"routing"};
    inline const static juce::String PARAMETER_GENERATOR{"generator"};
    inline const static juce::String PARAMETER_GENERATORFREQUENCY{"generatorfrequency"};
    inline const static juce::String PARAMETER_TRANSFORMATION{"transform"};
    inline const static juce::String PARAMETER_WAVELET{"wavelet"};
    inline const static juce::String PARAMETER_WAVELETPACKETBASE{"waveletpacketbase"};
    inline const static juce::String PARAMETER_WINDOWING{"windowing"};
    //NOLINTEND

    // --------------- methods --------------- //

    // Copy-constructors and move- and assignment-operator are deleted, because this class is a singleton.
    SpectronParameters(const SpectronParameters& other) = delete;
    SpectronParameters(SpectronParameters&& other) = delete;
    auto operator=(const SpectronParameters& other) -> SpectronParameters& = delete;
    auto operator=(SpectronParameters&& other) -> SpectronParameters& = delete;

    static auto getSingletonInstance() -> SpectronParameters &;

    static auto isTransformationParameter(const juce::String& parameterID) -> bool;

    void blockParameterChanges() { waitForParameterChange.reset(); }
    void unblockParameterChanges() { waitForParameterChange.signal(); }

    void setParameter(int index, float newValue);
    void setParameter(const juce::String &name, float newValue);
    auto getParameter(int index) -> float;
    auto getParameter(const juce::String &name) -> float;
    auto getParameterName(int index) -> const juce::String;
    auto getParameterIndex(const juce::String &name) -> int;

    auto getColorMode() -> int { return static_cast<int>(getParameter(PARAMETER_INDEX_ColorMode)); }
    auto getGenerator() -> int { return static_cast<int>(getParameter(PARAMETER_INDEX_Generator)); }
    auto getGeneratorFrequency() -> float { return getParameter(PARAMETER_INDEX_GeneratorFrequency); }
    auto getLogMagnitude() -> bool { return static_cast<bool>(getParameter(PARAMETER_INDEX_LogMagnitude)); }
    auto getLogFrequency() -> bool { return static_cast<bool>(getParameter(PARAMETER_INDEX_LogFrequency)); }
    auto getResolution() -> unsigned long { return static_cast<unsigned long>(getParameter(PARAMETER_INDEX_Resolution)); }
    auto getRouting() -> int { return static_cast<int>(getParameter(PARAMETER_INDEX_Routing)); }
    auto getTransformation() -> int { return static_cast<int>(getParameter(PARAMETER_INDEX_Transformation)); }
    auto getWavelet() -> int { return static_cast<int>(getParameter(PARAMETER_INDEX_Wavelet)); }
    auto getWaveletPaketBase() -> int { return static_cast<int>(getParameter(PARAMETER_INDEX_WaveletPacketBase)); }
    auto getWindowing() -> int { return static_cast<int>(getParameter(PARAMETER_INDEX_Windowing)); }

    //Adds a listener by delegating it to juce::ValueTree (see juce API documentation)
    void addListener(juce::ValueTree::Listener *listener, bool sendAllParametersForInitialisation = true);
    //Removes a listener by delegating it to juce::ValueTree (see juce API documentation)
    void removeListener(juce::ValueTree::Listener *listener);
    //read and write to XML
    void readFromXML(const XmlElement &xml);
    auto writeToXML() const -> std::unique_ptr<juce::XmlElement> { return properties.createXml(); }

private:
    enum ChildIndizes {
        CHILD_INDEX_Parameters = 0,
        CHILD_INDEX_Metadata,

        CHILD_INDEX_NumIndizes
    };

    // --------------- members --------------- //
    juce::ValueTree properties = juce::ValueTree("SpectronParameters");
    juce::WaitableEvent waitForParameterChange = juce::WaitableEvent(true);
    juce::CriticalSection criticalSection;

    // --------------- methods --------------- //

    SpectronParameters();
    ~SpectronParameters() = default;
};
