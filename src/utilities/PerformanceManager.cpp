#include "PerformanceManager.h"

auto PerformanceManager::getSingletonInstance() -> PerformanceManager & {
    static PerformanceManager singletonInstance;// Guaranteed to be destroyed. Instantiated on first use.
    return singletonInstance;
}

void PerformanceManager::start(const juce::String &name, int runsPerPrintout, const juce::File &loggingFile) {
#if _PERFORMACETEST
    auto iterator = map.find(name);
#if _LOGTOFILE
    if (iterator == map.end()) {
        if (loggingFile == juce::File::nonexistent) {
            add(name, runsPerPrintout, juce::File("c:/temp/speclet.log"));
        } else {
            add(name, runsPerPrintout, loggingFile);
        }
    }
#else
    if (iterator == map.end()) {
        add(name, runsPerPrintout, loggingFile);
    }
#endif
    get(name).start();
#endif
}

void PerformanceManager::stop(const juce::String &name) {
#if _PERFORMACETEST
    auto iterator = map.find(name);
    if (iterator == map.end()) {
        DBG("PerformanceManager::stop(" + name + "): PerformanceCounter not found");
        return;
    }
    get(name).stop();
#endif
}

void PerformanceManager::add(const juce::String &name, int runsPerPrintout, const juce::File &loggingFile) {
    auto iterator = map.find(name);
    if (iterator != map.end()) {
        return;
    }

    PerformanceCounter performanceCounter(name, runsPerPrintout, loggingFile);
    //auto *performanceCounter = new PerformanceCounter(name, runsPerPrintout, loggingFile);
    TKeyValue keyValue(name, performanceCounter);
    map.insert(keyValue);
}

auto PerformanceManager::get(const juce::String &name) -> juce::PerformanceCounter & {
    auto iterator = map.find(name);
    return iterator->second;
}