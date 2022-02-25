//Most of the window function implementations in this class had been taken out of the fft c program
//from George B. Moody (GNU General Public License, email: george@mit.edu, http://www.physionet.org/)
#include "WindowFunctions.h"
#include "../plugin/SpectronParameters.h"

#include <cmath>
#include <complex>


#define PI 3, 14159265358979323846 // 1 * PI
#define PIx2 6.28318530717958647692// 2 * PI

//------------------------------------------------------------------------------------//
// Interface as abstract class                                                        //
//------------------------------------------------------------------------------------//
WindowFunction::WindowFunction(long resolution) {
    mResolution = resolution;
    windowFunctionFactors = NULL;
}

WindowFunction::~WindowFunction() {
    if (windowFunctionFactors != nullptr) {
        free(windowFunctionFactors);
    }
}

double WindowFunction::getFactor(int index) {
    jassert(index >= 0);
    jassert(index < mResolution);

    return getWindow()[index];
}

double *WindowFunction::getWindow() {
    if (windowFunctionFactors == nullptr) {
        windowFunctionFactors = (double *) malloc(sizeof(double) * mResolution);

        for (int i = 0; i < mResolution; i++) {
            windowFunctionFactors[i] = calculateFactor(i);
        }
    }
    return windowFunctionFactors;
}

//------------------------------------------------------------------------------------//
// Factory                                                                            //
//------------------------------------------------------------------------------------//
// Singleton instance variable (only one instance of this class)
WindowFunctionsFactory *WindowFunctionsFactory::pSingletonInstance = 0;

WindowFunctionsFactory *WindowFunctionsFactory::getSingletonInstance() {
    // Method to get the single instance of this class (Singleton)
    if (WindowFunctionsFactory::pSingletonInstance == 0) {
        WindowFunctionsFactory::pSingletonInstance = new WindowFunctionsFactory();
    }
    return WindowFunctionsFactory::pSingletonInstance;
}

void WindowFunctionsFactory::destruct() {
    //deletes singleton instance - can be reactivated by calling getSingletonInstance
    if (pSingletonInstance == nullptr) {
        return;
    }

    pSingletonInstance->deleteWindowFunctionsMap();

    delete (pSingletonInstance);
    pSingletonInstance = 0;
}

WindowFunction *WindowFunctionsFactory::createWindowFunction(int windowFunctionNr, long resolution) {
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

WindowFunction *WindowFunctionsFactory::readWindowFunctionFromMap(int windowFunctionNr, long resolution) {
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

void WindowFunctionsFactory::writeWindowFunctionIntoMap(int windowFunctionNr, long resolution, WindowFunction *pWindowFunction) {
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
void WindowFunctionsFactory::deleteWindowFunctionsMap() {
    TWindowFunctionsMap windowFunctions = WindowFunctionsFactory::pSingletonInstance->windowingFunctions;
    auto windowingFunctionsIterator = windowFunctions.begin();

    while (windowingFunctionsIterator != windowFunctions.end()) {
        TResolutionsMap resolutions = windowingFunctionsIterator->second;
        TResolutionsMapIterator resolutionsIterator = resolutions.begin();
        while (resolutionsIterator != resolutions.end()) {
            delete (resolutionsIterator->second);
            resolutionsIterator++;
        }
        windowingFunctionsIterator++;
    }
}


//------------------------------------------------------------------------------------//
// Interface implementations                                                          //
//------------------------------------------------------------------------------------//
/* See Oppenheim & Schafer, Digital Signal Processing, p. 241 (1st ed.) */
double WindowBartlett::calculateFactor(int index) {
    double a = 2.0 / (mResolution - 1), w;

    if ((w = index * a) > 1.0) w = 2.0 - w;

    return (w);
};

/* See Oppenheim & Schafer, Digital Signal Processing, p. 242 (1st ed.) */
double WindowBlackman::calculateFactor(int index) {
    double a = PIx2 / (mResolution - 1), w;

    w = 0.42 - 0.5 * cos(a * index) + 0.08 * cos(2 * a * index);
    return (w);
};

/* See Harris, F.J., "On the use of windows for harmonic analysis with the
discrete Fourier transform", Proc. IEEE, Jan. 1978 */
double WindowBlackmanHarris::calculateFactor(int index) {
    double a = PIx2 / (mResolution - 1), w;

    w = 0.35875 - 0.48829 * cos(a * index) + 0.14128 * cos(2 * a * index) - 0.01168 * cos(3 * a * index);
    return (w);
};

/* See Oppenheim & Schafer, Digital Signal Processing, p. 242 (1st ed.) */
double WindowHamming::calculateFactor(int index) {
    double a = PIx2 / (mResolution - 1), w;

    w = 0.54 - 0.46 * cos(a * index);
    return (w);
};

/* See Oppenheim & Schafer, Digital Signal Processing, p. 242 (1st ed.)
The second edition of Numerical Recipes calls this the "Hann" window. */
double WindowHanning::calculateFactor(int index) {
    double a = PIx2 / (mResolution - 1), w;

    w = 0.5 - 0.5 * cos(a * index);
    return (w);
};

/* See Press, Flannery, Teukolsky, & Vetterling, Numerical Recipes in C,
p. 442 (1st ed.) */
double WindowParzen::calculateFactor(int index) {
    double a = (mResolution - 1) / 2.0, w;

    if ((w = (index - a) / (a + 1)) > 0.0) {
        w = 1 - w;
    } else {
        w = 1 + w;
    }
    return (w);
};

/* See any of the above references. */
double WindowSquare::calculateFactor(int index) {
    return (1.0);
};

/* See Press, Flannery, Teukolsky, & Vetterling, Numerical Recipes in C,
p. 442 (1st ed.) or p. 554 (2nd ed.) */
double WindowWelch::calculateFactor(int index) {
    double a = (mResolution - 1) / 2.0, w;

    w = (index - a) / (a + 1);
    w = 1 - w * w;

    return (w);
};