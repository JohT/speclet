#include "Transformation.h"
#include "../windowing/WindowFunctionFactory.h"
#include "JuceHeader.h"
#include "../../utilities/PerformanceLogger.h"

Transformation::Transformation(double newSamplingRate, ResolutionType newResolution, WindowFunctionFactory::Method newWindowFunction)
    : transformTypeNr(0),
      resolution(newResolution),
      ready(false),
      calculated(false),
      transformResultsListener(nullptr),
      calculationFrameTimer(PerformanceTimer("Transformation::calculate")),
      informListenersTimer(PerformanceTimer("Transformation::informListeners")),
      waitForDestructionTimer(PerformanceTimer("Transformation::waitForDestruction")) {
    
    waitForDestruction.signal();

    setWindowFunction(newWindowFunction);

    DBG("Transformation::initialize done with sampling rate=" + juce::String(newSamplingRate) + ", resolution/blockSize=" + juce::String(newResolution));
}

//Destructor waits until a possibly within another thread currently running
//calculation ends and deletes then all allocated objects
Transformation::~Transformation() {
    {
        LOG_PERFORMANCE_OF_SCOPE("Transformation waitForDestruction");
        waitForDestructionTimer.start();
        bool timeoutDuringWait = waitForDestruction.wait(WAIT_FOR_DESTRUCTION_TIMEOUT);
        waitForDestructionTimer.stop();
        if (!timeoutDuringWait) {
            DBG("Transformation destruction: Timeout during wait!");
        }
    }
    ready = false;
    waitForDestruction.signal();
    DBG("Transform destructed");
}

auto Transformation::getWindowFunction() const -> WindowFunction * {
    return windowFunction.get();
}

void Transformation::setWindowFunction(WindowFunctionFactory::Method newWindowFunction) {
    setReady(false);
    windowFunction = WindowFunctionFactory::getSingletonInstance().getWindow(newWindowFunction, resolution);
    assert(windowFunction);
    DBG("Transformation::setWindowFunction done with windowFunctionNr=" + juce::String(newWindowFunction));
    setReady(true);
}

auto Transformation::getInputQueue() -> std::queue<double> & {
    return inputQueue;
}

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
        LOG_PERFORMANCE_OF_SCOPE("Transformation calculationFrameTimer");
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

//simple single-listener, could be extend using arrays...
void Transformation::setTransformResultListener(TransformationListener *value) {
    transformResultsListener = value;
}

//since there is just one listener, only one call has to be done
void Transformation::informListenersAboutTransformResults() {
    LOG_PERFORMANCE_OF_SCOPE("Transformation informListenersAboutTransformResults");
    informListenersTimer.start();

    if (!ready) {
        return;
    }
    if (transformResultsListener != nullptr) {
        transformResultsListener->onTransformationEvent(this);
    }

    informListenersTimer.stop();
}