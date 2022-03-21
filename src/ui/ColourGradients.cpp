#include "ColourGradients.h"
#include "../plugin/SpectronParameters.h"

auto ColourGradients::forIndex(int index) -> juce::ColourGradient {
    if (index == SpectronParameters::COLORMODE_BLUE) {
        return BLUE;
    }
    if (index == SpectronParameters::COLORMODE_GREEN) {
        return GREEN;
    }
    if (index == SpectronParameters::COLORMODE_FIRE) {
        return fire();
    }
    if (index == SpectronParameters::COLORMODE_RAINBOW) {
        return rainbow();
    }

    DBG("juce::ColourGradient ColourGradients::get(): index not found (replaced by blue)! i=" + juce::String(index));
    return BLUE;
}

auto ColourGradients::fire() noexcept -> juce::ColourGradient {
    DBG("juce::ColourGradient ColourGradients::fire()");
	juce::ColourGradient gradient = ColourGradient();

    gradient.addColour(1.0F, juce::Colours::yellow);
    gradient.addColour(0.9F, juce::Colours::red);
    gradient.addColour(0.6F, juce::Colours::darkred);
    gradient.addColour(0.3F, juce::Colours::darkviolet);
    gradient.addColour(0.0F, juce::Colours::black);

    return gradient;
}

auto ColourGradients::rainbow() noexcept -> juce::ColourGradient {
    DBG("juce::ColourGradient ColourGradients::rainbow()");
	juce::ColourGradient gradient = ColourGradient();

    gradient.addColour(1.0F, juce::Colours::red);
    gradient.addColour(0.8F, juce::Colours::yellow);
    gradient.addColour(0.6F, juce::Colours::green);
    gradient.addColour(0.4F, juce::Colours::darkblue);
    gradient.addColour(0.2F, juce::Colours::darkviolet);
    gradient.addColour(0.0F, juce::Colours::black);

    return gradient;
}

auto ColourGradients::fadeToBlack(const Colour& colour) noexcept -> juce::ColourGradient {
    DBG("juce::ColourGradient ColourGradients::blue()");
    juce::ColourGradient gradient = ColourGradient();
    gradient.addColour(1.0F, colour);
    gradient.addColour(0.9F, colour.darker(0.1F));
    gradient.addColour(0.0F, juce::Colours::black);
    return gradient;
}