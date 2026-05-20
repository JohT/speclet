/*
  ==============================================================================
  This file is part of the VST spectrum analyzer plugin "speclet" (working title)
  Copyright 2011 by Johannes Troppacher
  ==============================================================================
*/
#pragma once

#include "transformations/Transformation.h"
#include "transformations/TransformationParameters.h"
#include "transformations/WaveletParameters.h"
#include "windowing/WindowParameters.h"
#include <atomic>
#include <juce_core/juce_core.h>
#include <vector>

/**
 * @brief Parameters needed to create or rebuild the active transformation.
 */
struct TransformationRebuildParams {
    TransformationParameters::Type transformationType = TransformationParameters::Type::DEFAULT;
    double sampleRate = 44100.0;
    Transformation::ResolutionType resolution = 4096;
    WindowParameters::WindowFunction windowFunction = WindowParameters::WindowFunction::DEFAULT;
    WaveletParameters::WaveletBase waveletBase = WaveletParameters::WaveletBase::DEFAULT;
    WaveletParameters::ResolutionRatioOption waveletPacketBasis = WaveletParameters::ResolutionRatioOption::DEFAULT;
};

/**
 * @brief Background thread that drains the lock-free audio input FIFO,
 *        runs DSP (FFT / wavelet) calculations, and handles transformation
 *        rebuilds triggered by parameter changes — all off the audio thread.
 */
class DspThread : public juce::Thread {
public:
    DspThread(juce::AbstractFifo &fifo, std::vector<float> &buffer);
    ~DspThread() override;

    void run() override;

    /** Called from the audio thread after writing new samples into the FIFO. */
    void notifyNewData();

    /** Called from any non-RT thread when transformation parameters change. */
    void requestTransformationRebuild(const TransformationRebuildParams &params);

private:
    juce::AbstractFifo &inputFifo;
    std::vector<float> &inputCircularBuffer;

    juce::WaitableEvent newDataAvailable{false};
    std::atomic<bool> transformationChangePending{false};
    TransformationRebuildParams pendingParams;
    juce::CriticalSection paramLock;

    Transformation *currentTransformation = nullptr;

    void rebuildTransformationIfNeeded();
    void drainFifo();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DspThread)
};
