//Most of the window function implementations in this class had been taken out of the fft c program
//from George B. Moody (GNU General Public License, email: george@mit.edu, http://www.physionet.org/)
#include "WindowFunction.h"
#include <cassert>

WindowFunction::WindowFunction(unsigned long newResolution) : resolution(newResolution) {
}

auto WindowFunction::getFactor(unsigned long index) -> double {
    assert(index >= 0);
    assert(index < resolution);
    getWindow();
    return factors[index];
}

auto WindowFunction::getWindow() -> double * {
    if (factors.empty()) {
        factors.reserve(resolution);
        for (unsigned long i = 0; i < resolution; i++) {
            factors.push_back(calculateFactor(i));
        }
    }
    return factors.data();
}