#pragma once

#include <map>
#include <string_view>

namespace ColorGradientsParameters {

    enum class ColorMode {
        BLUE = 1,
        GREEN,
        RAINBOW,
        FIRE,

        NUMBER_OF_OPTIONS,
        DEFAULT = FIRE
    };

    inline static const std::map<ColorMode, std::string_view> colorModeNames = {
            {ColorMode::BLUE, "Blue"},
            {ColorMode::GREEN, "Green"},
            {ColorMode::RAINBOW, "Rainbow"},
            {ColorMode::FIRE, "Fire"}};

}// namespace ColorGradientsParameters