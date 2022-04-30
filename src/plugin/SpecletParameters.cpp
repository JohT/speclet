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

auto SpecletParameters::getParameter(int index) const -> float {
    return getParameter(getParameterName(index));
    //TODO (JohT) delete if not needed any more
    // juce::ValueTree child = properties.getChild(index);
    // if (!child.isValid()) {
    //     DBG("SpecletParameters::getParameter: Invalid child index: " + juce::String(index));
    //     return 0.0F;
    // }
    // return child.getProperty(PROPERTY_VALUE);
}

auto SpecletParameters::getParameter(const juce::String &name) const -> float {
    auto defaultValue = parameters.getParameter(name)->getDefaultValue();
    auto value = parameters.getRawParameterValue(name)->load();
    DBG("SpecletParameters::getParameter: " + name + ": " + juce::String(value) + " (default: " + juce::String(defaultValue) + ")");
    return value;
    //TODO (JohT) delete if not needed any more
    // juce::ValueTree child = properties.getChildWithName(name);
    // if (!child.isValid()) {
    //     return 0.0F;
    // }
    // return child.getProperty(PROPERTY_VALUE, 0.0F);
}

//TODO (JohT) Declare a int type for parameter values and use it instead of float?
void SpecletParameters::setParameter(int index, float newValue) const {
    assert(index >= 0 && index < properties.getNumChildren());
    if (newValue == 0.0F) {
        DBG("SpecletParameters::setParameter: Value == 0.0F, parameter " << index << " will not be set");
        return;
    }
    const juce::ScopedLock myScopedLock(criticalSection);
    {
        LOG_PERFORMANCE_OF_SCOPE("SpecletParameters setParameter waitForParameterChange(index)");
        bool timeoutDuringWait = waitForParameterChange.wait(TIMEOUT_WAIT_BEFORE_SET);
        if (!timeoutDuringWait) {
            DBG("SpecletParameters::setParameter: Timeout during wait!");
        }
    }
    if ((index == PARAMETER_INDEX_Resolution) && (newValue < enumOptionToFloat(RESOLUTION_256))) {
        DBG("SpecletParameters::setParameter: Resolution might not be smaller than the minimum value, so it's set to 256");
        newValue = enumOptionToFloat(RESOLUTION_256);
    }
    newValue = sanitizeParameter(index, newValue);
    setParameterInternally(index, newValue);
}

auto SpecletParameters::sanitizeParameter(int index, float newValue) const -> float {
    if ((index == PARAMETER_INDEX_Resolution) && (newValue < enumOptionToFloat(RESOLUTION_256))) {
        DBG("SpecletParameters::setParameter: Resolution might not be smaller than the minimum value, so it's set to 256");
        return enumOptionToFloat(RESOLUTION_256);
    }
    return newValue;
}

void SpecletParameters::setParameterInternally(int index, juce::var newValue) const {
    parameters.getParameterAsValue(getParameterName(index)).setValue(newValue);
    //TODO (JohT) delete if not needed any more
    // juce::ValueTree child = properties.getChild(index);
    // if (!child.isValid()) {
    //     DBG("SpecletParameters::setParameter: Invalid child index: " + juce::String(index));
    //     return;
    // }
    // child.setProperty(PROPERTY_VALUE, newValue, nullptr);
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
    auto parameterIndex = getParameterIndex(name);
    newValue = sanitizeParameter(parameterIndex, newValue);
    parameters.getParameterAsValue(name).setValue(newValue);
    //juce::ValueTree child = properties.getChildWithName(name);
    //child.setProperty(PROPERTY_VALUE, newValue, nullptr);
}

auto SpecletParameters::getParameterIndex(const juce::String &name) const -> int {
    if (name.equalsIgnoreCase(PARAMETER_ROUTING)) {
        return PARAMETER_INDEX_Routing;
    } else if (name.equalsIgnoreCase(PARAMETER_TRANSFORMATION)) {
        return PARAMETER_INDEX_Transformation;
    } else if (name.equalsIgnoreCase(PARAMETER_RESOLUTION)) {
        return PARAMETER_INDEX_Resolution;
    } else if (name.equalsIgnoreCase(PARAMETER_WAVELETPACKETBASIS)) {
        return PARAMETER_INDEX_WaveletPacketBasis;
    } else if (name.equalsIgnoreCase(PARAMETER_WINDOWING)) {
        return PARAMETER_INDEX_Windowing;
    } else if (name.equalsIgnoreCase(PARAMETER_WAVELET)) {
        return PARAMETER_INDEX_Wavelet;
    } else if (name.equalsIgnoreCase(PARAMETER_GENERATOR)) {
        return PARAMETER_INDEX_Generator;
    } else if (name.equalsIgnoreCase(PARAMETER_GENERATORFREQUENCY)) {
        return PARAMETER_INDEX_GeneratorFrequency;
    } else if (name.equalsIgnoreCase(PARAMETER_LOGFREQUENCY)) {
        return PARAMETER_INDEX_LogFrequency;
    } else if (name.equalsIgnoreCase(PARAMETER_LOGMAGNITUDE)) {
        return PARAMETER_INDEX_LogMagnitude;
    } else if (name.equalsIgnoreCase(PARAMETER_COLORMODE)) {
        return PARAMETER_INDEX_ColorMode;
    } else {
        DBG("SpecletParameters::getParameterIndex: Unknown parameter name: " << name);
        return -1;
    }
    //TODO (JohT) delete if not needed any more
    // juce::ValueTree child = properties.getChildWithName(name);
    // if (!child.isValid()) {
    //     return -1;
    // }
    // return properties.indexOf(child);
}

