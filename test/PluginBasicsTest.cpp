#include "../src/PluginProcessor.h"
#include <catch2/catch_all.hpp>

SpectronAudioProcessor testPlugin;

TEST_CASE("Plugin instance name", "[name]")
{
  CHECK_THAT(testPlugin.getName().toStdString(),
             Catch::Matchers::Equals("Speclet"));
}