#include "SpecletPluginProcessor.h"
#include "dsp/DspThread.h"
#include "dsp/SignalGenerator.h"
#include "dsp/transformations/TransformationFactory.h"
#include "juce_core/system/juce_PlatformDefs.h"
#include "parameter/SpecletParameters.h"
#include "ui/SpecletMainUI.h"
#include "utilities/PerformanceLogger.h"
#include <memory>
#include <string>


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

    if (dspThread != nullptr) {
        dspThread->stopThread(3000);
        dspThread.reset();
    }

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
    juce::ignoreUnused(newValue);
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
void SpecletAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Stop any existing DSP thread before resizing shared buffers
    if (dspThread != nullptr) {
        dspThread->stopThread(3000);
        dspThread.reset();
    }

    // Report input samples that were dropped during the previous session (FIFO full)
    const int dropped = droppedInputSamples.exchange(0, std::memory_order_relaxed);
    if (dropped > 0) {
        DBG("SpecletAudioProcessor: " + juce::String(dropped) + " input samples dropped (FIFO full) in last session");
    }

    // Allocate lock-free FIFO (capacity: larger of 16 blocks or 8192 samples)
    const int fifoSize = std::max(samplesPerBlock * 16, 8192);
    inputFifo.setTotalSize(fifoSize);
    inputCircularBuffer.resize(static_cast<std::size_t>(fifoSize));

    // Create and start the background DSP thread
    dspThread = std::make_unique<DspThread>(inputFifo, inputCircularBuffer);
    updateTransformation();
    dspThread->startThread();

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

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    if (dspThread == nullptr) {
        return;
    }

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

    // Write samples into the lock-free FIFO; drop silently if full
    int start1, size1, start2, size2;
    inputFifo.prepareToWrite(numSamples, start1, size1, start2, size2);
    for (int i = 0; i < size1; ++i) {
        inputCircularBuffer[static_cast<std::size_t>(start1 + i)] = getSampleFromRouting(inL + i, inR + i);
    }
    for (int i = 0; i < size2; ++i) {
        inputCircularBuffer[static_cast<std::size_t>(start2 + i)] = getSampleFromRouting(inL + size1 + i, inR + size1 + i);
    }
    inputFifo.finishedWrite(size1 + size2);
    if (size1 + size2 < numSamples) {
        droppedInputSamples.fetch_add(numSamples - size1 - size2, std::memory_order_relaxed);
    }

    dspThread->notifyNewData();
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
void SpecletAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    parameters.getStateInformation(destData);
}

void SpecletAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
    auto tree = juce::ValueTree::readFromData(data, static_cast<size_t>(sizeInBytes));
    parameters.setStateInformation(tree);
    if (tree.isValid()) {
        updateTransformation();
    }
}

//==============================================================================

void SpecletAudioProcessor::updateTransformation() {
    if (dspThread == nullptr) {
        return;
    }
    DBG("SpecletAudioProcessor::updateTransformation()");
    double sampleRate = (getSampleRate() <= 100) ? DEFAULT_SAMPLINGRATE : getSampleRate();

    TransformationRebuildParams params;
    params.sampleRate = sampleRate;
    params.transformationType = static_cast<TransformationParameters::Type>(parameters.getTransformation());
    params.resolution = parameters.getResolution();
    params.windowFunction = static_cast<WindowParameters::WindowFunction>(parameters.getWindowing());
    params.waveletBase = static_cast<WaveletParameters::WaveletBase>(parameters.getWavelet());
    params.waveletPacketBasis = static_cast<WaveletParameters::ResolutionRatioOption>(parameters.getWaveletPacketBasis());

    dspThread->requestTransformationRebuild(params);
}

void SpecletAudioProcessor::updateSignalGenerator() {
    double sampleRate = (getSampleRate() <= 100) ? DEFAULT_SAMPLINGRATE : getSampleRate();
    signalGenerator = SignalGenerator(sampleRate, static_cast<SignalGeneratorParameters::Waveform>(parameters.getGenerator()), parameters.getGeneratorFrequency());
}

auto SpecletAudioProcessor::getSampleFromRouting(const float *inL, const float *inR) -> float {
    switch (parameterRouting.load()) {
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