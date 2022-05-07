#include "../src/dsp/transformations/WaveletTransformation.h"
#include "../src/dsp/transformations/WaveletParameters.h"
#include "../src/dsp/windowing/WindowParameters.h"

#include <catch2/catch_test_macros.hpp>

#include <string>

SCENARIO("Wavelet Transformation") {
    auto gui = juce::ScopedJuceInitialiser_GUI{};
    auto samplingRate = 44100;
    Transformation::ResolutionType resolution = 4096;
    auto windowFunction = WindowParameters::WindowFunction::BLACKMAN_HARRIS;
    auto waveletBase = WaveletParameters::WaveletBase::VAIDYANATHAN_18;

    GIVEN("Wavelet Transform is created") {
        auto transformation = WaveletTransformation(samplingRate, resolution, windowFunction, waveletBase);
        WHEN("name is requested") {
            THEN("it should return the correct name") {
                REQUIRE(transformation.getName() == std::string("Fast Wavelet Transformation"));
            }
        }
        WHEN("window function is requested") {
            THEN("it should return the correct window function") {
                REQUIRE(transformation.getWindowFunction()->getName() == std::string("Blackman-Harris"));
            }
        }
    }
}