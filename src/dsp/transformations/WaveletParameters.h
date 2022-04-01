#pragma once

#include <map>
#include <string_view>

namespace WaveletParameters {

    enum class WaveletBase {
        DAUBECHIES_02 = 1,
        DAUBECHIES_04,
        DAUBECHIES_06,
        DAUBECHIES_08,
        DAUBECHIES_10,
        DAUBECHIES_12,
        DAUBECHIES_14,
        DAUBECHIES_16,
        DAUBECHIES_18,
        DAUBECHIES_20,
        COIFMAN_06,
        COIFMAN_12,
        COIFMAN_18,
        COIFMAN_24,
        COIFMAN_30,
        BEYLKIN_18,
        VAIDYANATHAN_18,

        NUMBER_OF_OPTIONS,
        HAAR = DAUBECHIES_02,
        DEFAULT = VAIDYANATHAN_18
    };

    inline static const std::map<WaveletBase, std::string_view> waveletBaseNames = {
            {WaveletBase::HAAR, "Haar (2)"},
            {WaveletBase::DAUBECHIES_02, "Daubechies (2)"},
            {WaveletBase::DAUBECHIES_04, "Daubechies (4)"},
            {WaveletBase::DAUBECHIES_06, "Daubechies (6)"},
            {WaveletBase::DAUBECHIES_08, "Daubechies (8)"},
            {WaveletBase::DAUBECHIES_10, "Daubechies (10)"},
            {WaveletBase::DAUBECHIES_12, "Daubechies (12)"},
            {WaveletBase::DAUBECHIES_14, "Daubechies (14)"},
            {WaveletBase::DAUBECHIES_16, "Daubechies (16)"},
            {WaveletBase::DAUBECHIES_18, "Daubechies (18)"},
            {WaveletBase::DAUBECHIES_20, "Daubechies (20)"},
            {WaveletBase::COIFMAN_06, "Coifman (6)"},
            {WaveletBase::COIFMAN_12, "Coifman (12)"},
            {WaveletBase::COIFMAN_18, "Coifman (18)"},
            {WaveletBase::COIFMAN_24, "Coifman (24)"},
            {WaveletBase::COIFMAN_30, "Coifman (30)"},
            {WaveletBase::BEYLKIN_18, "Beylkin (18)"},
            {WaveletBase::VAIDYANATHAN_18, "Vaidyanathan (18)"}

    };

    enum class ResolutionRatioOption {
        EQUAL = 99,
        TIME_X4 = -2,
        TIME_X2 = -1,
        FREQUENCY_X2 = 1,
        FREQUENCY_X4 = 2,

        NUMBER_OF_OPTIONS = 5,
        DEFAULT = EQUAL
    };

    inline static const std::map<ResolutionRatioOption, std::string_view> resolutionRatioOptionNames = {
            {ResolutionRatioOption::EQUAL, "Freq/Time x1"},
            {ResolutionRatioOption::TIME_X4, "Time x4"},
            {ResolutionRatioOption::TIME_X2, "Time x2"},
            {ResolutionRatioOption::FREQUENCY_X2, "Freq x2"},
            {ResolutionRatioOption::FREQUENCY_X4, "Freq x4"}

    };

}// namespace WaveletParameter