#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <memory>
#include <algorithm>

//==============================================================================
SimpleEQAudioProcessor::SimpleEQAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                             .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                             .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ) {
}

SimpleEQAudioProcessor::~SimpleEQAudioProcessor() {
}

//==============================================================================
auto SimpleEQAudioProcessor::getName() const -> const juce::String {
    return JucePlugin_Name;
}

auto SimpleEQAudioProcessor::acceptsMidi() const -> bool {
#if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

auto SimpleEQAudioProcessor::producesMidi() const -> bool {
#if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

auto SimpleEQAudioProcessor::isMidiEffect() const -> bool {
#if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

auto SimpleEQAudioProcessor::getTailLengthSeconds() const -> double {
    return 0.0;
}

auto SimpleEQAudioProcessor::getNumPrograms() -> int {
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

auto SimpleEQAudioProcessor::getCurrentProgram() -> int {
    return 0;
}

void SimpleEQAudioProcessor::setCurrentProgram(int index) {
    juce::ignoreUnused (index);
}

auto SimpleEQAudioProcessor::getProgramName(int index) -> const juce::String {
    juce::ignoreUnused (index);
    return {};
}

void SimpleEQAudioProcessor::changeProgramName(int index, const juce::String &newName) {
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void SimpleEQAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec specification{};

    specification.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    specification.numChannels = 1;
    specification.sampleRate = sampleRate;

    leftChain.prepare(specification);
    rightChain.prepare(specification);

    updateFilters();

    leftChannelQueue.prepare(samplesPerBlock);
    rightChannelQueue.prepare(samplesPerBlock);

    oscillator.initialise([](float x) { return std::sin(x); });

    specification.numChannels = static_cast<unsigned int>(getTotalNumOutputChannels());
    oscillator.prepare(specification);
    oscillator.setFrequency(200);
}

void SimpleEQAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

auto SimpleEQAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const -> bool {
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

void SimpleEQAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
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
    
    updateFilters();
    
    juce::dsp::AudioBlock<float> block(buffer);

    // Oscillator for testing
    // buffer.clear();
    // juce::dsp::ProcessContextReplacing<float> stereoContext(block);
    // oscillator.process(stereoContext);

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    leftChain.process(leftContext);
    rightChain.process(rightContext);

    leftChannelQueue.update(buffer);
    rightChannelQueue.update(buffer);
}

//==============================================================================
auto SimpleEQAudioProcessor::hasEditor() const -> bool {
    return true; // (change this to false if you choose to not supply an editor)
}

auto SimpleEQAudioProcessor::createEditor() -> juce::AudioProcessorEditor * {
    return new SimpleEQAudioProcessorEditor(*this);
    // return new juce::GenericAudioProcessorEditor(*this); // generic editor for prototyping without GUI
}

//==============================================================================
void SimpleEQAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream memoryOutputStream(destData, true);
    parameters.state.writeToStream(memoryOutputStream);
}

void SimpleEQAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, static_cast<size_t>(sizeInBytes));
    if (tree.isValid()) {
        parameters.replaceState(tree);
        updateFilters();
    }
}

//==============================================================================
// This creates new instances of the plugin..
auto JUCE_CALLTYPE createPluginFilter() -> juce::AudioProcessor*
{
    return new SimpleEQAudioProcessor();
}

auto getChainSettings(const AudioProcessorValueTreeState &parameters) -> ChainSettings
{
    ChainSettings settings;

    settings.lowCutFrequency = parameters.getRawParameterValue("LowCut Frequency")->load();
    settings.highCutFrequency = parameters.getRawParameterValue("HighCut Frequency")->load();
    settings.peakFrequency = parameters.getRawParameterValue("Peak Frequency")->load();
    settings.peakGainInDecibels = parameters.getRawParameterValue("Peak Gain")->load();
    settings.peakQuality = parameters.getRawParameterValue("Peak Quality")->load();
    settings.lowCutSlope = static_cast<Slope>(parameters.getRawParameterValue("LowCut Slope")->load());
    settings.highCutSlope = static_cast<Slope>(parameters.getRawParameterValue("HighCut Slope")->load());

    settings.lowCutBypass = parameters.getRawParameterValue("LowCut Bypass")->load() > 0.5F;
    settings.highCutBypass = parameters.getRawParameterValue("HighCut Bypass")->load() > 0.5F;
    settings.peakBypass = parameters.getRawParameterValue("Peak Bypass")->load() > 0.5F;

    return settings;
}

void updateCoefficients(Coefficients &old, const Coefficients &replacements) {
    *old = *replacements;
}

auto makePeakFilter(const ChainSettings& chainSettings, double sampleRate) -> Coefficients {
    auto peakGain = juce::Decibels::decibelsToGain(chainSettings.peakGainInDecibels);
    return juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        sampleRate, 
        chainSettings.peakFrequency, 
        chainSettings.peakQuality,
        peakGain);
}

