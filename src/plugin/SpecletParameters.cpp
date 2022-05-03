#include "SpecletParameters.h"
#include "../dsp/SignalGeneratorParameters.h"
#include "../dsp/transformations/TransformationParameters.h"
#include "../dsp/transformations/WaveletParameters.h"
#include "../dsp/windowing/WindowParameters.h"
#include "../ui/ColorGradientsParameters.h"
#include "../ui/SpecletDrawerParameters.h"
#include "../utilities/PerformanceLogger.h"
#include "juce_core/system/juce_PlatformDefs.h"
#include <cassert>

SpecletParameters::SpecletParameters(juce::AudioProcessor &audioProcessor)
    : parameters(audioProcessor, nullptr, "Parameters", createParameterLayout()) {
    //create ValueTree object, which stores all parameters as childs in a tree structure
    
    //TODO (JohT) delete if not needed any more
    //add parameters as tree childs (must start with zero and use ascending indices -> enum)
    // properties.addChild(juce::ValueTree(PARAMETER_ROUTING), PARAMETER_INDEX_Routing, nullptr);
    // properties.addChild(juce::ValueTree(PARAMETER_TRANSFORMATION), PARAMETER_INDEX_Transformation, nullptr);
    // properties.addChild(juce::ValueTree(PARAMETER_RESOLUTION), PARAMETER_INDEX_Resolution, nullptr);
    // properties.addChild(juce::ValueTree(PARAMETER_WAVELETPACKETBASIS), PARAMETER_INDEX_WaveletPacketBasis, nullptr);
    // properties.addChild(juce::ValueTree(PARAMETER_WINDOWING), PARAMETER_INDEX_Windowing, nullptr);
    // properties.addChild(juce::ValueTree(PARAMETER_WAVELET), PARAMETER_INDEX_Wavelet, nullptr);
    // properties.addChild(juce::ValueTree(PARAMETER_GENERATOR), PARAMETER_INDEX_Generator, nullptr);
    // properties.addChild(juce::ValueTree(PARAMETER_GENERATORFREQUENCY), PARAMETER_INDEX_GeneratorFrequency, nullptr);
    // properties.addChild(juce::ValueTree(PARAMETER_LOGFREQUENCY), PARAMETER_INDEX_LogFrequency, nullptr);
    // properties.addChild(juce::ValueTree(PARAMETER_LOGMAGNITUDE), PARAMETER_INDEX_LogMagnitude, nullptr);
    // properties.addChild(juce::ValueTree(PARAMETER_COLORMODE), PARAMETER_INDEX_ColorMode, nullptr);

    //Initialize with default settings
    //TODO (JohT) delete if not needed any more
    // setParameterInternally(PARAMETER_INDEX_ColorMode, enumOptionToFloat(ColorGradientsParameters::ColorMode::DEFAULT));
    // setParameterInternally(PARAMETER_INDEX_Generator, enumOptionToFloat(SignalGeneratorParameters::Waveform::DEFAULT));
    // setParameterInternally(PARAMETER_INDEX_GeneratorFrequency, 1000.0);
    // setParameterInternally(PARAMETER_INDEX_LogFrequency, enumOptionToFloat(SpecletDrawerParameters::Axis::DEFAULT));
    // setParameterInternally(PARAMETER_INDEX_LogMagnitude, enumOptionToFloat(SpecletDrawerParameters::Axis::DEFAULT));
    // setParameterInternally(PARAMETER_INDEX_Resolution, RESOLUTION_DEFAULT);
    // setParameterInternally(PARAMETER_INDEX_Routing, ROUTING_MID);
    // setParameterInternally(PARAMETER_INDEX_Transformation, enumOptionToFloat(TransformationParameters::Type::DEFAULT));
    // setParameterInternally(PARAMETER_INDEX_Wavelet, enumOptionToFloat(WaveletParameters::WaveletBase::DEFAULT));
    // setParameterInternally(PARAMETER_INDEX_WaveletPacketBasis, enumOptionToFloat(WaveletParameters::ResolutionRatioOption::DEFAULT));
    // setParameterInternally(PARAMETER_INDEX_Windowing, enumOptionToFloat(WindowParameters::WindowFunction::DEFAULT));

    assert(getResolution() > 0);

    waitForParameterChange.signal();
}

template<class _Tp>
auto SpecletParameters::enumOptionToFloat(const _Tp &enumType) const -> float {
    auto enumValue = static_cast<typename std::underlying_type_t<_Tp>>(enumType);
    return static_cast<float>(enumValue);
}

