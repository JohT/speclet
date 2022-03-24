#include "WaveletPacketBestBasisTransformation.h"

#include "AbstractWaveletTransformation.h"
#include "../../utilities/PerformanceLogger.h"
#include "JuceHeader.h"
#include <cmath>
#include <math.h>

WaveletPacketBestBasisTransformation::WaveletPacketBestBasisTransformation(
        double newSamplingRate,
        ResolutionType newResolution,
        WindowFunctionFactory::Method newWindowFunction,
        WaveletBase newWaveletBaseType)
    : AbstractWaveletTransformation(newSamplingRate, newResolution, newWindowFunction, newWaveletBaseType),
      //Time and frequency resolution can't be estimated since they change dynamically. Assume same values as for the DWT.
      spectralDataInfo(SpectralDataInfo(newSamplingRate, newResolution, newResolution, newResolution / 2)) {

    DBG("WaveletPacketBestBasisTransformation constructor: " + getSpectralDataInfo().toString());
    setReady();
    setCalculated();
}

WaveletPacketBestBasisTransformation::~WaveletPacketBestBasisTransformation() {
    setReady(false);
    DBG("WaveletPacketBestBasisTransformation destructed");
}

void WaveletPacketBestBasisTransformation::calculate() {
    fillDWTInput();

    //to hold the result of the wavelet packet transformation (=DWPT coeffs)
    ArrayTreePer outDWPT(getWaveletFilterTreeMaxLevel());

    //DWPT (discrete wavelet packet transform), periodic
    analyse(outDWPT);
    sortWaveletFilterTreeByScaleDescending(outDWPT);

    //calculate noise level for a chosen SNR
    int signalToNoiseRatioInDecibel = Constants::SIGNAL_TO_NOISE_RATIO;
    auto resolution = static_cast<double>(getResolution());
    double noiseLevel = sqrt(1.0F / (signalToNoiseRatioInDecibel * resolution));
    const double oracCostFactor = (1.0 + sqrt(2.0F * log(static_cast<double>(getWaveletFilterTreeMaxLevel()) * resolution)));//D&J Best Wavelet (BWB)

    //Find the best basis
    HedgePer bestBasis;
    extractBestBasis(outDWPT, bestBasis, noiseLevel, oracCostFactor);

    if (bestBasis.num_of_levels <= 1) {
        DBG("WaveletPacketBestBasisTransformation::calculate best basis could not be found!");
        extractSpectrum(outDWPT);
    } else {
        extractSpectrum(outDWPT, bestBasis);
    }
}

// ----------------------------------------------------------------------------

void WaveletPacketBestBasisTransformation::getCosts(const ArrayTreePer &a, Tree &t, costFunAdv costFunction, const real_number &sigma, const real_number &factor) {
    getCostsHelp(a, &(t.root), costFunction, sigma, factor, 0, 0);
    t.maxlevel = a.maxlevel;
}


void WaveletPacketBestBasisTransformation::getCostsHelp(const ArrayTreePer &a, Node<real_number> **ptr,
                                                        costFunAdv costFunction, const real_number &sigma, const real_number &factor,
                                                        const integer_number &level, const integer_number &b) {
    if (level <= a.maxlevel) {
        real_number cost = (this->*costFunction)(a.block_start(level, b), a.block_length(level), sigma, factor, a.dim);
        (*ptr) = new Node<real_number>(cost, nullptr, nullptr);
        assert(*ptr);
        if (level < a.maxlevel) {
            getCostsHelp(a, &((*ptr)->left), costFunction, sigma, factor, level + 1, b << 1U);
            getCostsHelp(a, &((*ptr)->right), costFunction, sigma, factor, level + 1, (b << 1U) | 1);
        }
    }
}

auto WaveletPacketBestBasisTransformation::oracCostAdv(const real_number *data, const integer_number &n, const real_number &sigma, const real_number &factor, const integer_number &/* k */) -> real_number {
    real_number cost = 0;
    real_number var = sigma * sigma;
    real_number temp = 0;

    for (int i = 0; i < n; i++) {
        temp = data[i] * data[i];

        if (temp >= var * factor * factor) {
            cost += var;
        } else {
            cost += temp;
        }
    }
    return cost;
}

void WaveletPacketBestBasisTransformation::extractBestBasis(const ArrayTreePer &a, HedgePer &h, const double &sigma, const double &factor) {
    LOG_PERFORMANCE_OF_SCOPE("WaveletPacketBestBasisTransformation extractBestBasis");
    assert(a.origin);
    Tree b;

    // Get a member function reference to the cost function
    real_number (WaveletPacketBestBasisTransformation::*costFunction)(const real_number *data, const integer_number &n, const real_number &sigma, const real_number &factor, const integer_number &k);
    costFunction = &WaveletPacketBestBasisTransformation::oracCostAdv;

    getCosts(a, b, costFunction, sigma, factor);
    h.dim = a.dim;
    BestBasis(h, b);
    h.origin = new real_number[static_cast<unsigned long>(h.dim)];
    ExtractHedge(h, a);
}