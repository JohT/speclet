/*
  ==============================================================================
  ==============================================================================
*/

#include "../plugin/SpectronAudioProcessor.h"
#include "../user interface/SpectronMainUI.h"
#include "../dsp/transformations/TransformationFactory.h"
#include "../utilities/PerformanceManager.h"
#include "../user interface/ColourGradients.h"

#define DEFAULT_SAMPLINGRATE 44100

//==============================================================================
SpectronAudioProcessor::SpectronAudioProcessor()
	: signalGenerator(0), currentTransformation(0), parameters(0)
{
	 //TODO height and width later for flexible resizing?
	 lastUIWidth = 800;
	 lastUIHeight = 360;
	 //lastPosInfo.resetToDefault();

	 #if _LOGTOFILE
		 juce::Logger::setCurrentLogger(new juce::FileLogger(juce::File("c:/temp/speclet.log"), "Speclet LogFile"), true);
	 #endif

	 //gets the pointer to the parameters singelton - for a better readability
	 parameters = SpectronParameters::getSingletonInstance();

	 //Initialize with default settings
	 parameters->setParameter(SpectronParameters::PARAMETER_INDEX_ColorMode, SpectronParameters::COLORMODE_DEFAULT);
	 parameters->setParameter(SpectronParameters::PARAMETER_INDEX_Generator, SpectronParameters::GENERATOR_DEFAULT);
	 parameters->setParameter(SpectronParameters::PARAMETER_INDEX_GeneratorFrequency, 1000.0);
	 parameters->setParameter(SpectronParameters::PARAMETER_INDEX_LogFrequency, SpectronParameters::PLOT_AXIS_DEFAULT);
	 parameters->setParameter(SpectronParameters::PARAMETER_INDEX_LogMagnitude, SpectronParameters::PLOT_AXIS_DEFAULT);
	 parameters->setParameter(SpectronParameters::PARAMETER_INDEX_Resolution, SpectronParameters::RESOLUTION_DEFAULT);
	 parameters->setParameter(SpectronParameters::PARAMETER_INDEX_Routing, SpectronParameters::ROUTING_MID);
	 parameters->setParameter(SpectronParameters::PARAMETER_INDEX_Transformation, SpectronParameters::TRANSFORM_DEFAULT);
	 parameters->setParameter(SpectronParameters::PARAMETER_INDEX_Wavelet, SpectronParameters::WAVELET_DEFAULT);
	 parameters->setParameter(SpectronParameters::PARAMETER_INDEX_WaveletPaketBase, SpectronParameters::RESOLUTION_RATIO_DEFAULT);
	 parameters->setParameter(SpectronParameters::PARAMETER_INDEX_Windowing, SpectronParameters::WINDOWING_DEFAULT);
	 
	 parameter_routing = parameters->getRouting();
	 //registeres itself as listener for parameter-changes
	 parameters->addListener(this);
	 DBG("SpectronAudioProcessor as parameter listener added");
	 LOG("SpectronAudioProcessor as parameter listener added");
}

SpectronAudioProcessor::~SpectronAudioProcessor()
{
	SpectronParameters::getSingletonInstance()->removeListener(this);
	DBG("SpectronAudioProcessor as parameter listener removed");
	LOG("SpectronAudioProcessor as parameter listener removed");

	currentTransformation = NULL;
	parameters = NULL;

	TransformationFactory::getSingletonInstance()->destruct();
	WindowFunctionsFactory::getSingletonInstance()->destruct();
	PerformanceManager::getSingletonInstance()->destruct();
	ColourGradients::getSingletonInstance()->destruct();
	SpectronParameters::getSingletonInstance()->destruct();
	
	#if _LOGTOFILE
		juce::Logger::setCurrentLogger(0, true);
	#endif

	deleteAndZero(signalGenerator);
}

//==============================================================================
int SpectronAudioProcessor::getNumParameters()
{
	return parameters->TOTAL_NUMBER_OF_PARAMS;
}

float SpectronAudioProcessor::getParameter (int index)
{
	 // This method will be called by the host, probably on the audio thread, so
	 // it's absolutely time-critical. Don't use critical sections or anything
	 // UI-related, or anything at all that may block in any way! 
	return parameters->getParameter(index);
}

void SpectronAudioProcessor::setParameter (int index, float newValue)
{
	 // This method will be called by the host, probably on the audio thread, so
	 // it's absolutely time-critical. Don't use critical sections or anything
	 // UI-related, or anything at all that may block in any way!
	parameters->setParameter(index, newValue);
}

const String SpectronAudioProcessor::getParameterName (int index)
{
	return parameters->getParameterName(index);
}

const String SpectronAudioProcessor::getParameterText (int index)
{
	 return String (getParameter (index), 2);
}

