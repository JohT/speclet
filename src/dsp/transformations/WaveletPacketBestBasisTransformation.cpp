#include "WaveletPacketBestBasisTransformation.h"

#include "AbstractWaveletTransformation.h"
#include "JuceHeader.h"
#include <math.h>


WaveletPacketBestBasisTransformation::WaveletPacketBestBasisTransformation(
        double newSamplingRate,
        ResolutionType newResolution,
        int windowFunctionNr,
        WaveletBase newWaveletBaseType)
    : AbstractWaveletTransformation(newSamplingRate, newResolution, windowFunctionNr, newWaveletBaseType),
    //Time and frequency resolution can't be estimated since they change dynamically. Assume same values as for the DWT.
    spectralDataInfo(SpectralDataInfo(newSamplingRate, newResolution, newResolution, newResolution / 2)) {
    
    DBG("WaveletPacketBestBasisTransformation constructor: " + getSpectralDataInfo().toString());
    setReady();
    setCalculated();
};

WaveletPacketBestBasisTransformation::~WaveletPacketBestBasisTransformation() {
    setReady(false);
    DBG("WaveletPacketBestBasisTransformation destructed");
}

void WaveletPacketBestBasisTransformation::calculate() {
    fillDWTInput();

    //to hold the result of the wavelet packet transformation (=DWPT coeffs)
    ArrayTreePer outDWPT(mDwtMaxLevel);

    //DWPT (discrete wavelet packet transform), periodic
    Analysis(*mDwtInput, outDWPT, mDwtFilterH, mDwtFilterG, ConvDecPer);
    sortDWPTTreeByScaleDescending(outDWPT);

    //calculate noise level for a chosen SNR //TODO should be provided in a better way e.g. by measurement...
    int signalToNoiseRatioInDecibel = 48;
    auto resolution = getResolution();
    double noiseLevel = sqrt(1.0 / (signalToNoiseRatioInDecibel * resolution));
    const double oracCostFactor = (1.0 + sqrt((double) 2 * log((double) mDwtMaxLevel * resolution)));//D&J Best Wavelet (BWB)

    //Find the best basis
    HedgePer bestBasis;
    extractBestBasis(outDWPT, bestBasis, noiseLevel, oracCostFactor);

    if (bestBasis.num_of_levels <= 1) {
        DBG("WaveletPacketBestBasisTransformation::calculate best basis could not be found!");
        bestBasis = *mConstantLevelsHedge;
    }
    extractSpectrum(outDWPT, bestBasis);
}

// ----------------------------------------------------------------------------

void WaveletPacketBestBasisTransformation::getCosts(const ArrayTreePer &a, Tree &t, costFunAdv f, const real_number &sigma, const real_number &factor) {
    getCostsHelp(a, &(t.root), f, sigma, factor, 0, 0);
    t.maxlevel = a.maxlevel;
}


void WaveletPacketBestBasisTransformation::getCostsHelp(const ArrayTreePer &a, Node<real_number> **ptr,
                                                        costFunAdv f, const real_number &sigma, const real_number &factor,
                                                        const integer_number &l, const integer_number &b) {
    if (l <= a.maxlevel) {
        real_number cost = (this->*f)(a.block_start(l, b), a.block_length(l), sigma, factor, a.dim);
        (*ptr) = new Node<real_number>(cost, 0, 0);
        assert(*ptr);
        if (l < a.maxlevel) {
            getCostsHelp(a, &((*ptr)->left), f, sigma, factor, l + 1, b << 1);
            getCostsHelp(a, &((*ptr)->right), f, sigma, factor, l + 1, (b << 1) | 1);
        }
    }
}

auto WaveletPacketBestBasisTransformation::oracCostAdv(const real_number *data, const integer_number &n, const real_number &sigma, const real_number &factor, const integer_number &k) -> real_number {
    //	real_DWT factor= (1.0+sqrt((double)2*log((double)mDWT_maxLevel*mResolution)));//D&J Best Wavelet (BWB)
    real_number cost = 0;
    real_number var = sigma * sigma;
    real_number temp;

    for (int i = 0; i < n; i++) {
        temp = data[i] * data[i];

        if (temp >= var * factor * factor) cost += var;
        else
            cost += temp;
    }
    return cost;
}

void WaveletPacketBestBasisTransformation::extractBestBasis(const ArrayTreePer &a, HedgePer &h, const double &sigma, const double &factor) {
    assert(a.origin);
    Tree b;

    // Get a member function reference to the cost function
    real_number (WaveletPacketBestBasisTransformation::*costFunction)(const real_number *data, const integer_number &n, const real_number &sigma, const real_number &factor, const integer_number &k);
    costFunction = &WaveletPacketBestBasisTransformation::oracCostAdv;
   
    getCosts(a, b, costFunction, sigma, factor);
    h.dim = a.dim;
    BestBasis(h, b);
    h.origin = new real_number[h.dim];
    ExtractHedge(h, a);
}