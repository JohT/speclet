#include "PluginEditor.h"
#include "PluginProcessor.h"
#include <ratio>
#include <vector>


void SimpleEQLookAndFeel::drawRotarySlider(
        Graphics &graphics,
        int x,
        int y,
        int width,
        int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        Slider &slider) {

    jassert(rotaryStartAngle < rotaryEndAngle);

    using namespace juce;

    auto bounds = Rectangle<float>(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height));
    auto enabled = slider.isEnabled();

    graphics.setColour(enabled ? Colour(18, 10, 90) : Colours::darkgrey);
    graphics.fillEllipse(bounds);

    graphics.setColour(enabled ? Colour(95, 75, 255) : Colours::grey);
    graphics.drawEllipse(bounds, 2.F);

    if (auto *rotarySliderWithLabels = dynamic_cast<RotarySlidersWithLabels *>(&slider)) {
        auto center = bounds.getCentre();

        Path path;

        auto sliderTextHeight = static_cast<float>(RotarySlidersWithLabels::getTextHeight());

        Rectangle<float> rectangle;
        rectangle.setLeft(center.getX() - 2);
        rectangle.setRight(center.getX() + 2);
        rectangle.setTop(bounds.getY());
        rectangle.setBottom(center.toFloat().getY() - sliderTextHeight * 1.5F);

        path.addRoundedRectangle(rectangle, 2.F);
        path.addRectangle(rectangle);

        auto sliderAngleRadiants = jmap(sliderPosProportional, 0.F, 1.F, rotaryStartAngle, rotaryEndAngle);

        path.applyTransform(AffineTransform().rotated(sliderAngleRadiants, center.getX(), center.getY()));

        graphics.fillPath(path);

        graphics.setFont(sliderTextHeight);
        auto text = rotarySliderWithLabels->getDisplayString();
        auto stringWidth = static_cast<float>(graphics.getCurrentFont().getStringWidth(text));

        rectangle.setSize(stringWidth + 4, sliderTextHeight + 2);
        rectangle.setCentre(bounds.getCentre());

        graphics.setColour(enabled ? Colours::black : Colours::darkgrey);
        graphics.fillRect(rectangle);

        graphics.setColour(enabled ? Colours::white : Colours::lightgrey);
        graphics.drawFittedText(text, rectangle.toNearestInt(), Justification::centred, 1);
    }
}

void SimpleEQLookAndFeel::drawToggleButton(juce::Graphics &graphics, juce::ToggleButton &toggleButton, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
    using namespace juce;

    if (auto *powerButton = dynamic_cast<PowerButton *>(&toggleButton)) {
        Path powerButtonPath;

        auto bounds = toggleButton.getLocalBounds();
        auto size = juce::jmin(bounds.getWidth(), bounds.getHeight()) - 6;
        auto rectangle = bounds.withSizeKeepingCentre(size, size).toFloat();

        float const angle = 30.0F;

        size -= 6;

        powerButtonPath.addCentredArc(
                rectangle.getCentreX(),
                rectangle.getCentreY(),
                static_cast<float>(size * 0.5),
                static_cast<float>(size * 0.5),
                0.F,
                degreesToRadians(angle),
                degreesToRadians(360.0F - angle),
                true);
        powerButtonPath.startNewSubPath(rectangle.getCentreX(), rectangle.getY());
        powerButtonPath.lineTo(rectangle.getCentre());

        PathStrokeType curvedStrokeType(2.0F, PathStrokeType::JointStyle::curved);
        auto color = toggleButton.getToggleState() ? Colours::dimgrey : Colour(95, 75, 255);
        graphics.setColour(color);
        graphics.strokePath(powerButtonPath, curvedStrokeType);
        graphics.drawEllipse(rectangle, 2.0F);
    } else if (auto *analyzerButton = dynamic_cast<AnalyzerButton *>(&toggleButton)) {
        auto color = !toggleButton.getToggleState() ? Colours::dimgrey : Colour(95, 75, 255);
        graphics.setColour(color);

        auto bounds = toggleButton.getLocalBounds();
        graphics.drawRect(bounds);

        graphics.strokePath(analyzerButton->randomPath, PathStrokeType(1.0F));
    }
}

//==============================================================================

