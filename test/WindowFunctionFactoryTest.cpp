#include "../src/dsp/windowing/WindowFunctionFactory.h"
#include "../src/dsp/windowing/WindowParameters.h"

#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_contains.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include <exception>

SCENARIO("Window Function Factory") {
    WindowFunctionFactory &factory = WindowFunctionFactory::getSingletonInstance();

    GIVEN("get window") {
        const auto resolution = 4096;

        WHEN("window functions are cached") {
            // Generates a list of all window functions/methods and calls the tests methods below for each of them.
            // See: https://github.com/catchorg/Catch2/blob/devel/docs/generators.md
            auto windowMethod = static_cast<WindowParameters::WindowFunction>(GENERATE(range(1, static_cast<int>(WindowParameters::WindowFunction::NUMBER_OF_OPTIONS))));
            auto window = factory.getWindow(windowMethod, resolution);
            REQUIRE(window != nullptr);

            THEN("window " + std::string(window->getName()) + " is only created once") {
                auto reloadedWindow = factory.getWindow(windowMethod, resolution);
                REQUIRE(window == reloadedWindow);
                factory.clearCache();
            }
            THEN("window " + std::string(window->getName()) + " is newly created when resolution changes") {
                const auto anotherResolution = 2048;
                auto windowWithAnotherResolution = factory.getWindow(windowMethod, anotherResolution);
                REQUIRE(window != windowWithAnotherResolution);
                factory.clearCache();
            }
        }
        WHEN("invalid window number is given") {
            WindowParameters::WindowFunction invalidWindowMethod = WindowParameters::WindowFunction::NUMBER_OF_OPTIONS;
            THEN("exception is thrown") {
                auto expectedErrorMessage = "Unknown windowing function " + std::to_string(static_cast<int>(invalidWindowMethod));
                REQUIRE_THROWS_WITH(factory.getWindow(invalidWindowMethod, resolution), expectedErrorMessage);
            }
        }
    }
}