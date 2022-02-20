#include "PerformanceManager.h"

// Singleton instance variable (only one instance of this class) 
PerformanceManager* PerformanceManager::singletonInstance = 0;

PerformanceManager* PerformanceManager::getSingletonInstance() {
// Method to get the single instance of this class (Singleton)
	if (PerformanceManager::singletonInstance == 0) {
		PerformanceManager::singletonInstance = new PerformanceManager();
	}
	return PerformanceManager::singletonInstance;
}

void PerformanceManager::destruct() {
	if (!singletonInstance) return;
	singletonInstance->deletePerformanceManager();	
	delete(singletonInstance);
	singletonInstance = 0;
}

void PerformanceManager::start(juce::String name, int runsPerPrintout, const juce::File &loggingFile) {
	#if _PERFORMACETEST	
		TIterator iterator = map.find(name);
		#if _LOGTOFILE
			if (iterator == map.end()) {
				if (loggingFile == juce::File::nonexistent) {
					add(name, runsPerPrintout, juce::File("c:/temp/speclet.log"));
				} else {
					add(name, runsPerPrintout, loggingFile);
				}
			}
		#else
			if (iterator == map.end()) add(name, runsPerPrintout, loggingFile);
		#endif
		get(name)->start();
	#endif
}

void PerformanceManager::stop(juce::String name) {
	#if _PERFORMACETEST	
		TIterator iterator = map.find(name);
		if (iterator == map.end()) {
			DBG(T("PerformanceManager::stop(") + name + T("): PerformanceCounter not found"));
			return;
		}
		get(name)->stop();
	#endif
}

void PerformanceManager::add(juce::String name, int runsPerPrintout, const juce::File &loggingFile) {
	TIterator iterator = map.find(name);
	if (iterator != map.end()) return;
	
	PerformanceCounter* performanceCounter = new PerformanceCounter(name, runsPerPrintout, loggingFile);
	TKeyValue keyValue(name, performanceCounter);
	map.insert(keyValue);
}

juce::PerformanceCounter* PerformanceManager::get(juce::String name) {
	TIterator iterator = map.find(name);
	return iterator->second;
}

void PerformanceManager::deletePerformanceManager() {
	TIterator iterator = map.begin();

	while (iterator != map.end()) {
		delete(iterator->second);	
		iterator++;
	}
}