#include "SpectronParameters.h"
#include "../utilities/PerformanceManager.h"

// Singleton instance variable (only one instance of this class)
using juce::String;

SpectronParameters *SpectronParameters::singletonInstance = nullptr;

const juce::String SpectronParameters::PROPERTY_VALUE = "value";
const juce::String SpectronParameters::PARAMETER_COLORMODE = "colormode";
const juce::String SpectronParameters::PARAMETER_LOGMAGNITUDE = "logmagnitude";
const juce::String SpectronParameters::PARAMETER_LOGFREQUENCY = "logfrequency";
const juce::String SpectronParameters::PARAMETER_RESOLUTION = "resolution";
const juce::String SpectronParameters::PARAMETER_ROUTING = "routing";
const juce::String SpectronParameters::PARAMETER_GENERATOR = "generator";
const juce::String SpectronParameters::PARAMETER_GENERATORFREQUENCY = "generatorfrequency";
const juce::String SpectronParameters::PARAMETER_TRANSFORMATION = "transform";
const juce::String SpectronParameters::PARAMETER_WAVELET = "wavelet";
const juce::String SpectronParameters::PARAMETER_WAVELETPAKETBASE = "waveletpaketbase";
const juce::String SpectronParameters::PARAMETER_WINDOWING = "windowing";

SpectronParameters::SpectronParameters() {
    //create ValueTree object, which stores all parameters as childs in a tree structure

    properties = new juce::ValueTree("SpectronParameters");

    //add parameters as tree childs (must start with zero and use ascending indices -> enum)
    properties->addChild(juce::ValueTree(PARAMETER_ROUTING), PARAMETER_INDEX_Routing, nullptr);
    properties->addChild(juce::ValueTree(PARAMETER_TRANSFORMATION), PARAMETER_INDEX_Transformation, nullptr);
    properties->addChild(juce::ValueTree(PARAMETER_RESOLUTION), PARAMETER_INDEX_Resolution, nullptr);
    properties->addChild(juce::ValueTree(PARAMETER_WAVELETPAKETBASE), PARAMETER_INDEX_WaveletPaketBase, nullptr);
    properties->addChild(juce::ValueTree(PARAMETER_WINDOWING), PARAMETER_INDEX_Windowing, nullptr);
    properties->addChild(juce::ValueTree(PARAMETER_WAVELET), PARAMETER_INDEX_Wavelet, nullptr);
    properties->addChild(juce::ValueTree(PARAMETER_GENERATOR), PARAMETER_INDEX_Generator, nullptr);
    properties->addChild(juce::ValueTree(PARAMETER_GENERATORFREQUENCY), PARAMETER_INDEX_GeneratorFrequency, nullptr);
    properties->addChild(juce::ValueTree(PARAMETER_LOGFREQUENCY), PARAMETER_INDEX_LogFrequency, nullptr);
    properties->addChild(juce::ValueTree(PARAMETER_LOGMAGNITUDE), PARAMETER_INDEX_LogMagnitude, nullptr);
    properties->addChild(juce::ValueTree(PARAMETER_COLORMODE), PARAMETER_INDEX_ColorMode, nullptr);

    //add value zero to every tree child's value property
    for (int i = 0; i < properties->getNumChildren(); i++) {
        properties->getChild(i).setProperty(PROPERTY_VALUE, static_cast<float>(0), nullptr);
    }

    waitForParameterChange = new juce::WaitableEvent(true);
    waitForParameterChange->signal();
}

SpectronParameters::~SpectronParameters() {
}

SpectronParameters *SpectronParameters::getSingletonInstance() {
    // Method to get the single instance of this class (Singleton)
    if (SpectronParameters::singletonInstance == 0) {
        SpectronParameters::singletonInstance = new SpectronParameters();
    }
    return SpectronParameters::singletonInstance;
}

void SpectronParameters::destruct() {
    if (singletonInstance == nullptr) {
        return;
    }

    delete (singletonInstance);
    singletonInstance = nullptr;
}

float SpectronParameters::getParameter(int index) {
    juce::ValueTree child = properties->getChild(index);
    if (!child.isValid()) return (float) 0;
    return child.getProperty(PROPERTY_VALUE);
}

auto SpectronParameters::getParameter(const juce::String &name) -> float {
    juce::ValueTree child = properties->getChildWithName(name);
    if (!child.isValid()) {
        return 0.0F;
    }
    return child.getProperty(PROPERTY_VALUE, 0.0F);
}

void SpectronParameters::setParameter(int index, float newValue) {
    const ScopedLock myScopedLock(criticalSection);

    PerformanceManager::getSingletonInstance().start("waitForParameterChange");
    bool timeoutDuringWait = waitForParameterChange->wait(TIMEOUT_WAIT_BEFORE_SET);
    PerformanceManager::getSingletonInstance().stop("waitForParameterChange");
    if (!timeoutDuringWait) {
        DBG("SpectronParameters::setParameter: Timeout during wait!");
    }

    juce::ValueTree child = properties->getChild(index);
    if (!child.isValid()) {
        return;
    }
    child.setProperty(PROPERTY_VALUE, newValue, nullptr);
}

void SpectronParameters::setParameter(const juce::String &name, float newValue) {
    const ScopedLock myScopedLock(criticalSection);

    PerformanceManager::getSingletonInstance().start("waitForParameterChange");
    bool timeoutDuringWait = waitForParameterChange->wait(TIMEOUT_WAIT_BEFORE_SET);
    PerformanceManager::getSingletonInstance().stop("waitForParameterChange");
    if (!timeoutDuringWait) {
        DBG("SpectronParameters::setParameter: Timeout during wait!");
    }

    juce::ValueTree child = properties->getChildWithName(name);
    if (!child.isValid()) {
        return;
    }
    child.setProperty(PROPERTY_VALUE, newValue, nullptr);
}

auto SpectronParameters::getParameterIndex(const String &name) -> int {
    juce::ValueTree child = properties->getChildWithName(name);
    if (!child.isValid()) {
        return -1;
    }
    return properties->indexOf(child);
}

auto SpectronParameters::getParameterName(int index) -> const juce::String {
    juce::ValueTree child = properties->getChild(index);
    if (!child.isValid()) {
        return {};
    }
    return child.getType().toString();
}

//Adds a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpectronParameters::addListener(juce::ValueTree::Listener *listener, bool sendAllParametersForInitialisation) {
    properties->addListener(listener);

    //if selected, the already added listener will be informed about every parameter as if it had been changed
    if (sendAllParametersForInitialisation) {
        for (int i = 0; i < properties->getNumChildren(); i++) {
            juce::ValueTree parameter = properties->getChild(i);
            if (!parameter.isValid()) {
                continue;
            }
            listener->valueTreePropertyChanged(parameter, PROPERTY_VALUE);
        }
    }
}
//Removes a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpectronParameters::removeListener(juce::ValueTree::Listener *listener) {
    properties->removeListener(listener);
}

//Removes a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpectronParameters::readFromXML(const XmlElement &xml) {
    const ScopedLock myScopedLock(criticalSection);
    juce::ValueTree importedProperties = juce::ValueTree::fromXml(xml);

    //validate imported data (skip import if invalid)
    if (!importedProperties.isValid()) {
        return;
    }
    if (importedProperties.getNumChildren() < properties->getNumChildren()) {
        return;
    }

    //reads all currently defined parameters and overwrites their values,
    //if the same parameter is found in the imported ValueTree
    for (int i = 0; i < properties->getNumChildren(); i++) {
        //get current parameter, skip if invalid
        juce::ValueTree oldParameter = properties->getChild(i);
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
