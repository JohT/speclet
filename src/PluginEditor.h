#pragma once

#include "PluginProcessor.h"
#include <cmath>
#include <vector>

// Copied (and slightly adapted) from https://github.com/matkatmusic/SimpleEQ/blob/master/Source/PluginEditor.h
enum FFTOrder {
    Order_2048 = 11,
    Order_4096 = 12,
    Order_8192 = 13
};

// Copied (and slightly adapted) from https://github.com/matkatmusic/SimpleEQ/blob/master/Source/PluginEditor.h
template<typename BlockType>
struct FFTDataGenerator {
    /**
     produces the FFT data from an audio buffer.
     */
    void produceFFTDataForRendering(const juce::AudioBuffer<float> &audioData, const float negativeInfinity) {
        const auto fftSize = getFFTSize();

        fftData.assign(fftData.size(), 0);
        const auto *readIndex = audioData.getReadPointer(0);
        std::copy(readIndex, readIndex + fftSize, fftData.begin());

        // first apply a windowing function to our data
        window->multiplyWithWindowingTable(fftData.data(), fftSize);// [1]

        // then render our FFT data..
        forwardFFT->performFrequencyOnlyForwardTransform(fftData.data());// [2]

        int numBins = std::rintl(fftSize / 2);

        //normalize the fft values.
        for (int i = 0; i < numBins; ++i) {
            auto v = fftData[i];
            if (!std::isinf(v) && !std::isnan(v)) {
                v /= float(numBins);
            } else {
                v = 0.0F;
            }
            fftData[i] = v;
        }

        //convert them to decibels
        for (int i = 0; i < numBins; ++i) {
            fftData[i] = juce::Decibels::gainToDecibels(fftData[i], negativeInfinity);
        }

        fftDataFifo.push(fftData);
    }

    void changeOrder(FFTOrder newOrder) {
        //when you change order, recreate the window, forwardFFT, fifo, fftData
        //also reset the fifoIndex
        //things that need recreating should be created on the heap via std::make_unique<>

        order = newOrder;
        auto fftSize = getFFTSize();

        forwardFFT = std::make_unique<juce::dsp::FFT>(order);
        window = std::make_unique<juce::dsp::WindowingFunction<float>>(fftSize, juce::dsp::WindowingFunction<float>::blackmanHarris);

        fftData.clear();
        fftData.resize(fftSize * 2, 0);

        fftDataFifo.prepare(fftData.size());
    }
    //==============================================================================
    [[nodiscard]] auto getFFTSize() const -> int { return 1 << order; }
    [[nodiscard]] auto getNumAvailableFFTDataBlocks() const -> int { return fftDataFifo.getNumAvailableForReading(); }
    //==============================================================================
    auto getFFTData(BlockType &fftData) -> bool { return fftDataFifo.pull(fftData); }

private:
    FFTOrder order;
    BlockType fftData;
    std::unique_ptr<juce::dsp::FFT> forwardFFT;
    std::unique_ptr<juce::dsp::WindowingFunction<float>> window;

    Fifo<BlockType> fftDataFifo;
};

// Copied (and slightly adapted) from https://github.com/matkatmusic/SimpleEQ/blob/master/Source/PluginEditor.h
template<typename PathType>
struct AnalyzerPathGenerator {
    /*
     converts 'renderData[]' into a juce::Path
     */
    void generatePath(const std::vector<float> &renderData,
                      juce::Rectangle<float> fftBounds,
                      int fftSize,
                      float binWidth,
                      float negativeInfinity) {
        auto top = fftBounds.getY();
        auto bottom = fftBounds.getHeight();
        auto width = fftBounds.getWidth();

        int numBins = fftSize / 2;

        PathType p;
        p.preallocateSpace(3 * static_cast<int>(fftBounds.getWidth()));

        auto map = [bottom, top, negativeInfinity](float v) {
            return juce::jmap(v,
                              negativeInfinity, 0.0F,
                              float(bottom + 10), top);
        };

        auto y = map(renderData[0]);

        //        jassert( !std::isnan(y) && !std::isinf(y) );
        if (std::isnan(y) || std::isinf(y)) {
            y = bottom;
        }

        p.startNewSubPath(0, y);

        const int pathResolution = 2;//you can draw line-to's every 'pathResolution' pixels.

        for (int binNum = 1; binNum < numBins; binNum += pathResolution) {
            y = map(renderData[binNum]);

            //            jassert( !std::isnan(y) && !std::isinf(y) );

            if (!std::isnan(y) && !std::isinf(y)) {
                auto binFreq = binNum * binWidth;
                auto normalizedBinX = juce::mapFromLog10(binFreq, 20.0F, 20000.0F);
                int binX = std::floor(normalizedBinX * width);
                p.lineTo(binX, y);
            }
        }

        pathFifo.push(p);
    }

    [[nodiscard]] auto getNumPathsAvailable() const -> int {
        return pathFifo.getNumAvailableForReading();
    }

    auto getPath(PathType &path) -> bool {
        return pathFifo.pull(path);
    }

private:
    Fifo<PathType> pathFifo;
};

//==============================================================================

