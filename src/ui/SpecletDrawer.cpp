/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  4 May 2011 11:36:36pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "SpecletDrawer.h"
#include "../dsp/transformations/TransformationFactory.h"
#include "../utilities/PerformanceLogger.h"
#include "ColourGradients.h"
#include "SpecletDrawerParameters.h"
#include "juce_audio_utils/juce_audio_utils.h"
#include "juce_core/juce_core.h"
#include <type_traits>

//[/Headers]


//[MiscUserDefs] You can add your own user definitions and misc code here...
const juce::Colour SpecletDrawer::AXIS_COLOR(0xffffffc0);

//[/MiscUserDefs]

//==============================================================================
SpecletDrawer::SpecletDrawer() {

    //[UserPreSize]
    //[/UserPreSize]

    setSize(sizeX, sizeY);

    //[Constructor] You can add your own custom stuff here..

    //registers itself also as a transformation-result-lister for every transformation that will be created in future
    TransformationFactory::getSingletonInstance().registerForTransformationResults(this);

    startTimer(TIMER);
    ready = true;
    waitForDestruction.signal();
    //[/Constructor]
}

SpecletDrawer::~SpecletDrawer() {
    //[Destructor_pre]. You can add your own custom destruction code here..
    ready = false;
    {
        LOG_PERFORMANCE_OF_SCOPE("SpecletDrawer waitForDestruction");
        bool timeoutDuringWait = waitForDestruction.wait(WAIT_FOR_DESTRUCTION_TIMEOUT);
        if (!timeoutDuringWait) {
            DBG("SpecletDrawer destruction: Timeout during wait!");
        }
    }
    //[/Destructor_pre]

    //[Destructor]. You can add your own custom destruction code here..
    TransformationFactory::getSingletonInstance().registerForTransformationResults(nullptr);
    waitForDestruction.signal();
    DBG("SpecletDrawer removed as parameter and transformation results listener and finally destructed");
    //[/Destructor]
}

//==============================================================================
void SpecletDrawer::paint(juce::Graphics &g) {
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(juce::Colours::black);

    //[UserPaint] Add your own custom painting code here..

    //draw spectrum ----------------
    {
        LOG_PERFORMANCE_OF_SCOPE("SpecletDrawer paint draw spectrum");
        g.drawImageAt(spectrumImage, 0, 0);
    }

    //draw red position cursor ----------------
    g.setColour(juce::Colours::red);
    float cursorX = static_cast<float>(currentCursorXPos);
    g.drawLine(cursorX, 0, cursorX, static_cast<float>(sizeY), 1.0);

    //draw frequency and time axis ----------------
    {
        LOG_PERFORMANCE_OF_SCOPE("SpecletDrawer paint draw axis");
        g.drawImageAt(axisImage, 0, 0);
    }

    //[/UserPaint]
}

void SpecletDrawer::resized() {
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void SpecletDrawer::timerCallback() {
    stopTimer();
    if (!ready) {
        return;
    }
    repaint();
    startTimer(TIMER);
}

void SpecletDrawer::onTransformationEvent(TransformationResult *result) {
    //This method is automatically called, when there are new transformation results available,
    //as far as it had been successfully been registered as a listener by "SpecletJuceMainUI"
    int watchDog = 200;
    while (result->isOutputAvailable()) {
        waitForDestruction.reset();
        appendSpectralImage(result);
        waitForDestruction.signal();
        if (!ready) {
            DBG("SpecletDrawer: Transformation result received, but not ready to draw.");
            return;
        }
        watchDog--;
        if (watchDog <= 0) {
            //prevents endless loop
            DBG("SpecletDrawer::onTransformationEvent watchDog canceled drawing!");
            watchDog = 200;
            break;
        }
    }
    //if effective timeresolution didn't change, the timeresolution-axis needn't to be redrawn
    auto spectralDataInfo = result->getSpectralDataInfo();
    auto timeResolution = spectralDataInfo.getTimeResolutionMs();
    if (timeResolution != currentTimeResolution) {
        updateTimeAxisImage(timeResolution);
    }
    currentSamplingFrequency = spectralDataInfo.getSamplingFrequency();
}

//This method is called when a parameter changes (listener)
void SpecletDrawer::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier & /*changedProperty*/) {
    const juce::ScopedLock myScopedLock(criticalSection);

    juce::String changedParameterName = treeWhosePropertyHasChanged.getType().toString();
    juce::var changedParameterValue = treeWhosePropertyHasChanged.getProperty(SpecletParameters::PROPERTY_VALUE);

    if (changedParameterName.equalsIgnoreCase(SpecletParameters::PARAMETER_LOGFREQUENCY)) {
        settings.logFrequency = changedParameterValue.equals(static_cast<std::underlying_type<SpecletDrawerParameters::Axis>::type>(SpecletDrawerParameters::Axis::LOGARITHMIC));
        updateFrequencyAxisImage();
    }
    if (changedParameterName.equalsIgnoreCase(SpecletParameters::PARAMETER_LOGMAGNITUDE)) {
        settings.logMagnitude = changedParameterValue.equals(static_cast<std::underlying_type<SpecletDrawerParameters::Axis>::type>(SpecletDrawerParameters::Axis::LOGARITHMIC));
    }
    if (changedParameterName.equalsIgnoreCase(SpecletParameters::PARAMETER_COLORMODE)) {
        renderingHelper.setColourGradient(ColourGradients::forIndex(changedParameterValue));
    }
}

