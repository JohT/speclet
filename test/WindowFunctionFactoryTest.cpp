#include "../src/dsp/windowing/WindowFunctionFactory.h"
#include <catch2/catch_all.hpp>

SCENARIO("Window Function Factory") {
    WindowFunctionFactory &factory = WindowFunctionFactory::getSingletonInstance();

    GIVEN("get window") {
        const auto resolution = 4096;

        WHEN("window functions are cached") {
            // Generates a list of all window functions/methods and calls the tests methods below for each of them.
            // See: https://github.com/catchorg/Catch2/blob/devel/docs/generators.md
            WindowFunctionFactory::Method windowMethod = static_cast<WindowFunctionFactory::Method>(GENERATE(range(1, static_cast<int>(WindowFunctionFactory::Method::NUMBER_OF_OPTIONS))));
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
    }
}