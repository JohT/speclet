#include "PluginProcessor.h"
#include "dsp/transformations/TransformationFactory.h"
#include "ui/ColourGradients.h"
#include "ui/SpectronMainUI.h"
#include "utilities/PerformanceManager.h"
#include "dsp/windowing/WindowFunctionFactory.h"
#include <memory>

#define DEFAULT_SAMPLINGRATE 44100

//==============================================================================
SpectronAudioProcessor::SpectronAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                             .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                             .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                             ),
      parameters(SpectronParameters::getSingletonInstance()), parameterRouting(parameters->getRouting()), currentTransformation(nullptr), signalGenerator(nullptr) {

    //TODO height and width later for flexible resizing?
    lastUIWidth = 800;
    lastUIHeight = 360;
    //lastPosInfo.resetToDefault();

#if _LOGTOFILE
    juce::Logger::setCurrentLogger(new juce::FileLogger(juce::File("c:/temp/speclet.log"), "Speclet LogFile"), true);
#endif

    //gets the pointer to the parameters singelton - for a better readability

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

    //registers itself as listener for parameter-changes
    parameters->addListener(this, true);
    DBG("SpectronAudioProcessor as parameter listener added");
}

SpectronAudioProcessor::~SpectronAudioProcessor() {
    SpectronParameters::getSingletonInstance()->removeListener(this);
    DBG("SpectronAudioProcessor as parameter listener removed");
    LOG("SpectronAudioProcessor as parameter listener removed");

    currentTransformation = NULL;
    parameters = NULL;

    TransformationFactory::getSingletonInstance().destruct();
    SpectronParameters::getSingletonInstance()->destruct();

#if _LOGTOFILE
    juce::Logger::setCurrentLogger(0, true);
#endif

    deleteAndZero(signalGenerator);
}

//==============================================================================
auto SpectronAudioProcessor::getNumParameters() -> int {
    return parameters->TOTAL_NUMBER_OF_PARAMS;
}

auto SpectronAudioProcessor::getParameter(int index) -> float {
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    return parameters->getParameter(index);
}

void SpectronAudioProcessor::setParameter(int index, float newValue) {
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    parameters->setParameter(index, newValue);
}

auto SpectronAudioProcessor::getParameterName(int index) -> const String {
    return parameters->getParameterName(index);
}

auto SpectronAudioProcessor::getParameterText(int index) -> const String {
    return String(getParameter(index), 2);
}

//==============================================================================
auto SpectronAudioProcessor::getName() const -> const juce::String {
    return JucePlugin_Name;
}

auto SpectronAudioProcessor::acceptsMidi() const -> bool {
#if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

auto SpectronAudioProcessor::producesMidi() const -> bool {
#if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

auto SpectronAudioProcessor::isMidiEffect() const -> bool {
#if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

auto SpectronAudioProcessor::getTailLengthSeconds() const -> double {
    return 0.0;
}

auto SpectronAudioProcessor::getNumPrograms() -> int {
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

auto SpectronAudioProcessor::getCurrentProgram() -> int {
    return 0;
}

void SpectronAudioProcessor::setCurrentProgram(int index) {
    juce::ignoreUnused (index);
}

auto SpectronAudioProcessor::getProgramName(int index) -> const juce::String {
    juce::ignoreUnused (index);
    return {};
}

void SpectronAudioProcessor::changeProgramName(int index, const juce::String &newName) {
    juce::ignoreUnused (index, newName);
}

//This method is called when a parameter changes (listener)
void SpectronAudioProcessor::valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier &changedProperty) {
    const ScopedLock myScopedLock(criticalSection);
    juce::String changedParameterName = treeWhosePropertyHasChanged.getType().toString();
    LOG("SpectronAudioProcessor::valueTreePropertyChanged: " + changedParameterName);

    if ((changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_RESOLUTION))         //
        || (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_TRANSFORMATION))  //
        || (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_WAVELET))         //
        || (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_WAVELETPAKETBASE))//
        || (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_WINDOWING))) {
        updateTransformation();
    }
    if (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_ROUTING)) {
        parameterRouting = parameters->getRouting();
    }
    if ((changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_GENERATOR))//
        || (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_GENERATORFREQUENCY))) {
        updateSignalGenerator();
    }
};

//==============================================================================
void SpectronAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    //TODO(johnny) delete?
    // juce::dsp::ProcessSpec specification{};

    // specification.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    // specification.numChannels = 1;
    // specification.sampleRate = sampleRate;

    // leftChain.prepare(specification);
    // rightChain.prepare(specification);

    // updateFilters();

    // leftChannelQueue.prepare(samplesPerBlock);
    // rightChannelQueue.prepare(samplesPerBlock);

    // oscillator.initialise([](float x) { return std::sin(x); });

    // specification.numChannels = static_cast<unsigned int>(getTotalNumOutputChannels());
    // oscillator.prepare(specification);
    // oscillator.setFrequency(200);

    if (currentTransformation == nullptr) {
        updateTransformation();
    }
    if (signalGenerator == nullptr) {
        updateSignalGenerator();
    }
}

void SpectronAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

auto SpectronAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const -> bool {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()) {
        return false;
    }

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) {
        return false;
    }
   #endif

    return true;
  #endif
}

void SpectronAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                          juce::MidiBuffer &midiMessages) {
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    //TODO(johnny) delete?
    // updateFilters();

    // juce::dsp::AudioBlock<float> block(buffer);

    // // Oscillator for testing
    // // buffer.clear();
    // // juce::dsp::ProcessContextReplacing<float> stereoContext(block);
    // // oscillator.process(stereoContext);

    // auto leftBlock = block.getSingleChannelBlock(0);
    // auto rightBlock = block.getSingleChannelBlock(1);

    // juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    // juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    // leftChain.process(leftContext);
    // rightChain.process(rightContext);

    // leftChannelQueue.update(buffer);
    // rightChannelQueue.update(buffer);

    const ScopedLock myScopedLock(criticalSection);
    parameters->blockParameterChanges();

    const int numSamples = buffer.getNumSamples();
    const int numChannels = totalNumInputChannels;

    const float *inR = nullptr;
    const float *inL = nullptr;

    if (numChannels <= 0) {
        return;
    }
    if (numChannels == 1) {
        inR = buffer.getReadPointer(0);
        inL = buffer.getReadPointer(0);
    }
    if (numChannels >= 2) {
        inR = buffer.getReadPointer(0);
        inL = buffer.getReadPointer(1);
    }

    for (int s = 0; s < numSamples; ++s, ++inL, ++inR) {
        if (currentTransformation != nullptr) {
            currentTransformation->setNextInputSample(getSampleFromRouting(inL, inR));
        }
    }
    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = numChannels; i < numChannels; ++i) {
        buffer.clear(i, 0, numSamples);
    }

    parameters->unblockParameterChanges();
}

//==============================================================================
auto SpectronAudioProcessor::hasEditor() const -> bool {
    return true; // (change this to false if you choose to not supply an editor)
}

auto SpectronAudioProcessor::createEditor() -> juce::AudioProcessorEditor * {
    return new SpectronMainUI(*this);
    //return new SpectronAudioProcessorEditor(*this);
    // return new juce::GenericAudioProcessorEditor(*this); // generic editor for prototyping without GUI
}

//==============================================================================
void SpectronAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    // TODO(johnny): better use this method instead of xml below?
    //juce::MemoryOutputStream memoryOutputStream(destData, true);
    //oldParameters.state.writeToStream(memoryOutputStream);

    // Create an outer XML element..
    std::unique_ptr<XmlElement> xml = parameters->writeToXML();

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary(*xml, destData);
}

void SpectronAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // TODO(johnny): better use this method instead of xml below?
    //auto tree = juce::ValueTree::readFromData(data, static_cast<size_t>(sizeInBytes));
    //if (tree.isValid()) {
    //    oldParameters.replaceState(tree);
    //    updateFilters();
    //}

    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState) {
        parameters->readFromXML(*xmlState);
    }
}

//==============================================================================

void SpectronAudioProcessor::updateTransformation() {
    const ScopedLock myScopedLock(criticalSection);
    LOG("SpectronAudioProcessor::updateTransformation()");
    parameters->blockParameterChanges();

    currentTransformation = nullptr;
    double sampleRate = (getSampleRate() <= 100) ? DEFAULT_SAMPLINGRATE : getSampleRate();

    TransformationFactory::getSingletonInstance().createTransformation(
            parameters->getTransformation(),
            sampleRate,
            parameters->getResolution(),
            parameters->getWindowing(),
            parameters->getWavelet(),
            parameters->getWaveletPaketBase());

    parameters->unblockParameterChanges();
    currentTransformation = TransformationFactory::getSingletonInstance().getCurrentTransformation();
}

void SpectronAudioProcessor::updateSignalGenerator() {
    double sampleRate = (getSampleRate() <= 100) ? DEFAULT_SAMPLINGRATE : getSampleRate();
    deleteAndZero(signalGenerator);
    signalGenerator = new SignalGenerator(parameters->getGenerator(), parameters->getGeneratorFrequency(), sampleRate);
}

//TODO(johnny) switch to double or template for both?
auto SpectronAudioProcessor::getSampleFromRouting(const float *inL, const float *inR) -> float {
    switch (parameterRouting) {
        case SpectronParameters::ROUTING_SIDE:
            return *inL - *inR;
        case SpectronParameters::ROUTING_MID:
            return static_cast<float>((*inL + *inR) / 2.0);
        case SpectronParameters::ROUTING_R:
            return *inR;
        case SpectronParameters::ROUTING_L:
            return *inL;
        case SpectronParameters::ROUTING_GENERATOR:
            return (signalGenerator) != nullptr ? static_cast<float>(signalGenerator->getNextSample()) : 0.0F;
        default:
            return static_cast<float>((*inL + *inR) / 2.0);
    }
}

//==============================================================================
// This creates new instances of the plugin..
auto JUCE_CALLTYPE createPluginFilter() -> juce::AudioProcessor * {
    return new SpectronAudioProcessor();
}