#include "SpectronParameters.h"
#include "../utilities/PerformanceLogger.h"

SpectronParameters::SpectronParameters() {
    //create ValueTree object, which stores all parameters as childs in a tree structure

    //add parameters as tree childs (must start with zero and use ascending indices -> enum)
    properties.addChild(juce::ValueTree(PARAMETER_ROUTING), PARAMETER_INDEX_Routing, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_TRANSFORMATION), PARAMETER_INDEX_Transformation, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_RESOLUTION), PARAMETER_INDEX_Resolution, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_WAVELETPACKETBASE), PARAMETER_INDEX_WaveletPacketBase, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_WINDOWING), PARAMETER_INDEX_Windowing, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_WAVELET), PARAMETER_INDEX_Wavelet, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_GENERATOR), PARAMETER_INDEX_Generator, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_GENERATORFREQUENCY), PARAMETER_INDEX_GeneratorFrequency, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_LOGFREQUENCY), PARAMETER_INDEX_LogFrequency, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_LOGMAGNITUDE), PARAMETER_INDEX_LogMagnitude, nullptr);
    properties.addChild(juce::ValueTree(PARAMETER_COLORMODE), PARAMETER_INDEX_ColorMode, nullptr);

    //add value zero to every tree child's value property
    for (int i = 0; i < properties.getNumChildren(); i++) {
        properties.getChild(i).setProperty(PROPERTY_VALUE, static_cast<float>(0), nullptr);
    }

    waitForParameterChange.signal();
}

auto SpectronParameters::getSingletonInstance() -> SpectronParameters & {
    static SpectronParameters singletonInstance;
    return singletonInstance;
}

auto SpectronParameters::isTransformationParameter(const juce::String &parameterID) -> bool {
    return parameterID.equalsIgnoreCase(SpectronParameters::PARAMETER_RESOLUTION) || parameterID.equalsIgnoreCase(SpectronParameters::PARAMETER_TRANSFORMATION) || parameterID.equalsIgnoreCase(SpectronParameters::PARAMETER_WAVELET) || parameterID.equalsIgnoreCase(SpectronParameters::PARAMETER_WAVELETPACKETBASE) || parameterID.equalsIgnoreCase(SpectronParameters::PARAMETER_WINDOWING);
}

auto SpectronParameters::getParameter(int index) -> float {
    juce::ValueTree child = properties.getChild(index);
    if (!child.isValid()) {
        return 0.0F;
    }
    return child.getProperty(PROPERTY_VALUE);
}

auto SpectronParameters::getParameter(const juce::String &name) -> float {
    juce::ValueTree child = properties.getChildWithName(name);
    if (!child.isValid()) {
        return 0.0F;
    }
    return child.getProperty(PROPERTY_VALUE, 0.0F);
}

void SpectronParameters::setParameter(int index, float newValue) {
    const juce::ScopedLock myScopedLock(criticalSection);
    {
        LOG_PERFORMANCE_OF_SCOPE("SpectronParameters setParameter waitForParameterChange(index)");
        bool timeoutDuringWait = waitForParameterChange.wait(TIMEOUT_WAIT_BEFORE_SET);
        if (!timeoutDuringWait) {
            DBG("SpectronParameters::setParameter: Timeout during wait!");
        }
    }
    juce::ValueTree child = properties.getChild(index);
    if (!child.isValid()) {
        return;
    }
    child.setProperty(PROPERTY_VALUE, newValue, nullptr);
}

void SpectronParameters::setParameter(const juce::String &name, float newValue) {
    const juce::ScopedLock myScopedLock(criticalSection);

    {
        LOG_PERFORMANCE_OF_SCOPE("SpectronParameters setParameter waitForParameterChange(name)");
        bool timeoutDuringWait = waitForParameterChange.wait(TIMEOUT_WAIT_BEFORE_SET);
        if (!timeoutDuringWait) {
            DBG("SpectronParameters::setParameter: Timeout during wait!");
        }
    }
    juce::ValueTree child = properties.getChildWithName(name);
    if (!child.isValid()) {
        return;
    }
    child.setProperty(PROPERTY_VALUE, newValue, nullptr);
}

auto SpectronParameters::getParameterIndex(const juce::String &name) -> int {
    juce::ValueTree child = properties.getChildWithName(name);
    if (!child.isValid()) {
        return -1;
    }
    return properties.indexOf(child);
}

auto SpectronParameters::getParameterName(int index) -> const juce::String {
    juce::ValueTree child = properties.getChild(index);
    if (!child.isValid()) {
        return {};
    }
    return child.getType().toString();
}

//Adds a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpectronParameters::addListener(juce::ValueTree::Listener *listener, bool sendAllParametersForInitialisation) {
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
void SpectronParameters::removeListener(juce::ValueTree::Listener *listener) {
    properties.removeListener(listener);
}

//Removes a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpectronParameters::readFromXML(const juce::XmlElement &xml) {
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
