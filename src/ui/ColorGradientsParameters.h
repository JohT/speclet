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

    struct ColorModeNames {
        static std::map<ColorMode, std::string_view> createMap() {
            return {
                    {ColorMode::BLUE, "Blue"},
                    {ColorMode::GREEN, "Green"},
                    {ColorMode::RAINBOW, "Rainbow"},
                    {ColorMode::FIRE, "Fire"},
            };
        }
        static const std::map<ColorMode, std::string_view> map;
    };

    inline const std::map<ColorMode, std::string_view> ColorModeNames::map = ColorModeNames::createMap();

}// namespace ColorGradientsParameters