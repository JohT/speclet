#include "SpectronParameters.h"
#include "..\utilities\PerformanceManager.h"

// Singleton instance variable (only one instance of this class) 
SpectronParameters* SpectronParameters::singletonInstance = 0;

const juce::String SpectronParameters::PROPERTY_VALUE						= "value";
const juce::String SpectronParameters::PARAMETER_COLORMODE				= "colormode";
const juce::String SpectronParameters::PARAMETER_LOGMAGNITUDE			= "logmagnitude";
const juce::String SpectronParameters::PARAMETER_LOGFREQUENCY			= "logfrequency";
const juce::String SpectronParameters::PARAMETER_RESOLUTION				= "resolution";
const juce::String SpectronParameters::PARAMETER_ROUTING					= "routing";
const juce::String SpectronParameters::PARAMETER_GENERATOR				= "generator";
const juce::String SpectronParameters::PARAMETER_GENERATORFREQUENCY	= "generatorfrequency";
const juce::String SpectronParameters::PARAMETER_TRANSFORMATION		= "transform";
const juce::String SpectronParameters::PARAMETER_WAVELET					= "wavelet";
const juce::String SpectronParameters::PARAMETER_WAVELETPAKETBASE		= "waveletpaketbase";
const juce::String SpectronParameters::PARAMETER_WINDOWING				= "windowing";

SpectronParameters::SpectronParameters(void) {
	//create ValueTree object, which stores all parameters as childs in a tree structure 
	
	properties = new juce::ValueTree("SpectronParameters");

	//add parameters as tree childs (must start with zero and use ascending indizes -> enum)
	properties->addChild(juce::ValueTree(PARAMETER_ROUTING),					PARAMETER_INDEX_Routing, NULL);
	properties->addChild(juce::ValueTree(PARAMETER_TRANSFORMATION),		PARAMETER_INDEX_Transformation, NULL);
	properties->addChild(juce::ValueTree(PARAMETER_RESOLUTION),				PARAMETER_INDEX_Resolution, NULL);
	properties->addChild(juce::ValueTree(PARAMETER_WAVELETPAKETBASE),		PARAMETER_INDEX_WaveletPaketBase, NULL);
	properties->addChild(juce::ValueTree(PARAMETER_WINDOWING),				PARAMETER_INDEX_Windowing, NULL);
	properties->addChild(juce::ValueTree(PARAMETER_WAVELET),					PARAMETER_INDEX_Wavelet, NULL);
	properties->addChild(juce::ValueTree(PARAMETER_GENERATOR),				PARAMETER_INDEX_Generator, NULL);
	properties->addChild(juce::ValueTree(PARAMETER_GENERATORFREQUENCY),	PARAMETER_INDEX_GeneratorFrequency, NULL);
	properties->addChild(juce::ValueTree(PARAMETER_LOGFREQUENCY),			PARAMETER_INDEX_LogFrequency, NULL);
	properties->addChild(juce::ValueTree(PARAMETER_LOGMAGNITUDE),			PARAMETER_INDEX_LogMagnitude, NULL);
	properties->addChild(juce::ValueTree(PARAMETER_COLORMODE),				PARAMETER_INDEX_ColorMode, NULL);
	
	//add value zero to every tree child's value property
	for (int i = 0; i < properties->getNumChildren(); i++) {
		properties->getChild(i).setProperty(PROPERTY_VALUE, (float)0, NULL);
	}

	waitForParameterChange = new juce::WaitableEvent(true);
	waitForParameterChange->signal();
}

SpectronParameters::~SpectronParameters() {
}

SpectronParameters* SpectronParameters::getSingletonInstance() {
// Method to get the single instance of this class (Singleton)
	if (SpectronParameters::singletonInstance == 0) {
		SpectronParameters::singletonInstance = new SpectronParameters();
	}
	return SpectronParameters::singletonInstance;
}

void SpectronParameters::destruct() {
	if (!singletonInstance) return;

	delete(singletonInstance);
	singletonInstance = 0;
}

