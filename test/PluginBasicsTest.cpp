#include "../src/SpecletPluginProcessor.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

TEST_CASE("Plugin instance name", "[name]") {
    auto gui = juce::ScopedJuceInitialiser_GUI{};
    SpecletAudioProcessor testPlugin;
    CHECK_THAT(testPlugin.getName().toStdString(), Catch::Matchers::StartsWith("Speclet"));
}