#include "../src/SpecletPluginProcessor.h"
#include <catch2/catch_all.hpp>

SpecletAudioProcessor testPlugin;

TEST_CASE("Plugin instance name", "[name]")
{
  CHECK_THAT(testPlugin.getName().toStdString(),
             Catch::Matchers::Equals("Speclet"));
}