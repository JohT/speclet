#include "Transformation.h"
#include "JuceHeader.h"
#include "../windowing/WindowFunctionFactory.h"


using namespace std;

Transformation::Transformation(double samplingRate, ResolutionType newResolution, int newWindowFunctionNr)
    : waitForDestruction(new juce::WaitableEvent(true)), mTransformTypeNr(0), mOutputBuffer(new SpectralDataBuffer()),
      mTransformResultsListener(nullptr),
      resolution(newResolution), mSamplingRate(samplingRate), ready(false), calculated(false),
      mInputQueue(new queue<double>()),
      mSpectralDataInfo(nullptr),
      calculationFrameTimer(PerformanceTimer("Transformation::calculate")),
      informListenersTimer(PerformanceTimer("Transformation::informListeners")),
      waitForDestructionTimer(PerformanceTimer("Transformation::waitForDestruction")) {

    waitForDestruction->signal();

    setWindowFunction(newWindowFunctionNr);

    DBG("Transformation::initialize done with fs=" +
        juce::String(mSamplingRate) +
        ",res=" + juce::String(newResolution) +
        ",fres=" + juce::String(mFrequencyResolution));
}

//destructor: waits, until a possibly within another thread currently running
//calculation ends and deletes then all allocated objects
Transformation::~Transformation() {
    waitForDestructionTimer.start();
    bool timeoutDuringWait = waitForDestruction->wait(3000);
    waitForDestructionTimer.stop();
    if (!timeoutDuringWait) {
        DBG("Transformation destruction: Timeout during wait!");
    }

    ready = false;
    waitForDestruction->signal();

    delete (mInputQueue);
    delete (mOutputBuffer);
    delete (mSpectralDataInfo);
    delete (waitForDestruction);

    mInputQueue = nullptr;
    mOutputBuffer = nullptr;
    mSpectralDataInfo = nullptr;
    waitForDestruction = nullptr;

    DBG("Transform destructed");
}

auto Transformation::getWindowFunction() const -> WindowFunction * {
    return windowFunction.get();
}

//loads or replaces the window function with the given number (see class WindowFunctionsFactory)
void Transformation::setWindowFunction(int windowFunctionNr) {
    setReady(false);
    windowFunction = WindowFunctionFactory::getSingletonInstance().getWindow(static_cast<WindowFunctionFactory::Method>(windowFunctionNr), resolution);
    assert(windowFunction);
    DBG("Transformation::setWindowFunction done with windowFunctionNr=" + juce::String(windowFunctionNr));
    setReady(true);
}

//reads the next input sample
void Transformation::setNextInputSample(double sample) {
    if (!ready) {
        return;
    }
    if (mInputQueue == nullptr) {
        return;
    }
    mInputQueue->push(sample);

    if (windowFunction == nullptr) {
        return;
    }
    if (!calculated) {
        return;
    }

    calculationFrame();
}

//this method is called on every new input sample and
//contains the sequence of actions associated with the calculation, e.g.:
//enough data? - ready? - calculate - informListeners,...
void Transformation::calculationFrame() {
    calculated = false;

    assert(resolution > 0);
    if (mInputQueue->size() < static_cast<unsigned long>(resolution)) {
        //Calculation only with at least N samples possible
        calculated = true;
        return;
    }
    if (!ready) {
        return;
    }

    {
        //begin of critical section: only one thread per time ------------------------------
        calculationFrameTimer.start();
        const ScopedLock myScopedLock(criticalSection);
        waitForDestruction->reset();

        calculate();
        if (isOutputAvailable()) {
            informListenersAboutTransformResults();
        }

        calculated = true;
        waitForDestruction->signal();
        calculationFrameTimer.stop();
        //end of critical section                          ---------------------------------
    }
}

//returns true, if spectral data is available
auto Transformation::isOutputAvailable() -> bool {
    if (!ready) {
        return false;
    }
    if (mOutputBuffer == nullptr) {
        return false;
    }
    return (getSpectralDataBuffer()->unread() > 0);
}

auto Transformation::getSpectralDataBuffer() -> SpectralDataBuffer * {
    return mOutputBuffer;
}

void Transformation::getNextSpectrum(SpectralDataBuffer::ItemType *item) {
    getSpectralDataBuffer()->read(item);
}

auto Transformation::getSpectrumStatistics(SpectralDataBuffer::ItemType *item) -> SpectralDataBuffer::ItemStatisticsType {
    return getSpectralDataBuffer()->getStatistics(item);
}

//simple single-listener, could be extend using arrays...
void Transformation::setTransformResultListener(TransformationListener *value) {
    mTransformResultsListener = value;
}

//since there is just one listener, only one call has to be done
void Transformation::informListenersAboutTransformResults() {
    informListenersTimer.start();

    if (!ready) {
        return;
    }
    if (mTransformResultsListener != nullptr) {
        mTransformResultsListener->onTransformationEvent(this);
    }

    informListenersTimer.stop();
}