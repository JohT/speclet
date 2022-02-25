#include "WaveletPacketBestBasisTransformation.h"
#include "JuceHeader.h"

WaveletPacketBestBasisTransformation::WaveletPacketBestBasisTransformation(
        double samplingRate,
        long resolution,
        int windowFunctionNr,
        int waveletBaseTypeNr)
    : AbstractWaveletTransformation(samplingRate, resolution, windowFunctionNr, waveletBaseTypeNr) {
    DBG("WaveletPacketBestBasisTransformation constructor started");

    mSpectralDataInfo = NULL;
    mFrequencyResolution = resolution;//can not be estimated, since it changes dynamically. set to best possible resolution
    mTimeResolution = resolution / 2; //can not be estimated, since it changes dynamically. set to best possible resolution
    mSpectralDataInfo = new SpectralDataInfo(samplingRate, resolution, mFrequencyResolution, mTimeResolution);

    DBG("WaveletPacketBestBasisTransformation constructor freqResolution=" +
        juce::String(mFrequencyResolution) +
        ",tres=" + juce::String(mTimeResolution) +
        ",fs=" + juce::String(mSamplingRate) +
        ",res=" + juce::String(mResolution));

    ready = true;
    calculated = true;
};

WaveletPacketBestBasisTransformation::~WaveletPacketBestBasisTransformation() {
    ready = false;
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
    int SNR = 48;
    double noiseLevel = sqrt(1.0 / (SNR * mResolution));
    double oracCostFactor = (1.0 + sqrt((double) 2 * log((double) mDwtMaxLevel * mResolution)));//D&J Best Wavelet (BWB)

    //Find the best basis
    HedgePer bestBasis;
    ExtractBestBasis(outDWPT, bestBasis, noiseLevel, oracCostFactor);

    if (bestBasis.num_of_levels <= 1) {
        DBG("WaveletPacketBestBasisTransformation::calculate best basis could not be found!");
        bestBasis = *mConstantLevelsHedge;
    }
    extractSpectrum(outDWPT, bestBasis);
}