float	SpectronParameters::getParameter(int index) {
	juce::ValueTree child = properties->getChild(index);
	if (!child.isValid()) return (float)0;
	return child.getProperty(PROPERTY_VALUE);
}

float	SpectronParameters::getParameter(juce::String name) {
	juce::ValueTree child = properties->getChildWithName(name);
	if (!child.isValid()) return (float)0;
	return child.getProperty(PROPERTY_VALUE, (float)0);
}

void SpectronParameters::setParameter(int index, float newValue) {
	const ScopedLock myScopedLock (criticalSection);

	PerformanceManager::getSingletonInstance()->start("waitForParameterChange");
	bool timeoutDuringWait = waitForParameterChange->wait(TIMEOUT_WAIT_BEFORE_SET);
	PerformanceManager::getSingletonInstance()->stop("waitForParameterChange");
	if (!timeoutDuringWait) DBG("SpectronParameters::setParameter: Timeout during wait!");

	juce::ValueTree child = properties->getChild(index);
	if (!child.isValid()) return;
	child.setProperty(PROPERTY_VALUE, newValue, NULL);
}

void SpectronParameters::setParameter(juce::String name, float newValue) {
	const ScopedLock myScopedLock (criticalSection);

	PerformanceManager::getSingletonInstance()->start("waitForParameterChange");
	bool timeoutDuringWait = waitForParameterChange->wait(TIMEOUT_WAIT_BEFORE_SET);
	PerformanceManager::getSingletonInstance()->stop("waitForParameterChange");
	if (!timeoutDuringWait) DBG("SpectronParameters::setParameter: Timeout during wait!");

	juce::ValueTree child = properties->getChildWithName(name);
	if (!child.isValid()) return;
	child.setProperty(PROPERTY_VALUE, newValue, NULL);
}

int SpectronParameters::getParameterIndex(juce::String name) {
	juce::ValueTree child = properties->getChildWithName(name);
	if (!child.isValid()) return -1;
	return properties->indexOf(child);
}

juce::String SpectronParameters::getParameterName(int index) {
	juce::ValueTree child = properties->getChild(index);
	if (!child.isValid()) return juce::String::empty;
	return child.getType().toString();
}

//Adds a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpectronParameters::addListener (juce::ValueTree::Listener* listener, bool sendAllParametersForInitialisation) {
	properties->addListener(listener);
	
	//if selected, the already added listener will be informed about every parameter as if it had been changed 
	if (sendAllParametersForInitialisation) {
		for (int i = 0; i < properties->getNumChildren(); i++) {
			juce::ValueTree parameter = properties->getChild(i);
			if (!parameter.isValid()) continue;
			listener->valueTreePropertyChanged(parameter, PROPERTY_VALUE);
		}
	}
}
//Removes a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpectronParameters::removeListener (juce::ValueTree::Listener* listener) {
	properties->removeListener(listener);
}

//Removes a listener by delegating it to juce::ValueTree (see juce API documentation)
void SpectronParameters::readFromXML(const XmlElement &xml) {
	const ScopedLock myScopedLock (criticalSection);
	juce::ValueTree importedProperties = juce::ValueTree::fromXml(xml);

	//validate imported data (skip import if invalid)
	if (!importedProperties.isValid())	return;
	if (importedProperties.getNumChildren() < properties->getNumChildren()) return;

	//reads all currently defined parameters and overwrites their values,
	//if the same parameter is found in the imported ValueTree
	for (int i = 0; i < properties->getNumChildren(); i++) {
		//get current parameter, skip if invalid
		juce::ValueTree	old_parameter = properties->getChild(i);
		if (!old_parameter.isValid()) continue;

		//get new parameter with same name, skip if invalid
		juce::Identifier	paramter_identifier = old_parameter.getType();
		juce::ValueTree	new_parameter = importedProperties.getChildWithName(paramter_identifier);
		if (!new_parameter.isValid()) continue;

		//overwrite old parameter using the public setter, which will lead to listener notification
		juce::var			new_parameter_value = new_parameter.getProperty(PROPERTY_VALUE);
		setParameter(paramter_identifier.toString(), (float)new_parameter_value);
	}
}
