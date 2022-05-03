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

#include <juce_audio_utils/juce_audio_utils.h>
#include <string>

class SpecletParameters {
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
        PARAMETER_INDEX_WaveletPacketBasis,
        PARAMETER_INDEX_Windowing,
        PARAMETER_INDEX_Wavelet,
        PARAMETER_INDEX_Generator,
        PARAMETER_INDEX_GeneratorFrequency,
        PARAMETER_INDEX_LogFrequency,
        PARAMETER_INDEX_LogMagnitude,
        PARAMETER_INDEX_ColorMode,

        TOTAL_NUMBER_OF_PARAMS
    };
    enum OptionsResolution {
        RESOLUTION_256 = 1,
        RESOLUTION_512,
        RESOLUTION_1024,
        RESOLUTION_2048,
        RESOLUTION_4096,
        RESOLUTION_8192,
        RESOLUTION_16384,
        RESOLUTION_32768,
        RESOLUTION_65536,

        RESOLUTION_MAX = RESOLUTION_65536,
        RESOLUTION_DEFAULT = RESOLUTION_4096
    };
    enum OptionsRouting {
        ROUTING_MID = 1,
        ROUTING_SIDE,
        ROUTING_LEFT,
        ROUTING_RIGHT,
        ROUTING_GENERATOR,

        ROUTING_NUMBER_OF_OPTIONS,
        ROUTING_DEFAULT = ROUTING_MID
    };

    //"Initialization ... with static storage duration may throw an exception that cannot be caught"
    //Juce doesn't support string_view yet, constexpr doesn't work with string literals and the exception is very unlikely to happen
    //NOLINTBEGIN
    inline const static juce::String PROPERTY_ID{"id"};
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
    inline const static juce::String PARAMETER_WAVELETPACKETBASIS{"waveletpacketbasis"};
    inline const static juce::String PARAMETER_WINDOWING{"windowing"};
    //NOLINTEND

    // --------------- methods --------------- //

    explicit SpecletParameters(juce::AudioProcessor & audioProcessor);
    ~SpecletParameters() = default;
    SpecletParameters(const SpecletParameters& other) = delete; //non-copyable
    SpecletParameters(SpecletParameters&& other) = delete; //non-moveable
    auto operator=(const SpecletParameters& other) -> SpecletParameters& = delete; //non-copyable
    auto operator=(SpecletParameters&& other) -> SpecletParameters& = delete; //non-moveable

    static auto isTransformationParameter(const juce::String& parameterID) -> bool;

    void blockParameterChanges() const { waitForParameterChange.reset(); }
    void unblockParameterChanges() const { waitForParameterChange.signal(); }

    void setParameter(const juce::String &name, float newValue) const;
    auto getParameter(const juce::String &name) const -> float;
    auto getParameterAsSelection(const juce::String &name) const -> int;
    void setParameterFromSelection(const juce::String &name, int selectedId) const;
    auto getParameterList(const juce::String &name) const -> juce::StringArray;
    
    auto getColorMode() const -> int { return getParameterAsSelection(PARAMETER_COLORMODE); }
    auto getGenerator() const -> int { return getParameterAsSelection(PARAMETER_GENERATOR); }
    auto getGeneratorFrequency() const -> float { return getParameter(PARAMETER_GENERATORFREQUENCY); }
    auto getLogMagnitude() const -> bool { return getParameterAsSelection(PARAMETER_LOGMAGNITUDE); }
    auto getLogFrequency() const -> bool { return getParameterAsSelection(PARAMETER_LOGFREQUENCY); }
    auto getResolution() const -> unsigned long;
    auto getRouting() const -> int { return getParameterAsSelection(PARAMETER_ROUTING); }
    auto getTransformation() const -> int { return getParameterAsSelection(PARAMETER_TRANSFORMATION); }
    auto getWavelet() const -> int { return getParameterAsSelection(PARAMETER_WAVELET); }
    auto getWaveletPacketBasis() const -> int { return getParameterAsSelection(PARAMETER_WAVELETPACKETBASIS); }
    auto getWindowing() const -> int { return getParameterAsSelection(PARAMETER_WINDOWING); }
    auto getParameters() -> juce::AudioProcessorValueTreeState & { return parameters; }
    //Adds a listener by delegating it to juce::ValueTree (see juce API documentation)
    void addListener(juce::ValueTree::Listener *listener, bool sendAllParametersForInitialisation = true) const;
    //Removes a listener by delegating it to juce::ValueTree (see juce API documentation)
    void removeListener(juce::ValueTree::Listener *listener) const;

private:
    enum class ChildIndices {
        CHILD_INDEX_PARAMETERS = 0,
        CHILD_INDEX_METADATA,

        NUMBER_OF_INDICES
    };

    // --------------- members --------------- //
    juce::ValueTree properties = juce::ValueTree("SpecletParameters");
    juce::WaitableEvent waitForParameterChange = juce::WaitableEvent(true);
    juce::CriticalSection criticalSection;
    juce::AudioProcessorValueTreeState parameters;

    // --------------- methods --------------- //
    auto sanitizeParameter(const juce::String &name, float newValue) const -> float;
    template<class _Tp>
    auto enumOptionToFloat(const _Tp& enumType) const -> float;
    
    static auto createParameterLayout() -> juce::AudioProcessorValueTreeState::ParameterLayout;
};
