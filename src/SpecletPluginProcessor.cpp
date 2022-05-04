#include "SpecletPluginProcessor.h"
#include "dsp/SignalGenerator.h"
#include "dsp/transformations/AbstractWaveletTransformation.h"
#include "dsp/transformations/TransformationFactory.h"
#include "juce_core/system/juce_PlatformDefs.h"
#include "plugin/SpecletParameters.h"
#include "ui/ColourGradients.h"
#include "ui/SpecletMainUI.h"
#include "utilities/PerformanceLogger.h"
#include <memory>
#include <string>
#include <type_traits>


#define DEFAULT_SAMPLINGRATE 44100

//==============================================================================
SpecletAudioProcessor::SpecletAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                             .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                             .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                             ),
      parameters(*this),
      parameterRouting(parameters.getRouting()) {

    LOG_PERFORMANCE_BEGIN("SpecletAudioProcessor");
#if _LOGTOFILE
    juce::Logger::setCurrentLogger(new juce::FileLogger(juce::File("c:/temp/speclet.log"), "Speclet LogFile"), true);
#endif

    //registers itself as a listener for parameter-changes
    parameters.addListener(this);
    DBG("SpecletAudioProcessor as parameter listener added");
}

SpecletAudioProcessor::~SpecletAudioProcessor() {
    parameters.removeListener(this);
    DBG("SpecletAudioProcessor as parameter listener removed");
    currentTransformation = nullptr;

    TransformationFactory::getSingletonInstance().destruct();

#if _LOGTOFILE
    juce::Logger::setCurrentLogger(0, true);
#endif
    LOG_PERFORMANCE_END();
}

//==============================================================================
auto SpecletAudioProcessor::getName() const -> const juce::String {
    return JucePlugin_Name;
}

auto SpecletAudioProcessor::acceptsMidi() const -> bool {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

auto SpecletAudioProcessor::producesMidi() const -> bool {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

auto SpecletAudioProcessor::isMidiEffect() const -> bool {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

auto SpecletAudioProcessor::getTailLengthSeconds() const -> double {
    return 0.0;
}

auto SpecletAudioProcessor::getNumPrograms() -> int {
    return 1;// NB: some hosts don't cope very well if you tell them there are 0 programs,
             // so this should be at least 1, even if you're not really implementing programs.
}

auto SpecletAudioProcessor::getCurrentProgram() -> int {
    return 0;
}

void SpecletAudioProcessor::setCurrentProgram(int index) {
    juce::ignoreUnused(index);
}

auto SpecletAudioProcessor::getProgramName(int index) -> const juce::String {
    juce::ignoreUnused(index);
    return {};
}

void SpecletAudioProcessor::changeProgramName(int index, const juce::String &newName) {
    juce::ignoreUnused(index, newName);
}

void SpecletAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue) {
    const juce::ScopedLock myScopedLock(criticalSection);
    DBG("SpecletAudioProcessor::parameterChanged: " + parameterID);

    if (SpecletParameters::isTransformationParameter(parameterID)) {
        updateTransformation();
    }
    if (parameterID.equalsIgnoreCase(SpecletParameters::PARAMETER_ROUTING)) {
        parameterRouting = parameters.getRouting();
    }
    if ((parameterID.equalsIgnoreCase(SpecletParameters::PARAMETER_GENERATOR))//
        || (parameterID.equalsIgnoreCase(SpecletParameters::PARAMETER_GENERATORFREQUENCY))) {
        updateSignalGenerator();
    }
}

//==============================================================================
void SpecletAudioProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    if (currentTransformation == nullptr) {
        updateTransformation();
    }
    if (signalGenerator.getSamplingRate() != sampleRate) {
        signalGenerator = SignalGenerator(sampleRate, static_cast<SignalGeneratorParameters::Waveform>(parameters.getGenerator()), parameters.getGeneratorFrequency());
    }
}

void SpecletAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

auto SpecletAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const -> bool {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()) {
        return false;
    }

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) {
        return false;
    }
#endif

    return true;
#endif
}

void SpecletAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                         juce::MidiBuffer &midiMessages) {
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    const juce::ScopedLock myScopedLock(criticalSection);
    parameters.blockParameterChanges();

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

    parameters.unblockParameterChanges();
}

//==============================================================================
auto SpecletAudioProcessor::hasEditor() const -> bool {
    return true;// (change this to false if you choose to not supply an editor)
}

auto SpecletAudioProcessor::createEditor() -> juce::AudioProcessorEditor * {
    return new SpecletMainUI(*this);
    // return new juce::GenericAudioProcessorEditor(*this); // generic editor for prototyping without GUI
}

//==============================================================================
//TODO (JohT) Move to SpecletParameters
void SpecletAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream memoryOutputStream(destData, true);
    parameters.getParameters().state.writeToStream(memoryOutputStream);
}

void SpecletAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, static_cast<size_t>(sizeInBytes));
    if (tree.isValid()) {
        parameters.getParameters().replaceState(tree);
        if (currentTransformation == nullptr) {
            updateTransformation();
        }
    }
}

//==============================================================================

void SpecletAudioProcessor::updateTransformation() {
    const juce::ScopedLock myScopedLock(criticalSection);
    DBG("SpecletAudioProcessor::updateTransformation()");
    parameters.blockParameterChanges();

    currentTransformation = nullptr;
    double sampleRate = (getSampleRate() <= 100) ? DEFAULT_SAMPLINGRATE : getSampleRate();

    currentTransformation = TransformationFactory::getSingletonInstance().createTransformation(
            static_cast<TransformationParameters::Type>(parameters.getTransformation()),
            sampleRate,
            parameters.getResolution(),
            static_cast<WindowParameters::WindowFunction>(parameters.getWindowing()),
            static_cast<WaveletParameters::WaveletBase>(parameters.getWavelet()),
            static_cast<WaveletParameters::ResolutionRatioOption>(parameters.getWaveletPacketBasis()));

    parameters.unblockParameterChanges();
}

void SpecletAudioProcessor::updateSignalGenerator() {
    double sampleRate = (getSampleRate() <= 100) ? DEFAULT_SAMPLINGRATE : getSampleRate();
    signalGenerator = SignalGenerator(sampleRate, static_cast<SignalGeneratorParameters::Waveform>(parameters.getGenerator()), parameters.getGeneratorFrequency());
}

auto SpecletAudioProcessor::getSampleFromRouting(const float *inL, const float *inR) -> float {
    switch (parameterRouting) {
        case SpecletParameters::ROUTING_SIDE:
            return *inL - *inR;
        case SpecletParameters::ROUTING_MID:
            return static_cast<float>((*inL + *inR) / 2.0);
        case SpecletParameters::ROUTING_RIGHT:
            return *inR;
        case SpecletParameters::ROUTING_LEFT:
            return *inL;
        case SpecletParameters::ROUTING_GENERATOR:
            return static_cast<float>(signalGenerator.getNextSample());
        default:
            return static_cast<float>((*inL + *inR) / 2.0);
    }
}

//==============================================================================
// This creates new instances of the plugin..
auto JUCE_CALLTYPE createPluginFilter() -> juce::AudioProcessor * {
    return new SpecletAudioProcessor();
}