struct SimpleEQLookAndFeel : juce::LookAndFeel_V4 {
    void drawRotarySlider(juce::Graphics &graphics, int x, int y, int width, int height, float sliderPosProportional,
                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) override;
    void drawToggleButton(juce::Graphics &graphics, juce::ToggleButton &button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};

struct RotarySlidersWithLabels : juce::Slider {
    RotarySlidersWithLabels(juce::RangedAudioParameter &newParamter, juce::String newUnitSuffix)
        : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
          parameter(&newParamter),
          unitSuffix(std::move(newUnitSuffix)) {
        setLookAndFeel(&lookAndFeel);
    }

    ~RotarySlidersWithLabels() override { setLookAndFeel(nullptr); }

    struct LabelPosition {
        float position;
        juce::String label;
    };

    juce::Array<LabelPosition> labels;

    void paint(juce::Graphics &graphics) override;
    auto getSliderBounds() const -> juce::Rectangle<int>;
    static int getTextHeight() { return 14; }
    auto getDisplayString() const -> juce::String;

private:
    SimpleEQLookAndFeel lookAndFeel;

    juce::RangedAudioParameter *parameter;
    juce::String unitSuffix;
};

struct PathProducer {

public:
    explicit PathProducer(SingleChannelSampleFifo<SimpleEQAudioProcessor::BlockType> &singleChannelQueue) : leftChannelQueue(&singleChannelQueue) {
        leftChannelFFTDataGenerator.changeOrder(FFTOrder::Order_2048);
        monoBuffer.setSize(1, leftChannelFFTDataGenerator.getFFTSize());
    }
    void process(juce::Rectangle<float> fftBounds, double sampleRate);
    auto getPath() -> juce::Path { return leftChannelFFTPath; }

private:
    SingleChannelSampleFifo<SimpleEQAudioProcessor::BlockType> *leftChannelQueue;

    juce::AudioBuffer<float> monoBuffer;

    FFTDataGenerator<std::vector<float>> leftChannelFFTDataGenerator;
    AnalyzerPathGenerator<juce::Path> leftChannelPathGenerator;
    juce::Path leftChannelFFTPath;
};

struct ResponseCurveComponent : juce::Component,
                                juce::AudioProcessorParameter::Listener,
                                juce::Timer {
public:
    explicit ResponseCurveComponent(SimpleEQAudioProcessor &);
    ~ResponseCurveComponent() override;

    void parameterValueChanged(int, float) override;
    void parameterGestureChanged(int, bool) override {}

    void timerCallback() override;

    void paint(juce::Graphics &graphics) override;
    void resized() override;

    void setEnabled(bool value) {
        shouldShowFFTAnalysis = value;
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleEQAudioProcessor &audioProcessor;

    juce::Atomic<bool> parametersChanged{false};

    MonoChain monoChain;

    void updateChain();

    juce::Image background;

    auto getRenderArea() const -> juce::Rectangle<int>;
    auto getAnalysisArea() const -> juce::Rectangle<int>;

    PathProducer leftChannelPathProducer, rightChannelPathProducer;

    bool shouldShowFFTAnalysis = true;
};

//==============================================================================

struct PowerButton : juce::ToggleButton {};
struct AnalyzerButton : juce::ToggleButton {

    void resized() override {
        auto bounds = getLocalBounds();
        auto insetRectangle = bounds.reduced(4).toFloat();

        randomPath.clear();

        Random random;
        auto insetBottom = insetRectangle.getBottom();
        randomPath.startNewSubPath(insetRectangle.getX(), insetBottom * random.nextFloat());

        for (auto x = insetRectangle.toNearestInt().getX() + 1; x < insetRectangle.toNearestInt().getRight(); x += 2) {
            randomPath.lineTo(static_cast<float>(x), insetBottom * random.nextFloat());
        }
    };

    juce::Path randomPath;
};

class SimpleEQAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    explicit SimpleEQAudioProcessorEditor(SimpleEQAudioProcessor &);
    ~SimpleEQAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &graphics) override;
    void resized() override;

  private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
      SimpleEQAudioProcessor &audioProcessor;

      RotarySlidersWithLabels peakFrequencySlider, peakGainSlider, peakQualitySlider;
      RotarySlidersWithLabels lowCutFrequencySlider, highCutFrequencySlider, lowCutSlopeSlider, highCutSlopeSlider;

      ResponseCurveComponent responseCurveComponent;

      using ParameterAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
      ParameterAttachment peakFrequencyAttachment, peakGainAttachment, peakQualityAttachment;
      ParameterAttachment lowCutFrequencyAttachment, highCutFrequencyAttachment, lowCutSlopeAttachment, highCutSlopeAttachment;

      PowerButton lowCutBypassButton, peakBypassButton, highCutBypassButton;
      AnalyzerButton analyzerEnableButton;

      using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
      ButtonAttachment lowCutBypassAttachment, peakBypassAttachment, highCutBypassAttachment, analyzerEnableAttachment;

      auto getComponents() -> std::vector<juce::Component *>;

      SimpleEQLookAndFeel lookAndFeel;

      JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleEQAudioProcessorEditor)
};
