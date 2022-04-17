#pragma once

#include <map>
#include <string_view>

namespace TransformationParameters {

    enum class Type {
        FAST_FOURIER_TRANSFORM = 1,
        FAST_WAVELET_TRANSFORM,
        FAST_WAVELET_PACKET_TRANSFORM,
        FAST_WAVELET_PACKET_BEST_BASIS_TRANSFORM,
        BYPASS,

        NUMBER_OF_OPTIONS,
        DEFAULT = FAST_FOURIER_TRANSFORM
    };
    struct TypeNames {
        static std::map<Type, std::string_view> createMap() {
            return {
                    {Type::FAST_FOURIER_TRANSFORM, "FFT"},
                    {Type::FAST_WAVELET_TRANSFORM, "FWT"},
                    {Type::FAST_WAVELET_PACKET_TRANSFORM, "WPT"},
                    {Type::FAST_WAVELET_PACKET_BEST_BASIS_TRANSFORM, "WPT BestBasis"},
                    {Type::BYPASS, "Off"},
            };
        }
        static const std::map<Type, std::string_view> map;
    };

    inline const std::map<Type, std::string_view> TypeNames::map = TypeNames::createMap();

}// namespace TransformationParameters