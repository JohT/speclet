#include "..\config\JucePluginCharacteristics.h"
#include "JuceHeader.h"
#include "juce_StandaloneFilterWindow.h"

class SpectronJuceStandalone : public JUCEApplication {
public:
	SpectronJuceStandalone() {}

	void initialise(const String& commandLineParameters)
	{
		ApplicationProperties::getInstance()->setStorageParameters (
			JucePlugin_Name + juce::String("Standalone"),
			juce::String::empty, 
			JucePlugin_Name,
			400, 
			PropertiesFile::storeAsXML);
		filterWindow = new StandaloneFilterWindow (getApplicationName(), Colours::black);
		filterWindow->setTitleBarButtonsRequired (DocumentWindow::allButtons, false);
		filterWindow->setVisible (true);
		filterWindow->setResizable (true, true);
	}

	void shutdown() {
		deleteAndZero (filterWindow);
	}

	const String getApplicationName() {
		return JucePlugin_Name + juce::String(" (Standalone) v") + JucePlugin_VersionString;
	}

	const String getApplicationVersion() {
		return JucePlugin_VersionString;
	}

private:
	StandaloneFilterWindow *filterWindow;
};

START_JUCE_APPLICATION (SpectronJuceStandalone)