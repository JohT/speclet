#pragma once

#if _DEBUG
#define _PERFORMACETEST 1
#endif
#if _LOGTOFILE
#define LOG(logtext)                                    \
    {                                                   \
        JUCE_NAMESPACE::String tempLogBuf;              \
        tempLogBuf << logtext;                          \
        JUCE_NAMESPACE::Logger::writeToLog(tempLogBuf); \
    }
#else
#define LOG(logtext)
#endif

#include "JuceHeader.h"

/**
 * @brief Uses the JUCE PerformanceCounter to log performance data.
 */
class PerformanceTimer {

public:
    PerformanceTimer(const juce::String &counterName,
                     int runsPerPrintout = 1000,
                     const juce::File &loggingFile = juce::File())
#if _PERFORMACETEST
        : performanceCounter(juce::PerformanceCounter(counterName, runsPerPrintout, loggingFile))
#endif
    {
    }

    ~PerformanceTimer() {
#if _PERFORMACETEST
        performanceCounter.printStatistics();
#endif
    }

    PerformanceTimer() = delete;
    PerformanceTimer(const PerformanceTimer &) = delete;
    PerformanceTimer(PerformanceTimer &&) = delete;
    auto operator=(const PerformanceTimer &) -> PerformanceTimer & = delete;
    auto operator=(PerformanceTimer &&) -> PerformanceTimer & = delete;

    void start() noexcept {
#if _PERFORMACETEST
        performanceCounter.start();
#endif
    }

    void stop() {
#if _PERFORMACETEST
        performanceCounter.stop();
#endif
    }

private:
#if _PERFORMACETEST
    PerformanceCounter performanceCounter;
#endif
};