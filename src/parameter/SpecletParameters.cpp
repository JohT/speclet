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
}

auto SpecletParameters::getParameterAsSelection(const juce::String &name) const -> int {
    return static_cast<int>(getParameter(name)) + 1;
}

auto SpecletParameters::getParameterList(const juce::String &name) const -> juce::StringArray {
   return parameters.getParameter(name)->getAllValueStrings();
}

auto SpecletParameters::getResolution() const -> unsigned long {
    auto choice = getParameterAsSelection(PARAMETER_RESOLUTION);
    //choice = 1 means resolution = 256 (2 ^ 8) so the exponent is 7 + choice.
    auto resolutionShiftValue = 7 + choice;
    //instead of using pow(2, resolutionShiftValue) we shift the value 1 to the left
    return 1U << resolutionShiftValue;
}

void SpecletParameters::addListener(juce::AudioProcessorValueTreeState::Listener *listener) {
    for (int i = 0; i < parameters.state.getNumChildren(); i++) {
        auto parameter = parameters.state.getChild(i);
        if (parameter.isValid()) {
            auto parameterId = parameter.getProperty(PROPERTY_ID).toString();
            parameters.addParameterListener(parameterId, listener);
        }
    }
}

void SpecletParameters::removeListener(juce::AudioProcessorValueTreeState::Listener *listener) {
    for (int i = 0; i < parameters.state.getNumChildren(); i++) {
        auto parameter = parameters.state.getChild(i);
        if (parameter.isValid()) {
            auto parameterId = parameter.getProperty(PROPERTY_ID).toString();
            parameters.removeParameterListener(parameterId, listener);
        }
    }
}

void SpecletParameters::getStateInformation(juce::MemoryBlock &destData) const {
    // You should call this method from the audio processor's getStateInformation to store your parameters in the memory block.
    juce::MemoryOutputStream memoryOutputStream(destData, true);
    parameters.state.writeToStream(memoryOutputStream);
}

void SpecletParameters::setStateInformation(const juce::ValueTree &tree) {
    // You should call this method from the audio processor's setStateInformation to restore your parameters.
    if (tree.isValid()) {
        parameters.replaceState(tree);
    }
}

template<typename Param>
static void add(juce::AudioProcessorValueTreeState::ParameterLayout &group, std::unique_ptr<Param> param) {
    group.add(std::move(param));
}

template<typename Param, typename Group, typename Id, typename... Ts>
static Param &addToLayout(Group &layout, Id &&idString, Ts &&...ts) {
    auto parameterId = juce::ParameterID(std::forward<Id>(idString), SpecletParameters::Constants::VERSION_1);
    auto param = std::make_unique<Param>(parameterId, std::forward<Ts>(ts)...);
    auto &ref = *param;
    add(layout, std::move(param));
    return ref;
}

auto SpecletParameters::createParameterLayout() -> juce::AudioProcessorValueTreeState::ParameterLayout {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    //TODO Routing Parameter for special Audio Unit parameter index?
    auto routingParameter = juce::ParameterID(PARAMETER_ROUTING, 1);
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