void SpecletDrawer::appendSpectralImage(TransformationResult *result) {
    if (result == nullptr) {
        DBG("SpecletDrawer::appendSpectralImage(..) no result parameter");
        return;
    }
    if (currentCursorXPos > (sizeX - 1)) {
        currentCursorXPos = 0;
    }
    if (spectrumImage.isNull()) {
        spectrumImage = juce::Image(juce::Image::RGB, sizeX, sizeY, true);
    }

    renderingHelper.renderVerticalPoints(
            result,
            settings,
            currentCursorXPos,
            &spectrumImage);
    currentCursorXPos++;
}


/* 
This method draws the currently active frequency axis into the local member frequencyAxisImage,
which is drawn on top of the spectrum whithin the paint() method.
*/
void SpecletDrawer::updateFrequencyAxisImage() {
    int maxWidthOfFrequencyAxis = 80;

    //clears the part of the axis image, where the frequency resolution is drawn at
    juce::Rectangle<int> areaToClear(axisImage.getBounds().withRight(maxWidthOfFrequencyAxis));
    axisImage.clear(areaToClear, juce::Colours::transparentBlack);

    //get drawing context
    juce::Graphics g(axisImage);

    // --- gets max frequency
    double maxSpectralFrequency = 22050;
    if (currentSamplingFrequency > 0) {
        maxSpectralFrequency = currentSamplingFrequency / 2;
    }

    int axisLineLength = 10;
    g.setColour(AXIS_COLOR);
    juce::Font oldFont = g.getCurrentFont();

    if (settings.logFrequency) {
        double logSubDivisionsPerDecade[] = {log10(1.0), log10(2.0), log10(5.0), log10(7.0)};
        int numberOfSubDivisions = sizeof logSubDivisionsPerDecade / sizeof logSubDivisionsPerDecade[0];

        double maxFrequencyLog = log10(maxSpectralFrequency);
        double minFrequencyLog = 1.0;

        for (double logFreqDecade = 1; logFreqDecade <= 4; logFreqDecade++) {
            for (auto i = 0; i < (numberOfSubDivisions - 1); i++) {
                double logFreq = logSubDivisionsPerDecade[i] + logFreqDecade;
                if (logFreq > maxFrequencyLog) {
                    break;
                }
                double posPercent = (logFreq - minFrequencyLog) / (maxFrequencyLog - minFrequencyLog);
                int yPos = static_cast<int>(lrint((sizeY - 1) * (1.0f - posPercent)));
                double freqDouble = pow(10, logFreq);
                double freqCeil = ceil(freqDouble);
                double freqFloor = floor(freqDouble);
                int freq = ((freqCeil - freqDouble) > (freqDouble - freqFloor)) ? (int) freqFloor : (int) freqCeil;

                juce::String frequencyText(freq);
                frequencyText += " Hz";

                if (i == 0) {
                    //full decade: full line length and text size
                    g.setFont(12.0F);
                    g.drawFittedText(frequencyText,
                                     axisLineLength + 3,
                                     yPos - 7,
                                     maxWidthOfFrequencyAxis,
                                     yPos + 10,
                                     juce::Justification::topLeft,
                                     1);
                    g.fillRect(juce::Rectangle<int>(0, yPos, axisLineLength, 2));
                } else {
                    //subdivision: smaller font, smaller lines
                    g.setFont(9.0F);
                    g.drawFittedText(
                            frequencyText,
                            axisLineLength + 3,
                            yPos - 3,
                            maxWidthOfFrequencyAxis,
                            yPos + 10,
                            juce::Justification::topLeft,
                            1);
                    g.fillRect(juce::Rectangle<int>(0, yPos, axisLineLength, 1));
                }
            }
        }
    } else {
        double frequencyLabels = 10;
        double step = 1.0 / frequencyLabels;

        for (double posPercent = step; posPercent < 1.0F; posPercent += step) {
            int yPos = static_cast<int>(lrint(getHeight() * (1.0f - posPercent)));

            juce::String frequencyText(maxSpectralFrequency * posPercent);
            frequencyText += " Hz";

            g.setFont(12.0F);
            g.drawFittedText(
                    frequencyText,
                    axisLineLength + 3,
                    yPos - 7,
                    maxWidthOfFrequencyAxis,
                    yPos,
                    juce::Justification::topLeft,
                    1);
            g.fillRect(juce::Rectangle<int>(0, yPos, axisLineLength, 1));
        }
    }
    g.setFont(oldFont);
}

