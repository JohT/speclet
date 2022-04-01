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

    inline static const std::map<Axis, std::string_view> axisNames = {
            {Axis::LINEAR, "linear"},
            {Axis::LOGARITHMIC, "logarithmic"},

    };

}// namespace SpecletDrawerParameters