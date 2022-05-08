#pragma once

#include <functional>
#include <map>
#include <string_view>

namespace WindowParameters {

    /**
     * @brief Available window functions
     */
    enum class WindowFunction {
        BARLETT = 1,
        BLACKMAN,
        BLACKMAN_HARRIS,
        HAMMING,
        HANN,
        PARZEN,
        WELCH,
        RECTANGULAR,

        NUMBER_OF_OPTIONS,
        DEFAULT = BLACKMAN_HARRIS
    };

    struct WindowFunctionNames {
        static std::map<WindowFunction, std::string_view> createMap() {
            return {
                    {WindowFunction::BARLETT, "Barlett"},
                    {WindowFunction::BLACKMAN, "Blackman"},
                    {WindowFunction::BLACKMAN_HARRIS, "Blackman-Harris"},
                    {WindowFunction::HAMMING, "Hamming"},
                    {WindowFunction::HANN, "Hann"},
                    {WindowFunction::PARZEN, "Parzen"},
                    {WindowFunction::WELCH, "Welch"},
                    {WindowFunction::RECTANGULAR, "Rectangular"},
                };
        }
        static const std::map<WindowFunction, std::string_view> map;
    };

    inline const std::map<WindowFunction, std::string_view> WindowFunctionNames::map = WindowFunctionNames::createMap();

}// namespace WindowParameters