void RotarySlidersWithLabels::paint(juce::Graphics &graphics) {
    using namespace juce;

    auto startAngle = degreesToRadians(180.0f + 45.0f);
    auto endAngle = degreesToRadians(180.0f - 45.0f) + MathConstants<float>::twoPi;

    auto range = getRange();

    auto sliderBounds = getSliderBounds();

    // Helper rectangles for layout design purposes:
    // graphics.setColour(Colours::red);
    // graphics.drawRect(getLocalBounds());
    // graphics.setColour(Colours::yellow);
    // graphics.drawRect(sliderBounds);

    auto sliderPosProportional = static_cast<float>(jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0));
    getLookAndFeel().drawRotarySlider(
            graphics,
            sliderBounds.getX(),
            sliderBounds.getY(),
            sliderBounds.getWidth(),
            sliderBounds.getHeight(),
            sliderPosProportional,
            startAngle,
            endAngle,
            *this);

    auto center = sliderBounds.toFloat().getCentre();
    auto radius = sliderBounds.toFloat().getWidth() * 0.5F;
    auto textHeight = static_cast<float>(getTextHeight());

    graphics.setColour(Colour(95, 75, 255));
    graphics.setFont(textHeight);

    auto numberOfChoices = labels.size();
    for (int i = 0; i < numberOfChoices; ++i) {
        auto position = labels[i].position;
        jassert(0.0F <= position);
        jassert(1.0F >= position);

        auto angle = jmap(position, 0.0F, 1.0F, startAngle, endAngle);

        auto labelCenter = center.getPointOnCircumference(radius + textHeight * 0.5F + 1, angle);

        Rectangle<float> labelRectangle;
        auto string = labels[i].label;
        labelRectangle.setSize(static_cast<float>(graphics.getCurrentFont().getStringWidth(string)), textHeight);
        labelRectangle.setCentre(labelCenter);
        labelRectangle.setY(labelRectangle.getY() + textHeight);

        graphics.drawFittedText(string, labelRectangle.toNearestInt(), Justification::centred, 1);
    }
}

juce::Rectangle<int> RotarySlidersWithLabels::getSliderBounds() const {
    auto bounds = getLocalBounds();

    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());
    size -= getTextHeight() * 2;

    juce::Rectangle<int> rectangle;
    rectangle.setSize(size, size);
    rectangle.setCentre(bounds.getCentreX(), 0);
    rectangle.setY(2);

    return rectangle;
}

juce::String RotarySlidersWithLabels::getDisplayString() const {

    if (auto *choiceParameter = dynamic_cast<juce::AudioParameterChoice *>(parameter)) {
        return choiceParameter->getCurrentChoiceName();
    }

    juce::String string;
    bool inKiloHertz = false;

    if (auto *choiceParameter = dynamic_cast<juce::AudioParameterFloat *>(parameter)) {
        auto value = getValue();

        if (value > 999.0f) {
            value /= 1000.0f;
            inKiloHertz = true;
        }

        string = juce::String(value, inKiloHertz ? 2 : 0);
    } else {
        jassertfalse;
    }

    if (unitSuffix.isNotEmpty()) {
        string << " ";
        if (inKiloHertz) {
            string << "k";
        }
        string << unitSuffix;
    }

    return string;
}

//==============================================================================

ResponseCurveComponent::ResponseCurveComponent(SimpleEQAudioProcessor &referencedAudioProcessor) : audioProcessor(referencedAudioProcessor),
                                                                                                   leftChannelPathProducer(referencedAudioProcessor.leftChannelQueue),
                                                                                                   rightChannelPathProducer(referencedAudioProcessor.rightChannelQueue) {
    const auto &parameters = referencedAudioProcessor.getParameters();
    for (auto *parameter : parameters) {
        parameter->addListener(this);
    }

    updateChain();
    startTimerHz(60);
}

ResponseCurveComponent::~ResponseCurveComponent()
{
    const auto& parameters = audioProcessor.getParameters(); 
    for (auto *parameter : parameters) {
        parameter->removeListener(this);
    }
}

void ResponseCurveComponent::parameterValueChanged(int, float) {
    parametersChanged.set(true);
}

