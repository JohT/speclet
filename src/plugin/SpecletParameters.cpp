#include "SpecletParameters.h"
#include "../utilities/PerformanceLogger.h"
#include "../dsp/transformations/TransformationParameters.h"
#include "../dsp/transformations/WaveletParameters.h"
#include "../dsp/windowing/WindowParameters.h"
#include "../ui/ColorGradientsParameters.h"
#include "../ui/SpecletDrawerParameters.h"
#include "../dsp/SignalGeneratorParameters.h"
#include <cassert>

SpecletParameters::SpecletParameters() {
    //create ValueTree object, which stores all parameters as childs in a tree structure

    //add parameters as tree childs (must start with zero and use ascending indices -> enum)
    properties.addChild(juce::ValueTree(PARAMETER_ROUTING), PARAMETER_INDEX_Routing, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_TRANSFORMATION), PARAMETER_INDEX_Transformation, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_RESOLUTION), PARAMETER_INDEX_Resolution, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_WAVELETPACKETBASIS), PARAMETER_INDEX_WaveletPacketBasis, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_WINDOWING), PARAMETER_INDEX_Windowing, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_WAVELET), PARAMETER_INDEX_Wavelet, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_GENERATOR), PARAMETER_INDEX_Generator, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_GENERATORFREQUENCY), PARAMETER_INDEX_GeneratorFrequency, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_LOGFREQUENCY), PARAMETER_INDEX_LogFrequency, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_LOGMAGNITUDE), PARAMETER_INDEX_LogMagnitude, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_COLORMODE), PARAMETER_INDEX_ColorMode, nullptr);

    //Initialize with default settings
    setParameterInternally(PARAMETER_INDEX_ColorMode, enumOptionToFloat(ColorGradientsParameters::ColorMode::DEFAULT));
    setParameterInternally(PARAMETER_INDEX_Generator, enumOptionToFloat(SignalGeneratorParameters::Waveform::DEFAULT));
    setParameterInternally(PARAMETER_INDEX_GeneratorFrequency, 1000.0);
    setParameterInternally(PARAMETER_INDEX_LogFrequency, enumOptionToFloat(SpecletDrawerParameters::Axis::DEFAULT));
    setParameterInternally(PARAMETER_INDEX_LogMagnitude, enumOptionToFloat(SpecletDrawerParameters::Axis::DEFAULT));
    setParameterInternally(PARAMETER_INDEX_Resolution, RESOLUTION_DEFAULT);
    setParameterInternally(PARAMETER_INDEX_Routing, ROUTING_MID);
    setParameterInternally(PARAMETER_INDEX_Transformation, enumOptionToFloat(TransformationParameters::Type::DEFAULT));
    setParameterInternally(PARAMETER_INDEX_Wavelet, enumOptionToFloat(WaveletParameters::WaveletBase::DEFAULT));
    setParameterInternally(PARAMETER_INDEX_WaveletPacketBasis, enumOptionToFloat(WaveletParameters::ResolutionRatioOption::DEFAULT));
    setParameterInternally(PARAMETER_INDEX_Windowing, enumOptionToFloat(WindowParameters::WindowFunction::DEFAULT));

    assert(getResolution() > 0);
    waitForParameterChange.signal();
}

template<class _Tp>
auto SpecletParameters::enumOptionToFloat(const _Tp& enumType) const -> float{
   auto enumValue = static_cast<typename std::underlying_type<_Tp>::type>(enumType);
   return static_cast<float>(enumValue);
}

//TODO (JohT) Does it really need to be a singleton? Is classic dependency injection a viable alternative?
auto SpecletParameters::getSingletonInstance() -> SpecletParameters & {
    static SpecletParameters singletonInstance;
    return singletonInstance;
}

