//Most of the window function implementations in this class had been taken out of the fft c program
//from George B. Moody (GNU General Public License, email: george@mit.edu, http://www.physionet.org/)
#include "WindowFunctionFactory.h"
#include "WindowFunction.h"
#include "WindowFunctions.h"

#include "../../plugin/SpectronParameters.h"
#include <assert.h>
#include <cstddef>
#include <memory>
#include <utility>

auto WindowFunctionFactory::getSingletonInstance() -> WindowFunctionFactory & {
    static WindowFunctionFactory singletonInstance;// Guaranteed to be destroyed. Instantiated on first use.
    return singletonInstance;
}

auto WindowFunctionFactory::getWindow(const Method &method, unsigned long resolution) -> std::shared_ptr<WindowFunction> {
    Key key{method, resolution};
    auto lowerBoundIterator = windowFunctionsCache.lower_bound(key);
    auto keyComparator = windowFunctionsCache.key_comp();
    if ((lowerBoundIterator != windowFunctionsCache.end()) && !(keyComparator(key, lowerBoundIterator->first))) {
        // Window function exists in cache: return it
        return lowerBoundIterator->second;
    }
    // Window function does not exist in cache: create it.
    auto newWindow = createWindow(key.method, key.resolution);
    windowFunctionsCache[key] = newWindow;
    return newWindow;
}

auto WindowFunctionFactory::createWindow(const Method &method, unsigned long resolution) -> std::shared_ptr<WindowFunction> {
    switch (method) {
        case Method::BARLETT: {
            return std::make_shared<WindowBartlett>(resolution);
        }
        case Method::BLACKMAN: {
            return std::make_shared<WindowBlackman>(resolution);
        }
        case Method::BLACKMAN_HARRIS: {
            return std::make_shared<WindowBlackmanHarris>(resolution);
        }
        case Method::HAMMING: {
            return std::make_shared<WindowHamming>(resolution);
        }
        case Method::HANN: {
            return std::make_shared<WindowHann>(resolution);
        }
        case Method::PARZEN: {
            return std::make_shared<WindowParzen>(resolution);
        }
        case Method::RECTANGULAR: {
            return std::make_shared<WindowRectangular>(resolution);
        }
        case Method::WELCH: {
            return std::make_shared<WindowWelch>(resolution);
        }
        case Method::NUMBER_OF_OPTIONS:
        default: {
            bool windowing_function_unknown = false;
            assert(windowing_function_unknown);
            return nullptr;
        }
    }
}

void WindowFunctionFactory::clearCache() {
    windowFunctionsCache.clear();
}