void PathProducer::process(juce::Rectangle<float> fftBounds, double sampleRate) {
    juce::AudioBuffer<float> tempIncomingBuffer;
    const auto lowestLevelToAnalyzeInDB = -48.0F;

    while (leftChannelQueue->getNumCompleteBuffersAvailable() > 0) {
        if (leftChannelQueue->getAudioBuffer(tempIncomingBuffer)) {
            auto size = tempIncomingBuffer.getNumSamples();
            auto sizeToMove = monoBuffer.getNumSamples() - size;

            juce::FloatVectorOperations::copy(monoBuffer.getWritePointer(0, 0),
                                              monoBuffer.getReadPointer(0, size),
                                              sizeToMove);
            juce::FloatVectorOperations::copy(monoBuffer.getWritePointer(0, sizeToMove),
                                              tempIncomingBuffer.getReadPointer(0, 0),
                                              size);
            leftChannelFFTDataGenerator.produceFFTDataForRendering(monoBuffer, lowestLevelToAnalyzeInDB);
        }
    }
    const auto fftSize = leftChannelFFTDataGenerator.getFFTSize();
    const auto fftBinWidth = sampleRate / static_cast<double>(fftSize);

    while (leftChannelFFTDataGenerator.getNumAvailableFFTDataBlocks() > 0) {
        std::vector<float> fftData;
        if (leftChannelFFTDataGenerator.getFFTData(fftData)) {
            leftChannelPathGenerator.generatePath(fftData, fftBounds, fftSize, static_cast<float>(fftBinWidth), lowestLevelToAnalyzeInDB);
        }
    }

    // while there are patchs that can be pulled then pull as many as we can and display the most recent path
    while (leftChannelPathGenerator.getNumPathsAvailable() > 0) {
        leftChannelPathGenerator.getPath(leftChannelFFTPath);
    }
}

void ResponseCurveComponent::timerCallback() {

    if (shouldShowFFTAnalysis) {
        auto fftBounds = getAnalysisArea().toFloat();
        auto sampleRate = audioProcessor.getSampleRate();
        leftChannelPathProducer.process(fftBounds, sampleRate);
        rightChannelPathProducer.process(fftBounds, sampleRate);
    }

    if (parametersChanged.compareAndSetBool(false, true)) {
        updateChain();
    }
    repaint();
}

void ResponseCurveComponent::updateChain() {
    auto chainSettings = getChainSettings(audioProcessor.parameters);

    monoChain.setBypassed<ChainPositions::LowCut>(chainSettings.lowCutBypass);
    monoChain.setBypassed<ChainPositions::Peak>(chainSettings.peakBypass);
    monoChain.setBypassed<ChainPositions::HighCut>(chainSettings.highCutBypass);

    auto peakCoefficients = makePeakFilter(chainSettings, audioProcessor.getSampleRate());
    updateCoefficients(monoChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);

    auto lowCutCoefficients = makeLowCutFilter(chainSettings, audioProcessor.getSampleRate());
    updateCutFilter(monoChain.get<ChainPositions::LowCut>(), lowCutCoefficients, chainSettings.lowCutSlope);

    auto highCutCoefficients = makeHighCutFilter(chainSettings, audioProcessor.getSampleRate());
    updateCutFilter(monoChain.get<ChainPositions::HighCut>(), highCutCoefficients, chainSettings.highCutSlope);
}

