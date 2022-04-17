#pragma once

#include <map>
#include <string_view>

namespace SpecletDrawerParameters {

    enum class Axis {
        LINEAR = 1,
        LOGARITHMIC,

        NUMBER_OF_OPTIONS,
        DEFAULT = LOGARITHMIC
    };

    struct AxisNames {
        static std::map<Axis, std::string_view> createMap() {
            return {
                    {Axis::LINEAR, "linear"},
                    {Axis::LOGARITHMIC, "logarithmic"},
            };
        }
        static const std::map<Axis, std::string_view> map;
    };

    inline const std::map<Axis, std::string_view> AxisNames::map = AxisNames::createMap();

}// namespace SpecletDrawerParameters