void SimpleEQAudioProcessor::updatePeakFilter(const ChainSettings &chainSettings) {
    auto peakCoefficients = makePeakFilter(chainSettings, getSampleRate());

    leftChain.setBypassed<ChainPositions::Peak>(chainSettings.peakBypass);
    rightChain.setBypassed<ChainPositions::Peak>(chainSettings.peakBypass);

    updateCoefficients(leftChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);
    updateCoefficients(rightChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);
}

void SimpleEQAudioProcessor::updateLowCutFilters(const ChainSettings &chainSettings) {
    auto lowCutCoefficients = makeLowCutFilter(chainSettings, getSampleRate());
    auto &leftLowCut = leftChain.get<ChainPositions::LowCut>();
    auto &rightLowCut = rightChain.get<ChainPositions::LowCut>();

    leftChain.setBypassed<ChainPositions::LowCut>(chainSettings.lowCutBypass);
    rightChain.setBypassed<ChainPositions::LowCut>(chainSettings.lowCutBypass);

    updateCutFilter(leftLowCut, lowCutCoefficients, chainSettings.lowCutSlope);
    updateCutFilter(rightLowCut, lowCutCoefficients, chainSettings.lowCutSlope);
}

void SimpleEQAudioProcessor::updateHighCutFilters(const ChainSettings &chainSettings) {
    auto highCutCoefficients = makeHighCutFilter(chainSettings, getSampleRate());
    auto& leftHighCut = leftChain.get<ChainPositions::HighCut>();
    auto& rightHighCut = rightChain.get<ChainPositions::HighCut>();

    leftChain.setBypassed<ChainPositions::HighCut>(chainSettings.highCutBypass);
    rightChain.setBypassed<ChainPositions::HighCut>(chainSettings.highCutBypass);

    updateCutFilter(leftHighCut, highCutCoefficients, chainSettings.highCutSlope);
    updateCutFilter(rightHighCut, highCutCoefficients, chainSettings.highCutSlope);
}

void SimpleEQAudioProcessor::updateFilters() {
    auto chainSettings = getChainSettings(parameters);
    updateLowCutFilters(chainSettings);
    updatePeakFilter(chainSettings);
    updateHighCutFilters(chainSettings);
}

auto SimpleEQAudioProcessor::createParameterLayout() -> juce::AudioProcessorValueTreeState::ParameterLayout {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    const auto minFilterFrequency = 20.0F;
    const auto maxFilterFrequency = 20000.0F;
    const auto defaultPeakFilterFrequency = 750.0F;

    const auto filterGainLimit = 24.0F;
    const auto minFilterQuality = 0.1F;
    const auto maxFilterQuality = 10.0F;

    const auto frequencyStepSize = 1.0F;
    const auto gainStepSize = 0.5F;
    const auto qualityStepSize = 0.05F;

    const auto frequencySkewFactor = 0.25F;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
            "LowCut Frequency", "LowCut Frequency",
            juce::NormalisableRange<float>(minFilterFrequency, maxFilterFrequency, frequencyStepSize, frequencySkewFactor),
            minFilterFrequency));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
            "HighCut Frequency", "HighCut Frequency",
            juce::NormalisableRange<float>(minFilterFrequency, maxFilterFrequency, 1.0F, frequencySkewFactor),
            maxFilterFrequency));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
            "Peak Frequency", "Peak Frequency",
            juce::NormalisableRange<float>(minFilterFrequency, maxFilterFrequency, 1.0F, frequencySkewFactor),
            defaultPeakFilterFrequency));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
            "Peak Gain", "Peak Gain",
            juce::NormalisableRange<float>(-filterGainLimit, filterGainLimit, gainStepSize, 1.0F),
            0.0F));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
            "Peak Quality", "Peak Quality",
            juce::NormalisableRange<float>(minFilterQuality, maxFilterQuality, qualityStepSize, 1.0F),
            1.0F));

    juce::StringArray filterSlopeOptions = {"12 dB/Octave", "24 dB/Octave", "36 dB/Octave", "48 dB/Octave"};
    layout.add(std::make_unique<juce::AudioParameterChoice>("LowCut Slope", "LowCut Slope", filterSlopeOptions, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("HighCut Slope", "HighCut Slope", filterSlopeOptions, 0));

    layout.add(std::make_unique<juce::AudioParameterBool>("LowCut Bypass", "LowCut Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("Peak Bypass", "Peak Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("HighCut Bypass", "HighCut Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("Analyzer Enable", "Analyzer Enable", true));

    return layout;
}