auto SpecletParameters::isTransformationParameter(const juce::String &parameterID) -> bool {
    return parameterID.equalsIgnoreCase(SpecletParameters::PARAMETER_RESOLUTION) || parameterID.equalsIgnoreCase(SpecletParameters::PARAMETER_TRANSFORMATION) || parameterID.equalsIgnoreCase(SpecletParameters::PARAMETER_WAVELET) || parameterID.equalsIgnoreCase(SpecletParameters::PARAMETER_WAVELETPACKETBASIS) || parameterID.equalsIgnoreCase(SpecletParameters::PARAMETER_WINDOWING);
}

auto SpecletParameters::getParameter(int index) -> float {
    juce::ValueTree child = properties.getChild(index);
    if (!child.isValid()) {
        DBG("SpecletParameters::getParameter: Invalid child index: " + juce::String(index));
        return 0.0F;
    }
    return child.getProperty(PROPERTY_VALUE);
}

auto SpecletParameters::getParameter(const juce::String &name) -> float {
    juce::ValueTree child = properties.getChildWithName(name);
    if (!child.isValid()) {
        return 0.0F;
    }
    return child.getProperty(PROPERTY_VALUE, 0.0F);
}

//TODO (JohT) Declare a int type for parameter values and use it instead of float?
void SpecletParameters::setParameter(int index, float newValue) {
    assert(index >= 0 && index < properties.getNumChildren());
    if (newValue < 1.0F) {
        DBG("SpecletParameters::setParameter: Value < 1.0F, parameter " << index << " will not be set");
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
    if (index == PARAMETER_INDEX_Resolution) {
        if (newValue < enumOptionToFloat(RESOLUTION_256)) {
            DBG("SpecletParameters::setParameter: Resolution might not be smaller than the minimum value, so it's set to 256");
            newValue = enumOptionToFloat(RESOLUTION_256);
        }
    }
    newValue = sanitizeParameter(index, newValue);
    setParameterInternally(index, newValue);
}

auto SpecletParameters::sanitizeParameter(int index, float newValue) -> float{
    if (index == PARAMETER_INDEX_Resolution) {
        if (newValue < enumOptionToFloat(RESOLUTION_256)) {
            DBG("SpecletParameters::setParameter: Resolution might not be smaller than the minimum value, so it's set to 256");
            return enumOptionToFloat(RESOLUTION_256);
        }
    }
    return newValue;
}

void SpecletParameters::setParameterInternally(int index, juce::var newValue) {
    juce::ValueTree child = properties.getChild(index);
    if (!child.isValid()) {
        DBG("SpecletParameters::setParameter: Invalid child index: " + juce::String(index));
        return;
    }
    child.setProperty(PROPERTY_VALUE, newValue, nullptr);
}

void SpecletParameters::setParameter(const juce::String &name, float newValue) {
    const juce::ScopedLock myScopedLock(criticalSection);
    {
        LOG_PERFORMANCE_OF_SCOPE("SpecletParameters setParameter waitForParameterChange(name)");
        bool timeoutDuringWait = waitForParameterChange.wait(TIMEOUT_WAIT_BEFORE_SET);
        if (!timeoutDuringWait) {
            DBG("SpecletParameters::setParameter: Timeout during wait!");
        }
    }
    juce::ValueTree child = properties.getChildWithName(name);
    if (!child.isValid()) {
        return;
    }
    auto parameterIndex = properties.indexOf(child);
    newValue = sanitizeParameter(parameterIndex, newValue);
    child.setProperty(PROPERTY_VALUE, newValue, nullptr);
}

auto SpecletParameters::getParameterIndex(const juce::String &name) -> int {
    juce::ValueTree child = properties.getChildWithName(name);
    if (!child.isValid()) {
        return -1;
    }
    return properties.indexOf(child);
}

auto SpecletParameters::getParameterName(int index) -> const juce::String {
    juce::ValueTree child = properties.getChild(index);
    if (!child.isValid()) {
        return {};
    }
    return child.getType().toString();
}

//Adds a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpecletParameters::addListener(juce::ValueTree::Listener *listener, bool sendAllParametersForInitialisation) {
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
void SpecletParameters::readFromXML(const juce::XmlElement &xml) {
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
