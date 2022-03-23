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
#include "../dsp/transformations/TransformationFactory.h"
#include "../utilities/PerformanceLogger.h"
#include "ColourGradients.h"
//[/Headers]

#include "SpectronDrawer.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
const juce::Colour SpectronDrawer::AXIS_COLOR(0xffffffc0);

//[/MiscUserDefs]

//==============================================================================
SpectronDrawer::SpectronDrawer() : imageDrawingTimer(PerformanceTimer("imageDrawingTimer")), axisDrawingTimer(PerformanceTimer("axisDrawingTimer")) {

    //[UserPreSize]
    //[/UserPreSize]

    setSize(100, 360);

    //[Constructor] You can add your own custom stuff here..

    sizeX = SIZE_X;
    sizeY = getHeight();
    setSize(sizeX, sizeY);

    currentCursorXPos = 0;
    currentTimeResolution = 0;

    //creates and prepares the axislayer image
    axisImage = Image(juce::Image::PixelFormat::ARGB, sizeX, sizeY, true);

    //registers itself as a transformation-result-lister
    // Transformation *transformation = TransformationFactory::getSingletonInstance().getCurrentTransformation();
    // if (transformation != nullptr) {
    //     transformation->setTransformResultListener(this);
    // }
    //registers itself also as a transformation-result-lister for every transformation that will be created in future
    TransformationFactory::getSingletonInstance().registerForTransformationResults(this);
    //registers itself as listener for parameter-changes
    SpectronParameters::getSingletonInstance().addListener(this);
    DBG("SpectronDrawer as parameter listener added");
    LOG("SpectronDrawer as parameter listener added");

    startTimer(TIMER);
    //[/Constructor]
}

SpectronDrawer::~SpectronDrawer() {
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    //[Destructor]. You can add your own custom destruction code here..
    SpectronParameters::getSingletonInstance().removeListener(this);
    DBG("SpectronDrawer as parameter listener removed");
    LOG("SpectronDrawer as parameter listener removed");
    //[/Destructor]
}

//==============================================================================
void SpectronDrawer::paint(Graphics &g) {
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(Colours::black);

    //[UserPaint] Add your own custom painting code here..

    //draw spectrum ----------------
    imageDrawingTimer.start();
    {
        LOG_PERFORMANCE_OF_SCOPE("SpectronDrawer paint draw spectrum");
        g.drawImageAt(spectrumImage, 0, 0);
    }
    imageDrawingTimer.stop();

    //draw red position cursor ----------------
    g.setColour(Colours::red);
    float cursorX = static_cast<float>(currentCursorXPos);
    g.drawLine(cursorX, 0, cursorX, static_cast<float>(sizeY), 1.0);

    //draw frequency and time axis ----------------
    axisDrawingTimer.start();
    {
        LOG_PERFORMANCE_OF_SCOPE("SpectronDrawer paint draw axis");
        g.drawImageAt(axisImage, 0, 0);
    }
    axisDrawingTimer.stop();

    //[/UserPaint]
}

void SpectronDrawer::resized() {
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void SpectronDrawer::timerCallback() {
    stopTimer();
    repaint();
    startTimer(TIMER);
}

void SpectronDrawer::onTransformationEvent(TransformationResult *result) {
    //This method is automatically called, when there are new transformation results available,
    //as far as it had been successfully been registered as a listener by "SpectronJuceMainUI"
    int watchDog = 200;
    while (result->isOutputAvailable()) {
        appendSpectralImage(result);
        watchDog--;
        if (watchDog <= 0) {
            //prevents endless loop
            DBG("SpectronDrawer::onTransformationEvent watchDog canceled drawing!");
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
void SpectronDrawer::valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier & /*changedProperty*/) {
    const ScopedLock myScopedLock(criticalSection);

    juce::String changedParameterName = treeWhosePropertyHasChanged.getType().toString();
    juce::var changedParameterValue = treeWhosePropertyHasChanged.getProperty(SpectronParameters::PROPERTY_VALUE);

    if (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_LOGFREQUENCY)) {
        settings.logFrequency = changedParameterValue.equals(SpectronParameters::PLOT_AXIS_LOGARITHMIC);
        updateFrequencyAxisImage();
    }
    if (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_LOGMAGNITUDE)) {
        settings.logMagnitude = changedParameterValue.equals(SpectronParameters::PLOT_AXIS_LOGARITHMIC);
    }
    if (changedParameterName.equalsIgnoreCase(SpectronParameters::PARAMETER_COLORMODE)) {
        renderingHelper.setColourGradient(ColourGradients::forIndex(changedParameterValue));
    }
}

void SpectronDrawer::appendSpectralImage(TransformationResult *result) {
    if (result == nullptr) {
        DBG("SpectronDrawer::appendSpectralImage(..) no result parameter");
        return;
    }
    if (currentCursorXPos > (sizeX - 1)) {
        currentCursorXPos = 0;
    }
    if (spectrumImage.isNull()) {
        spectrumImage = Image(Image::RGB, sizeX, sizeY, true);
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
void SpectronDrawer::updateFrequencyAxisImage() {
    //TODO better encapsulation
    int maxWidthOfFrequencyAxis = 80;

    //clears the part of the axis image, where the frequency resolution is drawn at
    Rectangle<int> areaToClear(axisImage.getBounds().withRight(maxWidthOfFrequencyAxis));
    axisImage.clear(areaToClear, juce::Colours::transparentBlack);

    //get drawing context
    Graphics g(axisImage);

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
                    g.fillRect(Rectangle<int>(0, yPos, axisLineLength, 2));
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
                    g.fillRect(Rectangle<int>(0, yPos, axisLineLength, 1));
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
            g.fillRect(Rectangle<int>(0, yPos, axisLineLength, 1));
        }
    }
    g.setFont(oldFont);
}

void SpectronDrawer::updateTimeAxisImage(double timeresolution) {
    //TODO better encapsulation
    int timeAxisWidth = 60;
    int lineLength = 50;
    int xPosStart = sizeX - timeAxisWidth;
    int yPosStart = getHeight() - 50;
    int yPosLine = getHeight() - 30;

    currentTimeResolution = timeresolution;

    //clears the part of the axis image, where the time resolution is drawn at
    Rectangle<int> areaToClear(axisImage.getBounds().withLeft(xPosStart).withTop(yPosStart));
    axisImage.clear(areaToClear, juce::Colours::transparentBlack);

    //get drawing context
    Graphics g(axisImage);

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
    g.fillRect(Rectangle<int>(xPosStart, yPosLine, lineLength, 1));
    g.fillRect(Rectangle<int>(xPosStart, yPosLine - 5, 1, 10));
    g.fillRect(Rectangle<int>(xPosStart + lineLength, yPosLine - 5, 1, 10));
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

	 This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SpectronDrawer" componentName=""
					  parentClasses="public Component, public TransformationListener, public Timer, public juce::ValueTree::Listener"
					  constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
					  snapShown="1" overlayOpacity="0.330000013" fixedSize="0" initialWidth="100"
					  initialHeight="360">
  <BACKGROUND backgroundColour="ff000000"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
