//Most of the window function implementations in this class had been taken out of the fft c program
//from George B. Moody (GNU General Public License, email: george@mit.edu, http://www.physionet.org/)
#include "WindowFunctionFactory.h"
#include "WindowFunction.h"
#include "WindowFunctions.h"

#include "../../parameter/SpecletParameters.h"
#include "WindowParameters.h"
#include <assert.h>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

auto WindowFunctionFactory::getSingletonInstance() -> WindowFunctionFactory & {
    static WindowFunctionFactory singletonInstance;// Guaranteed to be destroyed. Instantiated on first use.
    return singletonInstance;
}

auto WindowFunctionFactory::getWindow(const WindowParameters::WindowFunction &newWindowFunction, unsigned long resolution) -> std::shared_ptr<WindowFunction> {
    Key key{newWindowFunction, resolution};
    auto lowerBoundIterator = windowFunctionsCache.lower_bound(key);
    auto keyComparator = windowFunctionsCache.key_comp();
    if ((lowerBoundIterator != windowFunctionsCache.end()) && !(keyComparator(key, lowerBoundIterator->first))) {
        // Window function exists in cache: return it
        return lowerBoundIterator->second;
    }
    // Window function does not exist in cache: create it.
    auto newWindow = createWindow(key.windowFunction, key.resolution);
    windowFunctionsCache[key] = newWindow;
    return newWindow;
}

auto WindowFunctionFactory::createWindow(const WindowParameters::WindowFunction &newWindowFunction, unsigned long resolution) -> std::shared_ptr<WindowFunction> {
    switch (newWindowFunction) {
        case WindowParameters::WindowFunction::BARLETT: {
            return std::make_shared<WindowBartlett>(resolution);
        }
        case WindowParameters::WindowFunction::BLACKMAN: {
            return std::make_shared<WindowBlackman>(resolution);
        }
        case WindowParameters::WindowFunction::BLACKMAN_HARRIS: {
            return std::make_shared<WindowBlackmanHarris>(resolution);
        }
        case WindowParameters::WindowFunction::HAMMING: {
            return std::make_shared<WindowHamming>(resolution);
        }
        case WindowParameters::WindowFunction::HANN: {
            return std::make_shared<WindowHann>(resolution);
        }
        case WindowParameters::WindowFunction::PARZEN: {
            return std::make_shared<WindowParzen>(resolution);
        }
        case WindowParameters::WindowFunction::RECTANGULAR: {
            return std::make_shared<WindowRectangular>(resolution);
        }
        case WindowParameters::WindowFunction::WELCH: {
            return std::make_shared<WindowWelch>(resolution);
        }
        case WindowParameters::WindowFunction::NUMBER_OF_OPTIONS:
        default: {
            using WindowFunctionValueType = std::underlying_type<WindowParameters::WindowFunction>::type;
            throw std::invalid_argument("Unknown windowing function " + std::to_string(static_cast<WindowFunctionValueType>(newWindowFunction)));
        }
    }
}

void WindowFunctionFactory::clearCache() {
    windowFunctionsCache.clear();
}