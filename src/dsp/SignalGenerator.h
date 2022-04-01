/*
  ==============================================================================
  This file is part of the VST spectrum analyzer plugin "speclet" (working title)
  Copyright 2011 by Johannes Troppacher
	------------------------------------------------------------------------------
  This file may use parts of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.
	------------------------------------------------------------------------------
  This file may use parts of the fftw library
  Copyright 2003 Matteo Frigo, Copyright 2003 Massachusetts Institute of Technology
	------------------------------------------------------------------------------
  This file may use parts of the wave++ library
  Copyright 1999 by S. E. Ferrando, L. A. Kolasa and N. Kovacevic
  ==============================================================================
*/
#pragma once
#include "../plugin/SpecletParameters.h"
#include "SignalGeneratorParameters.h"
#include <random>

class SignalGenerator {
public:
    explicit SignalGenerator(
            double newSamplingFrequency = DEFAULT_SAMPLING_FREQUENCY,
            SignalGeneratorParameters::Waveform newWaveform = SignalGeneratorParameters::Waveform::DEFAULT,
            double newSignalFrequency = DEFAULT_GENERATOR_FREQUENCY);

    auto getNextSample() -> double;

private:
    constexpr static const double DEFAULT_GENERATOR_FREQUENCY = 441.0;
    constexpr static const double DEFAULT_SAMPLING_FREQUENCY = 44100.0;
    constexpr static const double PI_TIMES_2 = 6.283185307179586476925286766559;

    double lastSignalGeneratorArgument = 0.0F;
    double lastSignalGeneratorSample = 0.0F;
    SignalGeneratorParameters::Waveform waveform;
    double signalFrequency;
    double samplingFrequency;

    std::mt19937 randomMersenneTwisterEngine;
    std::uniform_real_distribution<double> randomDistribution;

    auto generateSine() -> double;
    auto generateTriangle() -> double;
    auto generateRamp() -> double;
    auto generateSquare() -> double;
    auto generateNoise() -> double;
};