//This method is called when a parameter changes (listener)
void SpectronAudioProcessor::valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &changedProperty) {
	const ScopedLock myScopedLock (criticalSection);
	juce::String changedParameterName = treeWhosePropertyHasChanged.getType().toString();
	LOG("SpectronAudioProcessor::valueTreePropertyChanged: " + changedParameterName);

	if ( (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_RESOLUTION))
	||   (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_TRANSFORMATION))
	||   (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_WAVELET))
	||   (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_WAVELETPAKETBASE))
	||   (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_WINDOWING)) ) {
		updateTransformation();
	}
	if (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_ROUTING)) {
		parameter_routing = parameters->getRouting();
	}
	if ( (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_GENERATOR)) 
	||   (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_GENERATORFREQUENCY)) ) {
		updateSignalGenerator();
	}

	//TODO setParameterNotifyingHost
};

void SpectronAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// Here's an example of how you can use XML to make it easy and more robust:

	// Create an outer XML element..
	ScopedPointer<XmlElement> xml = parameters->writeToXML();

	// then use this helper function to stuff it into the binary blob and return it..
	copyXmlToBinary (*xml, destData);
}

void SpectronAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	 // You should use this method to restore your parameters from this memory block,
	 // whose contents will have been created by the getStateInformation() call.

	 // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
	ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

	 if (xmlState != 0) {
		 parameters->readFromXML(*xmlState);
	 }
}

//==============================================================================
void SpectronAudioProcessor::prepareToPlay (double sampleRate, int /*samplesPerBlock*/)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	// parameters->setParameter(SpectronParameters::PARAMETER_TRANSFORMATION, parameters->getTransformation());
	if (!currentTransformation) updateTransformation();
	if (!signalGenerator) updateSignalGenerator();
}

void SpectronAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

void SpectronAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{	
	const ScopedLock myScopedLock (criticalSection);
	parameters->blockParameterChanges();

	const int numSamples	= buffer.getNumSamples();
	const int numChannels	= getNumInputChannels();

	float*  inR = 0;
	float*  inL = 0;

	if (numChannels <= 0) return;
	if (numChannels == 1) {
		inR = buffer.getSampleData(0);
		inL = buffer.getSampleData(0);
	}
	if (numChannels >= 2) {
		inR = buffer.getSampleData(0);
		inL = buffer.getSampleData(1);
	}

	for (int s = 0; s < numSamples; ++s, ++inL, ++inR) {
		if (currentTransformation) currentTransformation->setNextInputSample(getSampleFromRouting(inL, inR));
	}
	// In case we have more outputs than inputs, we'll clear any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	for (int i = numChannels; i < numChannels; ++i) {
		buffer.clear (i, 0, numSamples);
	}

	parameters->unblockParameterChanges();
}

void SpectronAudioProcessor::updateTransformation() {
	const ScopedLock myScopedLock (criticalSection);
	LOG("SpectronAudioProcessor::updateTransformation()");
	parameters->blockParameterChanges();

	currentTransformation = 0;
	double sampleRate = (getSampleRate() <= 100) ? DEFAULT_SAMPLINGRATE : getSampleRate();
	
	TransformationFactory::getSingletonInstance()->createTransformation(
		parameters->getTransformation(), 
		sampleRate, 
		parameters->getResolution(),
		parameters->getWindowing(),
		parameters->getWavelet(),
		parameters->getWaveletPaketBase()
	);

	parameters->unblockParameterChanges();
	currentTransformation = TransformationFactory::getSingletonInstance()->getCurrentTransformation();
}

void SpectronAudioProcessor::updateSignalGenerator() {
	double sampleRate = (getSampleRate() <= 100) ? DEFAULT_SAMPLINGRATE : getSampleRate();
	deleteAndZero(signalGenerator);
	signalGenerator = new SignalGenerator(parameters->getGenerator(), parameters->getGeneratorFrequency(), sampleRate);
}

float SpectronAudioProcessor::getSampleFromRouting(float* inL, float* inR)  {
	switch (parameter_routing) {
		case SpectronParameters::ROUTING_SIDE:			return *inL - *inR;
		case SpectronParameters::ROUTING_MID:			return (*inL + *inR) / 2.0;
		case SpectronParameters::ROUTING_R:				return *inR;
		case SpectronParameters::ROUTING_L:				return *inL;
		case SpectronParameters::ROUTING_GENERATOR:	return (signalGenerator)? signalGenerator->getNextSample() : 0.0;
		default:													return (*inL + *inR) / 2.0;
	}
}
//==============================================================================

//==============================================================================
AudioProcessorEditor* SpectronAudioProcessor::createEditor()
{
	 return new SpectronMainUI (this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	 return new SpectronAudioProcessor();
}