void ResponseCurveComponent::paint(juce::Graphics &graphics) {
    using namespace juce;
    // Our component is opaque, so we must completely fill the background with a solid colour.
    // g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    graphics.fillAll(Colours::black);

    graphics.drawImage(background, getLocalBounds().toFloat());

    auto responseArea = getAnalysisArea();

    auto width = responseArea.toDouble().getWidth();

    auto &lowCut = monoChain.get<ChainPositions::LowCut>();
    auto &peak = monoChain.get<ChainPositions::Peak>();
    auto &highCut = monoChain.get<ChainPositions::HighCut>();

    auto sampleRate = audioProcessor.getSampleRate();

    std::vector<double> magnitudes;

    magnitudes.resize(static_cast<unsigned long>(lrint(width)));

    for (unsigned long i = 0; i < magnitudes.size(); ++i) {
        double magnitude = 1.0F;
        auto frequency = mapToLog10(static_cast<double>(i) / width, 20.0, 20000.0);

        if (!monoChain.isBypassed<ChainPositions::Peak>()) {
            magnitude *= peak.coefficients->getMagnitudeForFrequency(frequency, sampleRate);
        }

        if (!monoChain.isBypassed<ChainPositions::LowCut>()) {
            if (!lowCut.isBypassed<0>()) {
                magnitude *= lowCut.get<0>().coefficients->getMagnitudeForFrequency(frequency, sampleRate);
            }
            if (!lowCut.isBypassed<1>()) {
                magnitude *= lowCut.get<1>().coefficients->getMagnitudeForFrequency(frequency, sampleRate);
            }
            if (!lowCut.isBypassed<2>()) {
                magnitude *= lowCut.get<2>().coefficients->getMagnitudeForFrequency(frequency, sampleRate);
            }
            if (!lowCut.isBypassed<3>()) {
                magnitude *= lowCut.get<3>().coefficients->getMagnitudeForFrequency(frequency, sampleRate);
            }
        }
        if (!monoChain.isBypassed<ChainPositions::HighCut>()) {
            if (!highCut.isBypassed<0>()) {
                magnitude *= highCut.get<0>().coefficients->getMagnitudeForFrequency(frequency, sampleRate);
            }
            if (!highCut.isBypassed<1>()) {
                magnitude *= highCut.get<1>().coefficients->getMagnitudeForFrequency(frequency, sampleRate);
            }
            if (!highCut.isBypassed<2>()) {
                magnitude *= highCut.get<2>().coefficients->getMagnitudeForFrequency(frequency, sampleRate);
            }
            if (!highCut.isBypassed<3>()) {
                magnitude *= highCut.get<3>().coefficients->getMagnitudeForFrequency(frequency, sampleRate);
            }
        }
        magnitudes[i] = Decibels::gainToDecibels(magnitude);
    }

    Path responseCurve;

    const double outputMin = responseArea.getBottom();
    const double outputMax = responseArea.getY();
    auto map = [outputMin, outputMax](double input) {
        return jmap(input, -24.0, 24.0, outputMin, outputMax);
    };

    responseCurve.startNewSubPath(responseArea.toFloat().getX(), static_cast<float>(map(magnitudes.front())));

    for (size_t i = 1; i < magnitudes.size(); ++i) {
        responseCurve.lineTo(responseArea.toFloat().getX() + static_cast<float>(i), static_cast<float>(map(magnitudes[i])));
    }

    if (shouldShowFFTAnalysis) {
        auto leftChannelFFTPath = leftChannelPathProducer.getPath();
        leftChannelFFTPath.applyTransform(AffineTransform::translation(responseArea.toFloat().getX(), responseArea.toFloat().getY()));
        graphics.setColour(Colours::skyblue);
        graphics.strokePath(leftChannelFFTPath, PathStrokeType(1.0F));

        auto rightChannelFFTPath = rightChannelPathProducer.getPath();
        rightChannelFFTPath.applyTransform(AffineTransform::translation(responseArea.toFloat().getX(), responseArea.toFloat().getY()));
        graphics.setColour(Colours::lightyellow);
        graphics.strokePath(rightChannelFFTPath, PathStrokeType(1.0F));
    }
    graphics.setColour(Colours::orange);
    graphics.drawRoundedRectangle(getRenderArea().toFloat(), 4.0F, 1.0F);

    graphics.setColour(Colours::white);
    graphics.strokePath(responseCurve, PathStrokeType(2.0F));
}

