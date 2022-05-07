#include "ColourGradients.h"
#include "../plugin/SpecletParameters.h"
#include "ColorGradientsParameters.h"

auto ColourGradients::forIndex(int colorModeIndex) -> juce::ColourGradient {
    auto colorMode = static_cast<ColorGradientsParameters::ColorMode>(colorModeIndex);
    if (colorMode == ColorGradientsParameters::ColorMode::BLUE) {
        return BLUE;
    }
    if (colorMode == ColorGradientsParameters::ColorMode::GREEN) {
        return GREEN;
    }
    if (colorMode == ColorGradientsParameters::ColorMode::RAINBOW) {
        return rainbow();
    }
    if (colorMode == ColorGradientsParameters::ColorMode::FIRE) {
        return fire();
    }
    DBG("juce::ColourGradient ColourGradients::get(): index not found (replaced by blue)! i=" + juce::String(colorModeIndex));
    return BLUE;
}

auto ColourGradients::fire() noexcept -> juce::ColourGradient {
    DBG("juce::ColourGradient ColourGradients::fire()");
	auto gradient = juce::ColourGradient();

    gradient.addColour(1.0F, juce::Colours::yellow);
    gradient.addColour(0.9F, juce::Colours::red);
    gradient.addColour(0.6F, juce::Colours::darkred);
    gradient.addColour(0.3F, juce::Colours::darkviolet);
    gradient.addColour(0.0F, juce::Colours::black);

    return gradient;
}

auto ColourGradients::rainbow() noexcept -> juce::ColourGradient {
    DBG("juce::ColourGradient ColourGradients::rainbow()");
	auto gradient = juce::ColourGradient();

    gradient.addColour(1.0F, juce::Colours::red);
    gradient.addColour(0.8F, juce::Colours::yellow);
    gradient.addColour(0.6F, juce::Colours::green);
    gradient.addColour(0.4F, juce::Colours::darkblue);
    gradient.addColour(0.2F, juce::Colours::darkviolet);
    gradient.addColour(0.0F, juce::Colours::black);

    return gradient;
}

auto ColourGradients::fadeToBlack(const juce::Colour& colour) noexcept -> juce::ColourGradient {
    DBG("juce::ColourGradient ColourGradients::blue()");
    auto gradient = juce::ColourGradient();
    gradient.addColour(1.0F, colour);
    gradient.addColour(0.9F, colour.darker(0.1F));
    gradient.addColour(0.0F, juce::Colours::black);
    return gradient;
}