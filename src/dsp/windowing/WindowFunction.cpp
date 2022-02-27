//Most of the window function implementations in this class had been taken out of the fft c program
//from George B. Moody (GNU General Public License, email: george@mit.edu, http://www.physionet.org/)
#include "WindowFunction.h"
#include <cassert>

WindowFunction::WindowFunction(long newResolution) : resolution(newResolution), windowFunctionFactors(nullptr) {
    
}

WindowFunction::~WindowFunction() {
    if (windowFunctionFactors != nullptr) {
        free(windowFunctionFactors);
    }
}

double WindowFunction::getFactor(int index) {
    assert(index >= 0);
    assert(index < resolution);

    return getWindow()[index];
}

auto WindowFunction::getWindow() -> double * {
    if (windowFunctionFactors == nullptr) {
        windowFunctionFactors = static_cast<double *>(malloc(sizeof(double) * resolution));

        for (int i = 0; i < resolution; i++) {
            windowFunctionFactors[i] = calculateFactor(i);
        }
    }
    return windowFunctionFactors;
}

auto WindowFunction::getWindowSize() -> double {
    return static_cast<double>(resolution - 1);
}