#include "ColourGradients.h"
#include "../plugin/SpectronParameters.h"

auto ColourGradients::getSingletonInstance() -> ColourGradients & {
    static ColourGradients singletonInstance;// Guaranteed to be destroyed. Instantiated on first use.
    return singletonInstance;
}

auto ColourGradients::forIndex(int index) -> const juce::ColourGradient {
    if (index == SpectronParameters::COLORMODE_BLUE) {
        return blue();
    }
    if (index == SpectronParameters::COLORMODE_GREEN) {
        return green();
    }
    if (index == SpectronParameters::COLORMODE_FIRE) {
        return fire();
    }
    if (index == SpectronParameters::COLORMODE_RAINBOW) {
        return rainbow();
    }

    DBG("juce::ColourGradient ColourGradients::get(): index not found (replaced by blue)! i=" + juce::String(index));
    return blue();
}

auto ColourGradients::blue() -> const juce::ColourGradient {
    DBG("juce::ColourGradient ColourGradients::blue()");
    juce::ColourGradient gradient = ColourGradient();

    gradient.addColour(1.0F, Colour::fromRGB(45, 45, 255));
    gradient.addColour(0.9F, Colour::fromRGB(40, 40, 255));
    gradient.addColour(0.0F, juce::Colours::black);

    return gradient;
}

auto ColourGradients::green() -> const juce::ColourGradient {
	DBG("juce::ColourGradient ColourGradients::green()");
    juce::ColourGradient gradient = ColourGradient();

    gradient.addColour(1.0F, Colour::fromRGB(45, 255, 45));
    gradient.addColour(0.9F, Colour::fromRGB(40, 255, 40));
    gradient.addColour(0.0F, juce::Colours::black);

    return gradient;
}

auto ColourGradients::fire() -> const juce::ColourGradient {
    DBG("juce::ColourGradient ColourGradients::fire()");
	juce::ColourGradient gradient = ColourGradient();

    gradient.addColour(1.0F, juce::Colours::yellow);
    gradient.addColour(0.8F, juce::Colours::red);
    gradient.addColour(0.4F, juce::Colours::darkred);
    gradient.addColour(0.2F, juce::Colours::darkblue);
    gradient.addColour(0.0F, juce::Colours::black);

    return gradient;
}

auto ColourGradients::rainbow() -> const juce::ColourGradient {
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