auto SpecletParameters::getParameterName(int index) const -> juce::String {
    switch (index) {
        case PARAMETER_INDEX_Routing:
            return PARAMETER_ROUTING;
        case PARAMETER_INDEX_Transformation:
            return PARAMETER_TRANSFORMATION;
        case PARAMETER_INDEX_Resolution:
            return PARAMETER_RESOLUTION;
        case PARAMETER_INDEX_WaveletPacketBasis:
            return PARAMETER_WAVELETPACKETBASIS;
        case PARAMETER_INDEX_Windowing:
            return PARAMETER_WINDOWING;
        case PARAMETER_INDEX_Wavelet:
            return PARAMETER_WAVELET;
        case PARAMETER_INDEX_Generator:
            return PARAMETER_GENERATOR;
        case PARAMETER_INDEX_GeneratorFrequency:
            return PARAMETER_GENERATORFREQUENCY;
        case PARAMETER_INDEX_LogFrequency:
            return PARAMETER_LOGFREQUENCY;
        case PARAMETER_INDEX_LogMagnitude:
            return PARAMETER_LOGMAGNITUDE;
        case PARAMETER_INDEX_ColorMode:
            return PARAMETER_COLORMODE;
        default:
            DBG("SpecletParameters::getParameterName: Unknown parameter index: " << index);
            return "";
    }
    //TODO (JohT) delete if not needed any more
    // juce::ValueTree child = properties.getChild(index);
    // if (!child.isValid()) {
    //     return {};
    // }
    // return child.getType().toString();
}

auto SpecletParameters::getResolution() const -> unsigned long {
    auto choice = getParameter(PARAMETER_RESOLUTION);
    //choice = 0 means resolution = 256 (2 ^ 8) so the exponent is 8 + choice.
    auto resolutionShiftValue = (8U + static_cast<unsigned int>(choice));
    //instead of using pow(2, resolutionShiftValue) we shift the value 1 to the left
    return 1U << resolutionShiftValue;
}

//Adds a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpecletParameters::addListener(juce::ValueTree::Listener *listener, bool sendAllParametersForInitialisation) {
    //TODO (JohT) Revisit listener implementation
    properties.addListener(listener);

    //if selected, the already added listener will be informed about every parameter as if it had been changed
    if (sendAllParametersForInitialisation) {
        for (int i = 0; i < properties.getNumChildren(); i++) {
            juce::ValueTree parameter = properties.getChild(i);
            if (!parameter.isValid()) {
                continue;
            }
            listener->valueTreePropertyChanged(parameter, PROPERTY_VALUE);
        }
    }
}
//Removes a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpecletParameters::removeListener(juce::ValueTree::Listener *listener) {
    properties.removeListener(listener);
}

//Removes a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpecletParameters::readFromXML(const juce::XmlElement &xml) const {
    const juce::ScopedLock myScopedLock(criticalSection);
    juce::ValueTree importedProperties = juce::ValueTree::fromXml(xml);

    //validate imported data (skip import if invalid)
    if (!importedProperties.isValid()) {
        return;
    }

    if (importedProperties.getNumChildren() < properties.getNumChildren()) {
        return;
    }

    //reads all currently defined parameters and overwrites their values,
    //if the same parameter is found in the imported ValueTree
    for (int i = 0; i < properties.getNumChildren(); i++) {
        //get current parameter, skip if invalid
        juce::ValueTree oldParameter = properties.getChild(i);
        if (!oldParameter.isValid()) {
            continue;
        }

        //get new parameter with same name, skip if invalid
        juce::Identifier paramterIdentifier = oldParameter.getType();
        juce::ValueTree newParameter = importedProperties.getChildWithName(paramterIdentifier);
        if (!newParameter.isValid()) {
            continue;
        }

        //overwrite old parameter using the public setter, which will lead to listener notification
        juce::var newParameterValue = newParameter.getProperty(PROPERTY_VALUE);
        setParameter(paramterIdentifier.toString(), static_cast<float>(newParameterValue));
    }
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

    //TODO (JohT) Adjust routing values
    //ROUTING_GENERATOR = 1, ROUTING_L, ROUTING_R, ROUTING_MID, ROUTING_SIDE
    auto routingOptions = {"Mid", "Side", "Left", "Right", "Oscillator"};
    addToLayout<juce::AudioParameterChoice>(layout, PARAMETER_ROUTING, PARAMETER_ROUTING, routingOptions, 0, "Audio Source");

    auto transformationOptions = {"FFT", "FWT", "WPT", "WPT BestBasis", "Off"};
    addToLayout<juce::AudioParameterChoice>(layout, PARAMETER_TRANSFORMATION, PARAMETER_TRANSFORMATION, transformationOptions, 0, "Transformation");

    auto resolutionOptions = {"256", "512", "1024", "2048", "4096", "8192", "16384", "32768", "65536"};
    addToLayout<juce::AudioParameterChoice>(layout, PARAMETER_RESOLUTION, PARAMETER_RESOLUTION, resolutionOptions, 3, "Resolution");

    //TODO (JohT) Adjust packet basis resolution ratio values
    //TIME_X4 = -2, TIME_X2 = -1, FREQUENCY_X2 = 1, FREQUENCY_X4 = 2, EQUAL = 99,
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
