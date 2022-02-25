#include "ColourGradients.h"
#include "../plugin/SpectronParameters.h"

// Singleton instance variable (only one instance of this class) 
ColourGradients* ColourGradients::singletonInstance = 0;

ColourGradients* ColourGradients::getSingletonInstance() {
// Method to get the single instance of this class (Singleton)
	if (ColourGradients::singletonInstance == 0) {
		ColourGradients::singletonInstance = new ColourGradients();
	}
	return ColourGradients::singletonInstance;
}

void ColourGradients::destruct() {
	if (!singletonInstance) return;
	delete(singletonInstance);
	singletonInstance = 0;
}

juce::ColourGradient ColourGradients::get(int index) {
	if			(index == SpectronParameters::COLORMODE_BLUE)		{
		return getBlue();
	}
	else if	(index == SpectronParameters::COLORMODE_GREEN)		{
		return getGreen();
	}
	else if	(index == SpectronParameters::COLORMODE_FIRE)		{
		return getFire();
	}
	else if	(index == SpectronParameters::COLORMODE_RAINBOW)	{
		return getRainbow();
	}

    DBG("juce::ColourGradient ColourGradients::get(): index not found (replaced by blue)! i=" + juce::String(index));
    return getBlue();
}

juce::ColourGradient ColourGradients::getBlue() {
	juce::ColourGradient gradient = ColourGradient();

	gradient.addColour(1.0f, Colour::fromRGB(40, 40, 255));
	gradient.addColour(0.0f, Colour::fromRGB(0, 0, 0));

	return gradient;
}

juce::ColourGradient ColourGradients::getGreen() {
	juce::ColourGradient gradient = ColourGradient();

	gradient.addColour(1.0f, Colour::fromRGB(40, 255, 40));
	gradient.addColour(0.0f, Colour::fromRGB(0, 0, 0));

	return gradient;
}

juce::ColourGradient ColourGradients::getFire() {
	juce::ColourGradient gradient = ColourGradient();

	gradient.addColour(1.0f, juce::Colours::yellow);
	gradient.addColour(0.8f, juce::Colours::red);
	gradient.addColour(0.4f, juce::Colours::darkred);
	gradient.addColour(0.2f, juce::Colours::darkblue);
	gradient.addColour(0.0f, juce::Colours::black);

	return gradient;
}

juce::ColourGradient ColourGradients::getRainbow() {
	juce::ColourGradient gradient = ColourGradient();

	gradient.addColour(1.0f, juce::Colours::red);
	gradient.addColour(0.8f, juce::Colours::yellow);
	gradient.addColour(0.6f, juce::Colours::green);
	gradient.addColour(0.4f, juce::Colours::darkblue);
	gradient.addColour(0.2f, juce::Colours::darkviolet);
	gradient.addColour(0.0f, juce::Colours::black);
	
	return gradient;
}
