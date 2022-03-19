#include "SignalGenerator.h"
#include <cfloat>
#include <random>

SignalGenerator::SignalGenerator(double newSamplingFrequency, Waveform newWaveform, double newSignalFrequency)
    : waveform(newWaveform),
      signalFrequency(newSignalFrequency),
      samplingFrequency(newSamplingFrequency),
      randomMersenneTwisterEngine(std::random_device{}()),
      randomDistribution(std::uniform_real_distribution<double>(-1.0, std::nextafter(1.0, DBL_MAX))) {
}

auto SignalGenerator::getNextSample() -> double {
    switch (waveform) {
        case Waveform::SINE:
            return generateSine();
        case Waveform::TRIANGLE:
            return generateTriangle();
        case Waveform::RAMP:
            return generateRamp();
        case Waveform::SQUARE:
            return generateSquare();
        case Waveform::NOISE:
            return generateNoise();
        case Waveform::NUMBER_OF_OPTIONS:
        default: {
            //ignore if wrong: use sine as default
            return generateSine();
        }
    }
}

auto SignalGenerator::generateSine() -> double {
    double value = sin(lastSignalGeneratorArgument);
    lastSignalGeneratorSample = value;

    lastSignalGeneratorArgument = lastSignalGeneratorArgument + (PI_TIMES_2 / samplingFrequency * signalFrequency);
    if (lastSignalGeneratorArgument > PI_TIMES_2) {
        lastSignalGeneratorArgument -= PI_TIMES_2;
    }

    return value;
}

auto SignalGenerator::generateTriangle() -> double {
    if (lastSignalGeneratorArgument == 0.0) {
        //first call - short cut
        lastSignalGeneratorArgument = 4.0 * signalFrequency / samplingFrequency;
        lastSignalGeneratorSample = 0.0;
        return 0.0;
    }

    double value = lastSignalGeneratorSample + lastSignalGeneratorArgument;

    if ((lastSignalGeneratorArgument >= 0.0) && (value > 1.0)) {
        lastSignalGeneratorArgument *= -1.0;
        value = lastSignalGeneratorSample + lastSignalGeneratorArgument;
    }
    if ((lastSignalGeneratorArgument < 0.0) && (value < -1.0)) {
        lastSignalGeneratorArgument *= -1.0;
        value = lastSignalGeneratorSample + lastSignalGeneratorArgument;
    }

    lastSignalGeneratorSample = value;
    return value;
}

auto SignalGenerator::generateRamp() -> double {
    if (lastSignalGeneratorArgument == 0.0) {
        //first call - short cut
        lastSignalGeneratorArgument = 2.0 * signalFrequency / samplingFrequency;
        lastSignalGeneratorSample = -1.0;
        return -1.0;
    }

    double value = lastSignalGeneratorSample + lastSignalGeneratorArgument;

    if (value > 1.0) {
        value = -1.0;
    }

    lastSignalGeneratorSample = value;
    return value;
}

auto SignalGenerator::generateSquare() -> double {
    double value = 0.0;

    if (lastSignalGeneratorArgument <= (0.5 / signalFrequency)) {
        value = 1.0;
        lastSignalGeneratorArgument = lastSignalGeneratorArgument + 1.0 / samplingFrequency;
    } else {
        if (lastSignalGeneratorArgument < (1.0 / signalFrequency)) {
            value = -1.0;
            lastSignalGeneratorArgument = lastSignalGeneratorArgument + 1.0 / samplingFrequency;
        } else {
            value = 1.0;
            lastSignalGeneratorArgument = 0.0;
        }
    }

    lastSignalGeneratorSample = value;
    return value;
}

auto SignalGenerator::generateNoise() -> double {
    return randomDistribution(randomMersenneTwisterEngine);
}