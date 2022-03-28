// #include "../src/dsp/transformations/Transformation.h"
// #include "../src/dsp/transformations/TransformationFactory.h"
#include "../src/dsp/transformations/WaveletTransformation.h"
#include "catch2/matchers/catch_matchers.hpp"
#include <catch2/catch_all.hpp>
#include <string>

SCENARIO("Wavelet Transformation") {
    auto samplingRate = 44100;
    Transformation::ResolutionType resolution = 4096;
    auto windowFunction = WindowFunctionFactory::Method::BLACKMAN_HARRIS;
    auto waveletBase = AbstractWaveletTransformation::WaveletBase::VAIDYANATHAN_18;

    GIVEN("Wavelet Transform is created") {
        WaveletTransformation transformation = WaveletTransformation(samplingRate, resolution, windowFunction, waveletBase);
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