#include "Transformation.h"
#include "../../utilities/PerformanceManager.h"
#include "JuceHeader.h"


using namespace std;
#ifndef __WINDOW_FACTORY__
#define WINDOW_FACTORY WindowFunctionsFactory::getSingletonInstance()
#endif

Transformation::Transformation(double samplingRate, long resolution, int windowFunctionNr)
    : waitForDestruction(new juce::WaitableEvent(true)), mTransformTypeNr(0), mOutputBuffer(new SpectralDataBuffer()),
      mTransformResultsListener(nullptr),
      mResolution(resolution), mSamplingRate(samplingRate), ready(false), calculated(false),
      mInputQueue(new queue<double>()),
      mSpectralDataInfo(nullptr),
      mWindowFunction(nullptr) {

    waitForDestruction->signal();

    setWindowFunction(windowFunctionNr);

    DBG("Transformation::initialize done with fs=" +
        juce::String(mSamplingRate) +
        ",res=" + juce::String(mResolution) +
        ",fres=" + juce::String(mFrequencyResolution));
}

//destructor: waits, until a possibly within another thread currently running
//calculation ends and deletes then all allocated objects
Transformation::~Transformation() {
    PerformanceManager::getSingletonInstance().start("waitForDestruction");
    bool timeoutDuringWait = waitForDestruction->wait(3000);
    PerformanceManager::getSingletonInstance().stop("waitForDestruction");
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

//loads or replaces the window function with the given number (see class WindowFunctionsFactory)
void Transformation::setWindowFunction(int windowFunctionNr) {
    ready = false;

    mWindowFunction = WINDOW_FACTORY->createWindowFunction(windowFunctionNr, mResolution);
    assert(mWindowFunction);
    DBG("Transformation::setWindowFunction done with windowFunctionNr=" + juce::String(windowFunctionNr));

    ready = true;
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

    if (mWindowFunction == nullptr) {
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

    jassert(mResolution > 0);
    if (mInputQueue->size() < static_cast<unsigned long>(mResolution)) {
        //Calculation only with at least N samples possible
        calculated = true;
        return;
    }
    if (!ready) {
        return;
    }

    {
        //begin of critical section: only one thread per time ------------------------------
        PerformanceManager::getSingletonInstance().start("calculationFrame");
        const ScopedLock myScopedLock(criticalSection);
        waitForDestruction->reset();

        calculate();
        if (isOutputAvailable()) {
            informListenersAboutTransformResults();
        }

        calculated = true;
        waitForDestruction->signal();
        PerformanceManager::getSingletonInstance().stop("calculationFrame");
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
    PerformanceManager::getSingletonInstance().start("informListeners");

    if (!ready) {
        return;
    }
    if (mTransformResultsListener != nullptr) {
        mTransformResultsListener->onTransformationEvent(this);
    }

    PerformanceManager::getSingletonInstance().stop("informListeners");
}