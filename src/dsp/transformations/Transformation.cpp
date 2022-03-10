#include "Transformation.h"
#include "../windowing/WindowFunctionFactory.h"
#include "JuceHeader.h"

Transformation::Transformation(double newSamplingRate, ResolutionType newResolution, int newWindowFunctionNr)
    : transformTypeNr(0),
      samplingRate(newSamplingRate),
      resolution(newResolution),
      ready(false), 
      calculated(false),
      mTransformResultsListener(nullptr),
      calculationFrameTimer(PerformanceTimer("Transformation::calculate")),
      informListenersTimer(PerformanceTimer("Transformation::informListeners")),
      waitForDestructionTimer(PerformanceTimer("Transformation::waitForDestruction")) {

    waitForDestruction.signal();

    setWindowFunction(newWindowFunctionNr);

    DBG("Transformation::initialize done with sampling rate=" + juce::String(newSamplingRate) + ", resolution/blockSize=" + juce::String(newResolution));
}

//destructor: waits, until a possibly within another thread currently running
//calculation ends and deletes then all allocated objects
Transformation::~Transformation() {
    waitForDestructionTimer.start();
    bool timeoutDuringWait = waitForDestruction.wait(WAIT_FOR_DESTRUCTION_TIMEOUT);
    waitForDestructionTimer.stop();
    if (!timeoutDuringWait) {
        DBG("Transformation destruction: Timeout during wait!");
    }

    ready = false;
    waitForDestruction.signal();

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

auto Transformation::getInputQueue() -> std::queue<double> & {
    return inputQueue;
}

//reads the next input sample
void Transformation::setNextInputSample(const double &sample) {
    if (!ready) {
        return;
    }
    inputQueue.push(sample);

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
    if (inputQueue.size() < resolution) {
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
        waitForDestruction.reset();

        calculate();
        if (isOutputAvailable()) {
            informListenersAboutTransformResults();
        }

        calculated = true;
        waitForDestruction.signal();
        calculationFrameTimer.stop();
        //end of critical section                          ---------------------------------
    }
}

//returns true, if spectral data is available
auto Transformation::isOutputAvailable() -> bool {
    return ready && (outputBuffer.unread() > 0);
}

auto Transformation::getSpectralDataBuffer() -> SpectralDataBuffer * {
    return &outputBuffer;
}

void Transformation::getNextSpectrum(SpectralDataBuffer::ItemType *item) {
   outputBuffer.read(item);
}

auto Transformation::getSpectrumStatistics(SpectralDataBuffer::ItemType *item) -> SpectralDataBuffer::ItemStatisticsType {
    return SpectralDataBuffer::getStatistics(item);
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