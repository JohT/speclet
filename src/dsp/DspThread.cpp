/*
  ==============================================================================
  This file is part of the VST spectrum analyzer plugin "speclet" (working title)
  Copyright 2011 by Johannes Troppacher
  ==============================================================================
*/
#include "DspThread.h"
#include "transformations/TransformationFactory.h"
#include "juce_core/juce_core.h"

DspThread::DspThread(juce::AbstractFifo &fifo, std::vector<float> &buffer)
    : juce::Thread("DspThread"), inputFifo(fifo), inputCircularBuffer(buffer) {
}

DspThread::~DspThread() {
    stopThread(3000);
}

void DspThread::run() {
    while (!threadShouldExit()) {
        newDataAvailable.wait(100);
        rebuildTransformationIfNeeded();
        drainFifo();
    }
}

void DspThread::notifyNewData() {
    newDataAvailable.signal();
}

void DspThread::requestTransformationRebuild(const TransformationRebuildParams &params) {
    {
        const juce::ScopedLock lock(paramLock);
        pendingParams = params;
        transformationChangePending = true;
    }
    newDataAvailable.signal();
}

void DspThread::rebuildTransformationIfNeeded() {
    if (!transformationChangePending.load()) {
        return;
    }
    TransformationRebuildParams params;
    {
        const juce::ScopedLock lock(paramLock);
        if (!transformationChangePending.load()) {
            return;
        }
        params = pendingParams;
        transformationChangePending = false;
    }

    currentTransformation = TransformationFactory::getSingletonInstance().createTransformation(
            params.transformationType,
            params.sampleRate,
            params.resolution,
            params.windowFunction,
            params.waveletBase,
            params.waveletPacketBasis);

    DBG("DspThread::rebuildTransformationIfNeeded done");
}

void DspThread::drainFifo() {
    const int numReady = inputFifo.getNumReady();
    if (numReady <= 0) {
        return;
    }

    if (currentTransformation == nullptr) {
        // Discard samples when no transformation is active (e.g. BYPASS mode)
        int start1, size1, start2, size2;
        inputFifo.prepareToRead(numReady, start1, size1, start2, size2);
        inputFifo.finishedRead(size1 + size2);
        return;
    }

    int start1, size1, start2, size2;
    inputFifo.prepareToRead(numReady, start1, size1, start2, size2);
    for (int i = 0; i < size1; ++i) {
        currentTransformation->setNextInputSample(inputCircularBuffer[start1 + i]);
    }
    for (int i = 0; i < size2; ++i) {
        currentTransformation->setNextInputSample(inputCircularBuffer[start2 + i]);
    }
    inputFifo.finishedRead(size1 + size2);
}