void ResponseCurveComponent::resized() {

    using namespace juce;
    background = Image(Image::PixelFormat::RGB, getWidth(), getHeight(), true);

    Graphics graphics(background);

    graphics.setColour(Colours::dimgrey);

    // Draw vertical frequency lines
    Array<float> frequencies{
            20, 30, 40, 50, 100,
            200, 300, 400, 500, 1000,
            2000, 3000, 4000, 5000, 10000,
            20000};

    auto renderArea = getAnalysisArea();
    auto left = renderArea.toFloat().getX();
    auto right = renderArea.toFloat().getRight();
    auto top = renderArea.toFloat().getY();
    auto bottom = renderArea.toFloat().getBottom();
    auto width = renderArea.toFloat().getWidth();

    Array<int> frequencyLineXPositions;
    for (auto frequency : frequencies) {
        auto frequencyLineXPosition = mapFromLog10(frequency, 20.F, 20000.0F);
        frequencyLineXPositions.add(static_cast<int>(std::lrint(left + width * frequencyLineXPosition)));
    }

    for (auto frequencyLineXPosition : frequencyLineXPositions) {
        graphics.drawVerticalLine(frequencyLineXPosition, top, bottom);
    }

    // Draw horizontal gain lines
    Array<float> gainValues{-24.0F, -12.0F, 0.0F, 12.0F, 24.0F};
    for (auto gainValue : gainValues) {
        auto gainLineYPosition = static_cast<int>(std::lrint(jmap(gainValue, -24.0F, 24.0F, bottom, top)));
        graphics.setColour(gainValue == 0.0F ? Colour(95, 75, 255) : Colours::dimgrey);
        graphics.drawHorizontalLine(gainLineYPosition, left, right);
    }

    // Draw frequency labels
    graphics.setColour(Colours::lightgrey);
    const int magnitudeLabelsFontSize = 10;
    graphics.setFont(magnitudeLabelsFontSize);

    Array<bool> frequencyLabelsToPrint{
            true, false, false, true, true,
            true, false, false, true, true,
            true, false, false, true, true,
            true};

    for (int i = 0; i < frequencies.size(); ++i) {
        if (!frequencyLabelsToPrint[i]) {
            continue;
        }
        auto frequency = frequencies[i];
        auto frequencyLineXPosition = frequencyLineXPositions[i];

        bool inKiloHertz = false;
        if (frequency >= 999.0F) {
            inKiloHertz = true;
            frequency /= 1000.0F;
        }

        String frequencyLabelText;
        frequencyLabelText << frequency;
        if (inKiloHertz) {
            frequencyLabelText << "kHz";
        } else {
            frequencyLabelText << "Hz";
        }

        auto textWidth = graphics.getCurrentFont().getStringWidth(frequencyLabelText);

        Rectangle<int> rectangle;
        rectangle.setSize(textWidth, magnitudeLabelsFontSize);
        rectangle.setCentre(frequencyLineXPosition, 0);
        rectangle.setY(1);

        graphics.drawFittedText(frequencyLabelText, rectangle, juce::Justification::centred, 1);
    }

    // Draw gain labels
    for (auto gainValue : gainValues) {
        auto gainLineYPosition = static_cast<int>(std::lrint(jmap(gainValue, -24.0F, 24.0F, bottom, top)));

        String gainLabelText;
        if (gainValue > 0) {
            gainLabelText << "+";
        }
        gainLabelText << gainValue;

        auto textWidth = graphics.getCurrentFont().getStringWidth(gainLabelText);

        Rectangle<int> rectangle;
        rectangle.setSize(textWidth, magnitudeLabelsFontSize);
        rectangle.setX(getWidth() - textWidth);
        rectangle.setCentre(rectangle.getCentreX(), gainLineYPosition);
        graphics.setColour(gainValue == 0.0F ? Colour(95, 75, 255) : Colours::lightgrey);

        graphics.drawFittedText(gainLabelText, rectangle, juce::Justification::centred, 1);

        // Analyzer level label
        gainLabelText.clear();
        gainLabelText << (gainValue - 24.0F);

        rectangle.setX(1);
        textWidth = graphics.getCurrentFont().getStringWidth(gainLabelText);
        rectangle.setSize(textWidth, magnitudeLabelsFontSize);
        graphics.setColour(Colours::lightgrey);

        graphics.drawFittedText(gainLabelText, rectangle, juce::Justification::centred, 1);
    }
}

juce::Rectangle<int> ResponseCurveComponent::getRenderArea() const {
    auto bounds = getLocalBounds();

    bounds.removeFromTop(12);
    bounds.removeFromBottom(2);
    bounds.removeFromLeft(20);
    bounds.removeFromRight(20);

    return bounds;
}

juce::Rectangle<int> ResponseCurveComponent::getAnalysisArea() const {
    auto bounds = getRenderArea();

    bounds.removeFromTop(4);
    bounds.removeFromBottom(4);

    return bounds;
}

