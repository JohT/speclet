#include "SignalGenerator.h"
#include <cfloat>
#include <random>

#define PIx2 6.2831853071795865

//TODO(johnny) revisite random number generator
SignalGenerator::SignalGenerator(short signalType, double signalFrequency, double samplingFrequency)
    : mSignalType(signalType), mSignalFrequency(signalFrequency), mSamplingFrequency(samplingFrequency), mLastSignalGeneratorArgument(0.0F), mLastSignalGeneratorSample(0.0F) {

    if (signalType == SpectronParameters::GENERATOR_NOISE) {
        std::random_device randomDevice;
        randomMersenneTwisterEngine = std::mt19937(randomDevice());
        randomDistribution = std::uniform_real_distribution<double>(-1.0, std::nextafter(1.0, DBL_MAX));
    }
}

auto SignalGenerator::getNextSample() -> double {
    switch (mSignalType) {
        case SpectronParameters::GENERATOR_SINE:
            return generateSine();
        case SpectronParameters::GENERATOR_TRIANGLE:
            return generateTriangle();
        case SpectronParameters::GENERATOR_RAMP:
            return generateRamp();
        case SpectronParameters::GENERATOR_SQUARE:
            return generateSquare();
        case SpectronParameters::GENERATOR_NOISE:
            return generateNoise();
        default: {
            //ignore if wrong: use sine as default
            return generateSine();
        }
    }
}

auto SignalGenerator::generateSine() -> double {
    double value = sin(mLastSignalGeneratorArgument);
    mLastSignalGeneratorSample = value;

    mLastSignalGeneratorArgument = mLastSignalGeneratorArgument + (PIx2 / mSamplingFrequency * mSignalFrequency);
    if (mLastSignalGeneratorArgument > PIx2) {
        mLastSignalGeneratorArgument -= PIx2;
    }

    return value;
}

auto SignalGenerator::generateTriangle() -> double {
    if (mLastSignalGeneratorArgument == 0.0) {
        //first call - short cut
        mLastSignalGeneratorArgument = 4.0 * mSignalFrequency / mSamplingFrequency;
        mLastSignalGeneratorSample = 0.0;
        return 0.0;
    }

    double value = mLastSignalGeneratorSample + mLastSignalGeneratorArgument;

    if ((mLastSignalGeneratorArgument >= 0.0) && (value > 1.0)) {
        mLastSignalGeneratorArgument *= -1.0;
        value = mLastSignalGeneratorSample + mLastSignalGeneratorArgument;
    }
    if ((mLastSignalGeneratorArgument < 0.0) && (value < -1.0)) {
        mLastSignalGeneratorArgument *= -1.0;
        value = mLastSignalGeneratorSample + mLastSignalGeneratorArgument;
    }

    mLastSignalGeneratorSample = value;
    return value;
}

auto SignalGenerator::generateRamp() -> double {
    if (mLastSignalGeneratorArgument == 0.0) {
        //first call - short cut
        mLastSignalGeneratorArgument = 2.0 * mSignalFrequency / mSamplingFrequency;
        mLastSignalGeneratorSample = -1.0;
        return -1.0;
    }

    double value = mLastSignalGeneratorSample + mLastSignalGeneratorArgument;

    if (value > 1.0) {
        value = -1.0;
    }

    mLastSignalGeneratorSample = value;
    return value;
}

auto SignalGenerator::generateSquare() -> double {
    double value = 0.0;

    if (mLastSignalGeneratorArgument <= (0.5 / mSignalFrequency)) {
        value = 1.0;
        mLastSignalGeneratorArgument = mLastSignalGeneratorArgument + 1.0 / mSamplingFrequency;
    } else {
        if (mLastSignalGeneratorArgument < (1.0 / mSignalFrequency)) {
            value = -1.0;
            mLastSignalGeneratorArgument = mLastSignalGeneratorArgument + 1.0 / mSamplingFrequency;
        } else {
            value = 1.0;
            mLastSignalGeneratorArgument = 0.0;
        }
    }

    mLastSignalGeneratorSample = value;
    return value;
}

auto SignalGenerator::generateNoise() -> double {
    return randomDistribution(randomMersenneTwisterEngine);
}