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
#include "../plugin/SpectronParameters.h"
#include <random>

class SignalGenerator {
public:
    explicit SignalGenerator(
            short signalType = SpectronParameters::GENERATOR_DEFAULT,
            double signalFrequency = 441.0,
            double samplingFrequency = 44100.0);
            
    auto getNextSample() -> double;

private:
    auto generateSine() -> double;
    auto generateTriangle() -> double;
    auto generateRamp() -> double;
    auto generateSquare() -> double;
    auto generateNoise() -> double;

    double mLastSignalGeneratorArgument;
    double mLastSignalGeneratorSample;
    int mSignalType;
    double mSignalFrequency;
    double mSamplingFrequency;

    std::mt19937 randomMersenneTwisterEngine;
    std::uniform_real_distribution<double> randomDistribution;
};
