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

    struct WaveformNames {
        static std::map<Waveform, std::string_view> createMap() {
            return {
                    {Waveform::SINE, "Sine"},
                    {Waveform::TRIANGLE, "Triangle"},
                    {Waveform::RAMP, "Sawtooth"},
                    {Waveform::SQUARE, "Rectangle"},
                    {Waveform::NOISE, "Noise"},
            };
        }
        static const std::map<Waveform, std::string_view> map;
    };

    inline const std::map<Waveform, std::string_view> WaveformNames::map = WaveformNames::createMap();
}// namespace SignalGeneratorParameters