void SpecletDrawer::updateTimeAxisImage(double timeresolution) {
    int timeAxisWidth = 60;
    int lineLength = 50;
    int xPosStart = sizeX - timeAxisWidth;
    int yPosStart = getHeight() - 50;
    int yPosLine = getHeight() - 30;

    currentTimeResolution = timeresolution;

    //clears the part of the axis image, where the time resolution is drawn at
    auto axisImageBounds = axisImage.getBounds();
    auto rectangleToClear = axisImageBounds.withLeft(xPosStart).withTop(yPosStart);

    if (rectangleToClear.getWidth() <= 0) {
        DBG("updateTimeAxisImage: Time axis width is 0. Skip axis update.");

        auto topLeftX = axisImageBounds.getTopLeft().getX();
        auto topLeftY = axisImageBounds.getTopLeft().getY();
        DBG("image topLeftX to clear: " << topLeftX << " image topLeftY to clear: " << topLeftY);

        auto rectangleTopLeftX = rectangleToClear.getTopLeft().getX();
        auto rectangleTopLeftY = rectangleToClear.getTopLeft().getY();
        DBG("rectangle topLeftX to clear: " << rectangleTopLeftX << " rectangle topLeftY to clear: " << rectangleTopLeftY);

        return;
    }
    juce::Rectangle<int> areaToClear(rectangleToClear);
    axisImage.clear(areaToClear, juce::Colours::transparentBlack);

    //get drawing context
    juce::Graphics g(axisImage);

    double timeResultionForGivenLength = timeresolution * lineLength;
    juce::String timeResolutionText;
    if (timeResultionForGivenLength < 1.0) {
        timeResolutionText = juce::String((int) timeResultionForGivenLength * 1000);
        timeResolutionText += " ns";
    } else if (timeResultionForGivenLength < 1000.0) {
        timeResolutionText = juce::String((int) timeResultionForGivenLength);
        timeResolutionText += " ms";
    } else if (timeResultionForGivenLength > 1000.0) {
        timeResolutionText = juce::String((int) (timeResultionForGivenLength / 1000.0));
        timeResolutionText += " s";
    }
    g.setColour(AXIS_COLOR);
    g.drawFittedText(timeResolutionText, xPosStart + 7, yPosStart, 79, 49, juce::Justification::topLeft, true);
    g.fillRect(juce::Rectangle<int>(xPosStart, yPosLine, lineLength, 1));
    g.fillRect(juce::Rectangle<int>(xPosStart, yPosLine - 5, 1, 10));
    g.fillRect(juce::Rectangle<int>(xPosStart + lineLength, yPosLine - 5, 1, 10));
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

	 This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SpecletDrawer" componentName=""
					  parentClasses="public Component, public TransformationListener, public Timer, public juce::ValueTree::Listener"
					  constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
					  snapShown="1" overlayOpacity="0.330000013" fixedSize="0" initialWidth="100"
					  initialHeight="360">
  <BACKGROUND backgroundColour="ff000000"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
