//Most of the window function implementations in this class had been taken out of the fft c program
//from George B. Moody (GNU General Public License, email: george@mit.edu, http://www.physionet.org/)
#include "WindowFunctions.h"
#include "../../plugin/SpecletParameters.h"

/* See Oppenheim & Schafer, Digital Signal Processing, p. 241 (1st ed.) */
auto WindowBartlett::calculateFactor(unsigned long index) -> double {
    double a = 2.0 / getWindowSize();
    double indexDouble = static_cast<double>(index);
    double w = indexDouble * a;
    if (w > 1.0) {
        w = 2.0 - w;
    }
    return w;
}

/* See Oppenheim & Schafer, Digital Signal Processing, p. 242 (1st ed.) */
auto WindowBlackman::calculateFactor(unsigned long index) -> double {
    double a = PIx2 / getWindowSize();
    double indexDouble = static_cast<double>(index);
    double w = 0.42 - 0.5 * cos(a * indexDouble) + 0.08 * cos(2 * a * indexDouble);
    return w;
}

/* See Harris, F.J., "On the use of windows for harmonic analysis with the
discrete Fourier transform", Proc. IEEE, Jan. 1978 */
auto WindowBlackmanHarris::calculateFactor(unsigned long index) -> double {
    double a = PIx2 / getWindowSize();
    double indexDouble = static_cast<double>(index);
    double w = 0.35875 - 0.48829 * cos(a * indexDouble) + 0.14128 * cos(2 * a * indexDouble) - 0.01168 * cos(3 * a * indexDouble);
    return w;
}

/* See Oppenheim & Schafer, Digital Signal Processing, p. 242 (1st ed.) */
auto WindowHamming::calculateFactor(unsigned long index) -> double {
    double a = PIx2 / getWindowSize();
    double indexDouble = static_cast<double>(index);
    double w = 0.54 - 0.46 * cos(a * indexDouble);
    return w;
}

/* See Oppenheim & Schafer, Digital Signal Processing, p. 242 (1st ed.)
The second edition of Numerical Recipes calls this the "Hann" window. */
auto WindowHann::calculateFactor(unsigned long index) -> double {
    double a = PIx2 / getWindowSize();
    double indexDouble = static_cast<double>(index);
    double w = 0.5 - 0.5 * cos(a * indexDouble);
    return w;
}

/* See Press, Flannery, Teukolsky, & Vetterling, Numerical Recipes in C,
p. 442 (1st ed.) */
auto WindowParzen::calculateFactor(unsigned long index) -> double {
    double a = getWindowSize() / 2.0;
    double indexDouble = static_cast<double>(index);
    double w = (indexDouble - a) / (a + 1);
    if (w > 0.0) {
        w = 1 - w;
    } else {
        w = 1 + w;
    }
    return w;
}

/* See any of the above references. */
auto WindowRectangular::calculateFactor(unsigned long /* index */) -> double {
    return (1.0);
}

/* See Press, Flannery, Teukolsky, & Vetterling, Numerical Recipes in C,
p. 442 (1st ed.) or p. 554 (2nd ed.) */
auto WindowWelch::calculateFactor(unsigned long index) -> double {
    double a = getWindowSize() / 2.0, w;
    double indexDouble = static_cast<double>(index);
    w = (indexDouble - a) / (a + 1);
    w = 1 - w * w;

    return w;
}