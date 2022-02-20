#pragma once

#include <JuceHeader.h>
#include <array>

enum Channel {
    Left = 0,
    Right = 1
};

enum Slope {
    Slope_12dB,
    Slope_24dB,
    Slope_36dB,
    Slope_48dB
};

//==============================================================================

//Copied from https://github.com/matkatmusic/SimpleEQ/blob/master/src/PluginProcessor.h
template<typename T>
struct Fifo {
    void prepare(int numChannels, int numSamples) {
        static_assert(std::is_same_v<T, juce::AudioBuffer<float>>,
                      "prepare(numChannels, numSamples) should only be used when the Fifo is holding juce::AudioBuffer<float>");
        for (auto &buffer : buffers) {
            buffer.setSize(numChannels,
                           numSamples,
                           false,//clear everything?
                           true, //including the extra space?
                           true);//avoid reallocating if you can?
            buffer.clear();
        }
    }

    void prepare(size_t numElements) {
        static_assert(std::is_same_v<T, std::vector<float>>,
                      "prepare(numElements) should only be used when the Fifo is holding std::vector<float>");
        for (auto &buffer : buffers) {
            buffer.clear();
            buffer.resize(numElements, 0);
        }
    }

    auto push(const T &t) -> bool {
        auto write = fifo.write(1);
        if (write.blockSize1 > 0) {
            buffers[write.startIndex1] = t;
            return true;
        }

        return false;
    }

    auto pull(T &t) -> bool {
        auto read = fifo.read(1);
        if (read.blockSize1 > 0) {
            t = buffers[read.startIndex1];
            return true;
        }

        return false;
    }

    auto getNumAvailableForReading() const -> int {
        return fifo.getNumReady();
    }

private:
    static constexpr int Capacity = 30;
    std::array<T, Capacity> buffers;
    juce::AbstractFifo fifo{Capacity};
};

//Copied from https://github.com/matkatmusic/SimpleEQ/blob/master/src/PluginProcessor.h
template<typename BlockType>
struct SingleChannelSampleFifo {
    explicit SingleChannelSampleFifo(Channel ch) : channelToUse(ch) {
        prepared.set(false);
    }

    void update(const BlockType &buffer) {
        jassert(prepared.get());
        jassert(buffer.getNumChannels() > channelToUse);
        auto *channelPtr = buffer.getReadPointer(channelToUse);

        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            pushNextSampleIntoFifo(channelPtr[i]);
        }
    }

    void prepare(int bufferSize) {
        prepared.set(false);
        size.set(bufferSize);

        bufferToFill.setSize(1,         //channel
                             bufferSize,//num samples
                             false,     //keepExistingContent
                             true,      //clear extra space
                             true);     //avoid reallocating
        audioBufferFifo.prepare(1, bufferSize);
        fifoIndex = 0;
        prepared.set(true);
    }
    //==============================================================================
    auto getNumCompleteBuffersAvailable() const -> int { return audioBufferFifo.getNumAvailableForReading(); }
    auto isPrepared() const -> bool { return prepared.get(); }
    auto getSize() const -> int { return size.get(); }
    //==============================================================================
    auto getAudioBuffer(BlockType &buf) -> bool { return audioBufferFifo.pull(buf); }

private:
    Channel channelToUse;
    int fifoIndex = 0;
    Fifo<BlockType> audioBufferFifo;
    BlockType bufferToFill;
    juce::Atomic<bool> prepared = false;
    juce::Atomic<int> size = 0;

    void pushNextSampleIntoFifo(float sample) {
        if (fifoIndex == bufferToFill.getNumSamples()) {
            auto ok = audioBufferFifo.push(bufferToFill);

            juce::ignoreUnused(ok);

            fifoIndex = 0;
        }

        bufferToFill.setSample(0, fifoIndex, sample);
        ++fifoIndex;
    }
};

//==============================================================================

struct ChainSettings {
    float peakFrequency{0}, peakGainInDecibels{0}, peakQuality{1.0F};
    float lowCutFrequency{0}, highCutFrequency{0};

    Slope lowCutSlope{Slope::Slope_12dB}, highCutSlope{Slope::Slope_12dB};