auto SpecletParameters::isTransformationParameter(const juce::String &parameterID) -> bool {
    return parameterID.equalsIgnoreCase(SpecletParameters::PARAMETER_RESOLUTION) || parameterID.equalsIgnoreCase(SpecletParameters::PARAMETER_TRANSFORMATION) || parameterID.equalsIgnoreCase(SpecletParameters::PARAMETER_WAVELET) || parameterID.equalsIgnoreCase(SpecletParameters::PARAMETER_WAVELETPACKETBASIS) || parameterID.equalsIgnoreCase(SpecletParameters::PARAMETER_WINDOWING);
}

auto SpecletParameters::getParameter(const juce::String &name) const -> float {
    return parameters.getRawParameterValue(name)->load();
    //TODO (JohT) delete if not needed any more
    // juce::ValueTree child = properties.getChildWithName(name);
    // if (!child.isValid()) {
    //     return 0.0F;
    // }
    // return child.getProperty(PROPERTY_VALUE, 0.0F);
}

auto SpecletParameters::getParameterAsSelection(const juce::String &name) const -> int {
    return static_cast<int>(getParameter(name)) + 1;
}

void SpecletParameters::setParameterFromSelection(const juce::String &name, int selectedId) const {
    setParameter(name, static_cast<float>(selectedId - 1));
}

auto SpecletParameters::getParameterList(const juce::String &name) const -> juce::StringArray {
   return parameters.getParameter(name)->getAllValueStrings();
}

auto SpecletParameters::sanitizeParameter(const juce::String &name, float newValue) const -> float {
    if ((name == PARAMETER_RESOLUTION) && (newValue < enumOptionToFloat(RESOLUTION_256))) {
        DBG("SpecletParameters::setParameter: Resolution might not be smaller than the minimum value, so it's set to 256");
        return enumOptionToFloat(RESOLUTION_256);
    }
    return newValue;
}

void SpecletParameters::setParameter(const juce::String &name, float newValue) const {
    const juce::ScopedLock myScopedLock(criticalSection);
    {
        LOG_PERFORMANCE_OF_SCOPE("SpecletParameters setParameter waitForParameterChange(name)");
        bool timeoutDuringWait = waitForParameterChange.wait(TIMEOUT_WAIT_BEFORE_SET);
        if (!timeoutDuringWait) {
            DBG("SpecletParameters::setParameter: Timeout during wait!");
        }
    }
    //TODO (JohT) delete commented lines if not needed any more
    //parameters.getParameterAsValue(name).setValue(newValue);
    //if (!child.isValid()) {
    //    return;
    //}
    newValue = sanitizeParameter(name, newValue);
    parameters.getRawParameterValue(name)->store(newValue);
    //juce::ValueTree child = properties.getChildWithName(name);
    //child.setProperty(PROPERTY_VALUE, newValue, nullptr);
}

auto SpecletParameters::getResolution() const -> unsigned long {
    auto choice = getParameterAsSelection(PARAMETER_RESOLUTION);
    //choice = 1 means resolution = 256 (2 ^ 8) so the exponent is 7 + choice.
    auto resolutionShiftValue = 7 + choice;
    //instead of using pow(2, resolutionShiftValue) we shift the value 1 to the left
    return 1U << resolutionShiftValue;
}

//Adds a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpecletParameters::addListener(juce::ValueTree::Listener *listener, bool sendAllParametersForInitialisation) const {
    for (int i = 0; i < parameters.state.getNumChildren(); i++) {
        juce::ValueTree parameter = parameters.state.getChild(i);
        if (!parameter.isValid()) {
            DBG("SpecletParameters::addListener: Invalid parameter: " << parameter.getType().toString() << "(index: " << i << ")");
            continue;
        }
        DBG("SpecletParameters::addListener: Adding listener to parameter: " << parameter.getProperty(PROPERTY_VALUE).toString() << "(id: " << parameter.getProperty("id").toString() << ")");
        parameter.addListener(listener);
        //TODO (JohT) delete commented lines if not needed any more
        //if (sendAllParametersForInitialisation) {
        //    listener->valueTreePropertyChanged(parameter, PROPERTY_VALUE);
        //}
    }

    //TODO (JohT) delete commented lines if not needed any more
    // properties.addListener(listener);

    // //if selected, the already added listener will be informed about every parameter as if it had been changed
    // if (sendAllParametersForInitialisation) {
    //     for (int i = 0; i < properties.getNumChildren(); i++) {
    //         juce::ValueTree parameter = properties.getChild(i);
    //         if (!parameter.isValid()) {
    //             continue;
    //         }
    //         listener->valueTreePropertyChanged(parameter, PROPERTY_VALUE);
    //     }
    // }
}
//Removes a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpecletParameters::removeListener(juce::ValueTree::Listener *listener) const {
     if (listener == nullptr) {
         DBG("SpecletParameters::removeListener: Listener is nullptr");
        return;
     }
     for (int i = 0; i < parameters.state.getNumChildren(); i++) {
        juce::ValueTree parameter = parameters.state.getChild(i);
        if (!parameter.isValid()) {
            DBG("SpecletParameters::removeListener: Invalid parameter: " << parameter.getType().toString() << "(index: " << i << ")");
            continue;
        }
        parameter.removeListener(listener);
    }
    //TODO (JohT) delete commented lines if not needed any more
    //properties.removeListener(listener);
}

