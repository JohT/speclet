#include "../src/utilities/RenderingHelper.h"

#include <catch2/catch_test_macros.hpp>

SCENARIO("Rendering Helper") {
    GIVEN("pixel to index") {
        RenderingHelper renderingHelper;
        SpectralDataInfo info(44100, 4096, 4096);
        const auto windowHeight = 100;
        const bool linearFrequency = false;
        const bool logarithmicFrequency = true;

        WHEN("pixel value isn't within the valid range") {
            THEN("index is zero for pixel = 0") {
                auto index = renderingHelper.pixelToIndex(0, windowHeight, info, linearFrequency);
                REQUIRE(index == 0);
            }
            THEN("index is zero for pixel < 0") {
                auto index = renderingHelper.pixelToIndex(-1, windowHeight, info, linearFrequency);
                REQUIRE(index == 0);
            }
        }
        WHEN("linear frequency scale") {
            THEN("index is resolution dived by window height for pixel = 1") {
                auto index = renderingHelper.pixelToIndex(1, windowHeight, info, linearFrequency);
                REQUIRE(index == 41);
            }
            THEN("index is half resolution for pixel = half window height") {
                auto index = renderingHelper.pixelToIndex(windowHeight / 2, windowHeight, info, linearFrequency);
                REQUIRE(index == 2048);
            }
            THEN("index is nearly the resolution for pixel = window height") {
                auto index = renderingHelper.pixelToIndex(windowHeight, windowHeight, info, linearFrequency);
                REQUIRE(index >= 4095);
                REQUIRE(index <= 4096);
            }
        }
        WHEN("logarithmic frequency scale") {
            THEN("index is resolution dived by window height for pixel = 1") {
                auto index = renderingHelper.pixelToIndex(1, windowHeight, info, logarithmicFrequency);
                REQUIRE(index == 2);
            }
            THEN("index doubles when pixel is a decade higher") {
                auto index = renderingHelper.pixelToIndex(10, windowHeight, info, logarithmicFrequency);
                REQUIRE(index == 4);
            }
            THEN("index is nearly the resolution for pixel = window height") {
                auto index = renderingHelper.pixelToIndex(windowHeight, windowHeight, info, logarithmicFrequency);
                REQUIRE(index >= 4095);
                REQUIRE(index <= 4096);
            }
        }
    }
}

// These hidden tests (marked with the tag [.debug]) are used to debug the RenderingHelper.
// They are not part of the automated tests.
SCENARIO("Rendering Helper Debugging", "[.debug]") {
    GIVEN("pixel to index") {
        RenderingHelper renderingHelper;
        SpectralDataInfo info(44100, 4096, 4096);
        const auto windowHeight = 100;

        WHEN("linear frequency scale") {
            THEN("printing the index values for debugging purposes") {
                printf("printing pixelToIndex linear values for debugging purposes:\n");
                for (auto pixel = 0; pixel <= windowHeight; pixel++) {
                    auto index = renderingHelper.pixelToIndex(pixel, windowHeight, info, false);
                    printf("[%3d]: %3lu |", pixel, index);
                    if (((pixel + 1) % 4) == 0) {
                        printf("\n");
                    }
                }
            }
        }
        WHEN("logarithmic frequency scale") {
            THEN("printing the index values for debugging purposes") {
                printf("\n\nprinting pixelToIndex logarithmic values for debugging purposes:\n");
                for (auto pixel = 0; pixel <= windowHeight; pixel++) {
                    auto index = renderingHelper.pixelToIndex(pixel, windowHeight, info, true);
                    printf("[%3d]: %3lu |", pixel, index);
                    if (((pixel + 1) % 4) == 0) {
                        printf("\n");
                    }
                }
            }
        }
    }
}