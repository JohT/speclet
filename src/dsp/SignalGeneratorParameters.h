#pragma once

#include <map>
#include <string_view>

namespace SignalGeneratorParameters {

    enum class Waveform {
        SINE = 1,
        TRIANGLE,
        RAMP,
        SQUARE,
        NOISE,

        NUMBER_OF_OPTIONS,
        DEFAULT = SINE
    };

    inline static const std::map<Waveform, std::string_view> typeNames = {
            {Waveform::SINE, "Sine"},
            {Waveform::TRIANGLE, "Triangle"},
            {Waveform::RAMP, "Sawtooth"},
            {Waveform::SQUARE, "Rectangle"},
            {Waveform::NOISE, "Noise"}
    };

}