template<typename Param>
static void add(juce::AudioProcessorValueTreeState::ParameterLayout &group, std::unique_ptr<Param> param) {
    group.add(std::move(param));
}

template<typename Param, typename Group, typename... Ts>
static Param &addToLayout(Group &layout, Ts &&...ts) {
    auto param = std::make_unique<Param>(std::forward<Ts>(ts)...);
    auto &ref = *param;
    add(layout, std::move(param));
    return ref;
}

auto SpecletParameters::createParameterLayout() -> juce::AudioProcessorValueTreeState::ParameterLayout {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto routingOptions = {"Mid", "Side", "Left", "Right", "Oscillator"};
    addToLayout<juce::AudioParameterChoice>(layout, PARAMETER_ROUTING, PARAMETER_ROUTING, routingOptions, 0, "Audio Source");

    auto transformationOptions = {"FFT", "FWT", "WPT", "WPT BestBasis", "Off"};
    addToLayout<juce::AudioParameterChoice>(layout, PARAMETER_TRANSFORMATION, PARAMETER_TRANSFORMATION, transformationOptions, 0, "Transformation");

    auto resolutionOptions = {"256", "512", "1024", "2048", "4096", "8192", "16384", "32768", "65536"};
    addToLayout<juce::AudioParameterChoice>(layout, PARAMETER_RESOLUTION, PARAMETER_RESOLUTION, resolutionOptions, 3, "Resolution");

    auto waveletPacketBasisOptions = {"Time x4", "Time x2", "Freq/Time x1", "Freq x2", "Freq x4"};
    addToLayout<juce::AudioParameterChoice>(layout, PARAMETER_WAVELETPACKETBASIS, PARAMETER_WAVELETPACKETBASIS, waveletPacketBasisOptions, 2, "Wavelet Packet Basis");

    auto windowingOptions = {"Barlett", "Blackman", "Blackman-Harris", "Hamming", "Hann", "Parzen", "Welch", "Rectangular"};
    addToLayout<juce::AudioParameterChoice>(layout, PARAMETER_WINDOWING, PARAMETER_WINDOWING, windowingOptions, 2, "Window Function");

    auto waveletOptions = {"Haar (2)", "Daubechies (4)", "Daubechies (6)", "Daubechies (8)", "Daubechies (10)", "Daubechies (12)", "Daubechies (14)", "Daubechies (16)", "Daubechies (18)", "Daubechies (20)", "Coifman (6)", "Coifman (12)", "Coifman (18)", "Coifman (24)", "Coifman (30)", "Beylkin (18)", "Vaidyanathan (18)"};
    addToLayout<juce::AudioParameterChoice>(layout, PARAMETER_WAVELET, PARAMETER_WAVELET, waveletOptions, 17, "Wavelet");

    auto generatorFrequencyRange = juce::NormalisableRange<float>(10.0F, 20000.0F, 1.0F, 0.25F);
    addToLayout<juce::AudioParameterFloat>(layout, PARAMETER_GENERATORFREQUENCY, PARAMETER_GENERATORFREQUENCY, generatorFrequencyRange, 100.0f, "Oscillator Frequency");

    auto generatorOptions = {"Sine", "Triangle", "Sawtooth", "Rectangle", "Noise"};
    addToLayout<juce::AudioParameterChoice>(layout, PARAMETER_GENERATOR, PARAMETER_GENERATOR, generatorOptions, 0, "Oscillator");

    auto axisOptions = {"linear", "logarithmic"};
    addToLayout<juce::AudioParameterChoice>(layout, PARAMETER_LOGFREQUENCY, PARAMETER_LOGFREQUENCY, axisOptions, 1, "Frequency Scale");
    addToLayout<juce::AudioParameterChoice>(layout, PARAMETER_LOGMAGNITUDE, PARAMETER_LOGMAGNITUDE, axisOptions, 1, "Magnitude Scale");

    auto colorModeOptions = {"Blue", "Green", "Rainbow", "Fire"};
    addToLayout<juce::AudioParameterChoice>(layout, PARAMETER_COLORMODE, PARAMETER_COLORMODE, colorModeOptions, 3, "Color Mode");

    return layout;
}
