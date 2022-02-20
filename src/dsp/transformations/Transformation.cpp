#include "Transformation.h"
#include "../../libs/juce/JuceLibraryCode/JuceHeader.h"
#include "../../source/utilities/PerformanceManager.h"

using namespace std;
#ifndef __WINDOW_FACTORY__
	#define WINDOW_FACTORY WindowFunctionsFactory::getSingletonInstance()
#endif

Transformation::Transformation(double samplingRate, long resolution, int windowFunctionNr) 
	:	mInputQueue(0),
		mOutputBuffer(0),
		mSpectralDataInfo(0),
		mWindowFunction(0),
		mTransformResultsListener(0)
{
	ready					= false;
	calculated			= false;

	mSamplingRate		= samplingRate;	
	mResolution			= resolution;	
	mInputQueue			= new queue<double>();
	mOutputBuffer		= new SpectralDataBuffer();
	waitForDestruction= new juce::WaitableEvent(true);
	waitForDestruction->signal();
	mTransformTypeNr	= 0;

	setWindowFunction(windowFunctionNr);

	DBG(T("Transformation::initialize done with fs=")	+ 
						  juce::String(mSamplingRate)			+
		T(",res=")	+ juce::String(mResolution)			+
		T(",fres=")	+ juce::String(mFrequencyResolution)
	);
}

//destructor: waits, until a possibly within another thread currently running 
//calculation ends and deletes then all allocated objects
Transformation::~Transformation() {
	PerformanceManager::getSingletonInstance()->start(T("waitForDestruction"));
	bool timeoutDuringWait = waitForDestruction->wait(3000);
	PerformanceManager::getSingletonInstance()->stop(T("waitForDestruction"));
	if (!timeoutDuringWait) DBG(T("Transformation destruction: Timeout during wait!"));
	
	ready = false;
	waitForDestruction->signal();

	if (mInputQueue)			delete(mInputQueue);
	if (mOutputBuffer)		delete(mOutputBuffer);
	if (mSpectralDataInfo)	delete(mSpectralDataInfo);
	if (waitForDestruction) delete(waitForDestruction);

	mInputQueue = 0;
	mOutputBuffer = 0;
	mSpectralDataInfo = 0;
	waitForDestruction = 0;

	DBG(T("Transform destructed"));
}

//loads or replaces the window function with the given number (see class WindowFunctionsFactory)
void Transformation::setWindowFunction(int windowFunctionNr) {
	ready = false;

	mWindowFunction = WINDOW_FACTORY->createWindowFunction(windowFunctionNr, mResolution);
	assert(mWindowFunction);
	DBG(T("Transformation::setWindowFunction done with windowFunctionNr=") + juce::String(windowFunctionNr));

	ready = true;
}

//reads the next input sample
void Transformation::setNextInputSample(double sample) {
	if (!ready)					return;
	if (!mInputQueue) return; 
	mInputQueue->push(sample);

	if (!mWindowFunction)	return;
	if (!calculated)			return;

	calculationFrame();
}

//this method is called on every new input sample and 
//contains the sequence of actions associated with the calculation, e.g.:
//enough data? - ready? - calculate - informListeners,...
void Transformation::calculationFrame() {
	calculated = false;

	assert(mResolution > 0);
	if (mInputQueue->size() < mResolution) {
	//Calculation only with at least N samples possible
		calculated = true;
		return;
	}
	if (!ready) return;

	{
		//begin of critical section: only one thread per time ------------------------------
		PerformanceManager::getSingletonInstance()->start(T("calculationFrame"));
		const ScopedLock myScopedLock (criticalSection);
		waitForDestruction->reset();

		calculate();
		if (isOutputAvailable()) informListenersAboutTransformResults();

		calculated = true;
		waitForDestruction->signal();
		PerformanceManager::getSingletonInstance()->stop(T("calculationFrame"));
		//end of critical section                          ---------------------------------
	}
}

//returns true, if spectral data is available
bool Transformation::isOutputAvailable() {
	if (!ready) return false;
	if (!mOutputBuffer) return false;
	return (getSpectralDataBuffer()->unread() > 0);
}

SpectralDataBuffer* Transformation::getSpectralDataBuffer(void) {
	return mOutputBuffer;
};

void Transformation::getNextSpectrum (SpectralDataBuffer::ItemType* item) {
	getSpectralDataBuffer()->read(item);
}

SpectralDataBuffer::ItemStatisticsType	Transformation::getSpectrumStatistics (SpectralDataBuffer::ItemType* item) {
	return getSpectralDataBuffer()->getStatistics(item);
}

//simple single-listener, could be extend using arrays...
void Transformation::setTransformResultListener(TransformationListener* value) {
	mTransformResultsListener = value;
}

//since there is just one listener, only one call has to be done
void Transformation::informListenersAboutTransformResults() {
	PerformanceManager::getSingletonInstance()->start(T("informListeners"));

	if (!ready) return;
	if (mTransformResultsListener) mTransformResultsListener->onTransformationEvent(this);

	PerformanceManager::getSingletonInstance()->stop(T("informListeners"));
}