//==============================================================================
SimpleEQAudioProcessorEditor::SimpleEQAudioProcessorEditor(SimpleEQAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p),
      peakFrequencySlider(*audioProcessor.parameters.getParameter("Peak Frequency"), "Hz"),
      peakGainSlider(*audioProcessor.parameters.getParameter("Peak Gain"), "dB"),
      peakQualitySlider(*audioProcessor.parameters.getParameter("Peak Quality"), ""),
      lowCutFrequencySlider(*audioProcessor.parameters.getParameter("LowCut Frequency"), "Hz"),
      highCutFrequencySlider(*audioProcessor.parameters.getParameter("HighCut Frequency"), "Hz"),
      lowCutSlopeSlider(*audioProcessor.parameters.getParameter("LowCut Slope"), "dB/Oct"),
      highCutSlopeSlider(*audioProcessor.parameters.getParameter("HighCut Slope"), "dB/Oct"),

      responseCurveComponent(audioProcessor),
      peakFrequencyAttachment(audioProcessor.parameters, "Peak Frequency", peakFrequencySlider),
      peakGainAttachment(audioProcessor.parameters, "Peak Gain", peakGainSlider),
      peakQualityAttachment(audioProcessor.parameters, "Peak Quality", peakQualitySlider),
      lowCutFrequencyAttachment(audioProcessor.parameters, "LowCut Frequency", lowCutFrequencySlider),
      highCutFrequencyAttachment(audioProcessor.parameters, "HighCut Frequency", highCutFrequencySlider),
      lowCutSlopeAttachment(audioProcessor.parameters, "LowCut Slope", lowCutSlopeSlider),
      highCutSlopeAttachment(audioProcessor.parameters, "HighCut Slope", highCutSlopeSlider),

      lowCutBypassAttachment(audioProcessor.parameters, "LowCut Bypass", lowCutBypassButton),
      peakBypassAttachment(audioProcessor.parameters, "Peak Bypass", peakBypassButton),
      highCutBypassAttachment(audioProcessor.parameters, "HighCut Bypass", highCutBypassButton),
      analyzerEnableAttachment(audioProcessor.parameters, "Analyzer Enable", analyzerEnableButton) {

    juce::ignoreUnused(audioProcessor);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    peakFrequencySlider.labels.add({0.0F, "20Hz"});
    peakFrequencySlider.labels.add({1.0F, "20kHz"});

    peakGainSlider.labels.add({0.0F, "-24dB"});
    peakGainSlider.labels.add({1.0F, "+24dB"});

    peakQualitySlider.labels.add({0.0F, "0.1"});
    peakQualitySlider.labels.add({1.0F, "10.0"});

    lowCutFrequencySlider.labels.add({0.0F, "20Hz"});
    lowCutFrequencySlider.labels.add({1.0F, "20kHz"});

    highCutFrequencySlider.labels.add({0.0F, "20Hz"});
    highCutFrequencySlider.labels.add({1.0F, "20kHz"});

    lowCutSlopeSlider.labels.add({0.0F, "12"});
    lowCutSlopeSlider.labels.add({1.0F, "48"});

    highCutSlopeSlider.labels.add({0.0F, "12"});
    highCutSlopeSlider.labels.add({1.0F, "48"});

    for (auto *component : getComponents()) {
        addAndMakeVisible(component);
    }

    lowCutBypassButton.setLookAndFeel(&lookAndFeel);
    peakBypassButton.setLookAndFeel(&lookAndFeel);
    highCutBypassButton.setLookAndFeel(&lookAndFeel);
    analyzerEnableButton.setLookAndFeel(&lookAndFeel);

    auto safeEditorPointer = juce::Component::SafePointer<SimpleEQAudioProcessorEditor>(this);
    peakBypassButton.onClick = [safeEditorPointer]() {
        if (auto *component = safeEditorPointer.getComponent()) {
            auto bypassed = component->peakBypassButton.getToggleState();
            component->peakFrequencySlider.setEnabled(!bypassed);
            component->peakGainSlider.setEnabled(!bypassed);
            component->peakQualitySlider.setEnabled(!bypassed);
        }
    };
    lowCutBypassButton.onClick = [safeEditorPointer]() {
        if (auto *component = safeEditorPointer.getComponent()) {
            auto bypassed = component->lowCutBypassButton.getToggleState();
            component->lowCutFrequencySlider.setEnabled(!bypassed);
            component->lowCutSlopeSlider.setEnabled(!bypassed);
        }
    };
    highCutBypassButton.onClick = [safeEditorPointer]() {
        if (auto *component = safeEditorPointer.getComponent()) {
            auto bypassed = component->highCutBypassButton.getToggleState();
            component->highCutFrequencySlider.setEnabled(!bypassed);
            component->highCutSlopeSlider.setEnabled(!bypassed);
        }
    };

    analyzerEnableButton.onClick = [safeEditorPointer]() {
        if (auto *component = safeEditorPointer.getComponent()) {
            auto enabled = component->analyzerEnableButton.getToggleState();
            component->responseCurveComponent.setEnabled(enabled);
        }
    };

    setSize(600, 480);
}

