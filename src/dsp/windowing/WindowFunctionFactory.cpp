//Most of the window function implementations in this class had been taken out of the fft c program
//from George B. Moody (GNU General Public License, email: george@mit.edu, http://www.physionet.org/)
#include "WindowFunctionFactory.h"
#include "WindowFunctions.h"

#include "../../plugin/SpectronParameters.h"

auto WindowFunctionFactory::getSingletonInstance() -> WindowFunctionFactory & {
    static WindowFunctionFactory singletonInstance;// Guaranteed to be destroyed. Instantiated on first use.
    return singletonInstance;
}

WindowFunctionFactory::~WindowFunctionFactory() {
    deleteWindowFunctionsMap();
}

auto WindowFunctionFactory::createWindowFunction(int windowFunctionNr, long resolution) -> WindowFunction * {
    WindowFunction *result = readWindowFunctionFromMap(windowFunctionNr, resolution);
    if (result != nullptr) {
        return result;
    }

    switch (windowFunctionNr) {
        case SpectronParameters::WINDOWING_BARTLETT: {
            result = (WindowFunction *) (new WindowBartlett(resolution));
            break;
        }
        case SpectronParameters::WINDOWING_BLACKMAN: {
            result = (WindowFunction *) (new WindowBlackman(resolution));
            break;
        }
        case SpectronParameters::WINDOWING_BLACKMAN_HARRIS: {
            result = (WindowFunction *) (new WindowBlackmanHarris(resolution));
            break;
        }
        case SpectronParameters::WINDOWING_HAMMING: {
            result = (WindowFunction *) (new WindowHamming(resolution));
            break;
        }
        case SpectronParameters::WINDOWING_HANNING: {
            result = (WindowFunction *) (new WindowHanning(resolution));
            break;
        }
        case SpectronParameters::WINDOWING_PARZEN: {
            result = (WindowFunction *) (new WindowParzen(resolution));
            break;
        }
        case SpectronParameters::WINDOWING_SQUARE: {
            result = (WindowFunction *) (new WindowSquare(resolution));
            break;
        }
        case SpectronParameters::WINDOWING_WELCH: {
            result = (WindowFunction *) (new WindowWelch(resolution));
            break;
        }
        default: {
            bool windowing_function_unknown = false;
            assert(windowing_function_unknown);
        }
    }

    writeWindowFunctionIntoMap(windowFunctionNr, resolution, result);
    return result;
}

WindowFunction *WindowFunctionFactory::readWindowFunctionFromMap(int windowFunctionNr, long resolution) {
    TWindowFunctionsMapIterator windowingFunctionsIterator;
    TResolutionsMapIterator resolutionsIterator;

    //Get the windowing function
    windowingFunctionsIterator = windowingFunctions.find(windowFunctionNr);
    if (windowingFunctionsIterator == windowingFunctions.end()) {
        return nullptr;
    }

    //Get the resolutions map that belongs to the windowing function
    TResolutionsMap resolutions = windowingFunctionsIterator->second;

    //Get the pointer to the windowing function class that belongs to that specific window function and resolution
    resolutionsIterator = resolutions.find(resolution);
    if (resolutionsIterator == resolutions.end()) {
        return nullptr;
    }
    return resolutionsIterator->second;
}

void WindowFunctionFactory::writeWindowFunctionIntoMap(int windowFunctionNr, long resolution, WindowFunction *pWindowFunction) {
    TWindowFunctionsMapIterator windowingFunctionsIterator;
    TResolutionsMapIterator resolutionsIterator;

    //Get the windowing function
    windowingFunctionsIterator = windowingFunctions.find(windowFunctionNr);

    if (windowingFunctionsIterator == windowingFunctions.end()) {
        //window function not found: new resolution map + new window function map entry
        TResolutionsKeyValue resolutionMapEntry(resolution, pWindowFunction);
        TResolutionsMap resolutions;
        resolutions.insert(resolutionMapEntry);

        //insert the new window function entry incl. the resolution
        TWindowFunctionsKeyValue windowFunctionsMapEntry(windowFunctionNr, resolutions);
        windowingFunctions.insert(windowFunctionsMapEntry);
    } else {
        //window function found. does resolution exist in resolutionMap?
        TResolutionsMap resolutions = windowingFunctionsIterator->second;

        resolutionsIterator = resolutions.find(windowFunctionNr);
        if (resolutionsIterator == resolutions.end()) {
            //resolution entry not found
            TResolutionsKeyValue resolutionsKeyValue(resolution, pWindowFunction);
            resolutions.insert(resolutionsKeyValue);

            //erase old windowing function entry to get space for the new
            //one with one more resolution entry
            windowingFunctions.erase(windowingFunctionsIterator);

            //insert the new window function entry incl. the old and new resolutions
            TWindowFunctionsKeyValue windowFunctionsMapEntry(windowFunctionNr, resolutions);
            windowingFunctions.insert(windowFunctionsMapEntry);
        } else {
            //resolution entry also exists, so there is nothing to be done
        }
    }
}

//delete all cached windowing functions
void WindowFunctionFactory::deleteWindowFunctionsMap() {
    TWindowFunctionsMap windowFunctions = windowingFunctions;
    auto windowingFunctionsIterator = windowFunctions.begin();

    while (windowingFunctionsIterator != windowFunctions.end()) {
        TResolutionsMap resolutions = windowingFunctionsIterator->second;
        auto resolutionsIterator = resolutions.begin();
        while (resolutionsIterator != resolutions.end()) {
            delete (resolutionsIterator->second);
            resolutionsIterator++;
        }
        windowingFunctionsIterator++;
    }
}