    bool lowCutBypass{false}, peakBypass{false}, highCutBypass{false};
};

auto getChainSettings(const AudioProcessorValueTreeState& parameters) -> ChainSettings;

using Filter = juce::dsp::IIR::Filter<float>;
using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>; 
using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter>;

enum ChainPositions { LowCut, Peak, HighCut };

using Coefficients = Filter::CoefficientsPtr;
void updateCoefficients(Coefficients& old, const Coefficients& replacements);

auto makePeakFilter(const ChainSettings& chainSettings, double sampleRate) -> Coefficients;

template <int Index, typename ChainType, typename CoefficientType>
void update(ChainType &chain, const CoefficientType &coefficients) {
  updateCoefficients(chain.template get<Index>().coefficients,
                     coefficients[Index]);
  chain.template setBypassed<Index>(false);
}

template <typename ChainType, typename CoefficientType>
void updateCutFilter(ChainType &chain, const CoefficientType &cutCoefficients,
                     const Slope &slope) {
  chain.template setBypassed<0>(true);
  chain.template setBypassed<1>(true);
  chain.template setBypassed<2>(true);
  chain.template setBypassed<3>(true);

  // 12dB/octave = slope 0 = order 2
  // 24dB/octave = slope 1 = order 4
  // 36dB/octave = slope 2 = order 6
  // 48dB/octave = slope 3 = order 8
  switch (slope) {
  case Slope_48dB: {
    update<3>(chain, cutCoefficients);
  }
  case Slope_36dB: {
    update<2>(chain, cutCoefficients);
  }
  case Slope_24dB: {
    update<1>(chain, cutCoefficients);
  }
  case Slope_12dB: {
    update<0>(chain, cutCoefficients);
  }
  }
}

inline auto makeLowCutFilter(const ChainSettings& chainSettings, double sampleRate) {
  auto lowCutOrder = (chainSettings.lowCutSlope + 1) * 2;
  auto highestPossibleFrequency = static_cast<float>(sampleRate * 0.5) - 1;
  return dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(
      std::min<float>(chainSettings.lowCutFrequency, highestPossibleFrequency),
      sampleRate, lowCutOrder);
}

inline auto makeHighCutFilter(const ChainSettings& chainSettings, double sampleRate) {
  auto highCutOrder = (chainSettings.highCutSlope + 1) * 2;
  auto highestPossibleFrequency = static_cast<float>(sampleRate * 0.5) - 1;
  return dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(
      std::min<float>(chainSettings.highCutFrequency, highestPossibleFrequency),
      sampleRate, highCutOrder);
}

//==============================================================================
class SimpleEQAudioProcessor : public juce::AudioProcessor {
public:
    //==============================================================================
    SimpleEQAudioProcessor();
    ~SimpleEQAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    auto isBusesLayoutSupported (const BusesLayout& layouts) const -> bool override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    auto createEditor() -> juce::AudioProcessorEditor* override;
    auto hasEditor() const -> bool override;

    //==============================================================================
    auto getName() const -> const juce::String override;

    auto acceptsMidi() const -> bool override;
    auto producesMidi() const -> bool override;
    auto isMidiEffect() const -> bool override;
    auto getTailLengthSeconds() const -> double override;

    //==============================================================================
    auto getNumPrograms() -> int override;
    auto getCurrentProgram() -> int override;
    void setCurrentProgram (int index) override;
    auto getProgramName (int index) -> const juce::String override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState parameters {*this, nullptr, "Parameters", createParameterLayout()};

    using BlockType = juce::AudioBuffer<float>;
    SingleChannelSampleFifo<BlockType> leftChannelQueue{Channel::Left};
    SingleChannelSampleFifo<BlockType> rightChannelQueue{Channel::Right};

private:
    static auto createParameterLayout() -> juce::AudioProcessorValueTreeState::ParameterLayout;

    MonoChain leftChain, rightChain;

    void updatePeakFilter(const ChainSettings& chainSettings);
    void updateLowCutFilters(const ChainSettings& chainSettings);
    void updateHighCutFilters(const ChainSettings& chainSettings);

    void updateFilters();

    juce::dsp::Oscillator<float> oscillator;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleEQAudioProcessor)
};