SimpleEQAudioProcessorEditor::~SimpleEQAudioProcessorEditor() {
    lowCutBypassButton.setLookAndFeel(nullptr);
    peakBypassButton.setLookAndFeel(nullptr);
    highCutBypassButton.setLookAndFeel(nullptr);
    analyzerEnableButton.setLookAndFeel(nullptr);
}

//==============================================================================
void SimpleEQAudioProcessorEditor::paint(juce::Graphics &graphics) {
    using namespace juce;
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    graphics.fillAll(Colours::black);
}

void SimpleEQAudioProcessorEditor::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();

    auto analyzerEnableArea = bounds.removeFromTop(25);
    analyzerEnableArea.setWidth(100);
    analyzerEnableArea.setX(5);
    analyzerEnableArea.removeFromTop(2);

    analyzerEnableButton.setBounds(analyzerEnableArea);

    bounds.removeFromTop(5);

    // JUCE_LIVE_CONSTANT lets us adjust the size of the sliders without having to recompile
    // float heightRatio = JUCE_LIVE_CONSTANT(25) / 100.0F;
    auto const heightRatio = 25.0 / 100.0F;

    auto responseArea = bounds.removeFromTop(static_cast<int>(lrint(bounds.getHeight() * heightRatio)));

    responseCurveComponent.setBounds(responseArea);

    bounds.removeFromTop(5);
    auto lowCutArea = bounds.removeFromLeft(static_cast<int>(lrint(bounds.getWidth() * 0.33)));
    auto highCutArea = bounds.removeFromRight(static_cast<int>(lrint(bounds.getWidth() * 0.5)));

    auto const bypassButtonsTopPadding = 25;
    lowCutBypassButton.setBounds(lowCutArea.removeFromTop(bypassButtonsTopPadding));
    lowCutFrequencySlider.setBounds(lowCutArea.removeFromTop(static_cast<int>(lrint(lowCutArea.getHeight() * 0.5))));
    lowCutSlopeSlider.setBounds(lowCutArea);

    highCutBypassButton.setBounds(highCutArea.removeFromTop(bypassButtonsTopPadding));
    highCutFrequencySlider.setBounds(highCutArea.removeFromTop(static_cast<int>(lrint(highCutArea.getHeight() * 0.5))));
    highCutSlopeSlider.setBounds(highCutArea);

    peakBypassButton.setBounds(bounds.removeFromTop(bypassButtonsTopPadding));
    peakFrequencySlider.setBounds(bounds.removeFromTop(static_cast<int>(lrint(bounds.getHeight() * 0.33))));
    peakGainSlider.setBounds(bounds.removeFromTop(static_cast<int>(lrint(bounds.getHeight() * 0.5))));
    peakQualitySlider.setBounds(bounds);
}

auto SimpleEQAudioProcessorEditor::getComponents() -> std::vector<juce::Component *> {
    return {
            &peakFrequencySlider,
            &peakGainSlider,
            &peakQualitySlider,
            &lowCutFrequencySlider,
            &highCutFrequencySlider,
            &lowCutSlopeSlider,
            &highCutSlopeSlider,
            &responseCurveComponent,

            &lowCutBypassButton,
            &peakBypassButton,
            &highCutBypassButton,
            &analyzerEnableButton};
}