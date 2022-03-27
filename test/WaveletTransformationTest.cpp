#include "../src/PluginProcessor.h"
#include <catch2/catch_all.hpp>
#include <string>

SCENARIO("Wavelet Transformation") {
    auto samplingRate = 44100;
    Transformation::ResolutionType resolution = 1024;
    Transformation *transformation = TransformationFactory::getSingletonInstance().createTransformation(
            TransformationFactory::Type::FAST_WAVELET_TRANSFORM,
            samplingRate,
            resolution,
            WindowFunctionFactory::Method::RECTANGULAR,
            AbstractWaveletTransformation::WaveletBase::VAIDYANATHAN_18);
    REQUIRE(transformation != nullptr);
    //TODO (JohT): Implement test
    GIVEN("get window") {

        WHEN("window functions are cached") {
            THEN("window is newly created when resolution changes") {
            }
        }
    }
}