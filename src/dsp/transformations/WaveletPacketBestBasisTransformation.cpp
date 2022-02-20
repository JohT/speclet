#include "WaveletPacketBestBasisTransformation.h"
#include "../../libs/juce/JuceLibraryCode/JuceHeader.h"

WaveletPacketBestBasisTransformation::WaveletPacketBestBasisTransformation(
	double samplingRate, 
	long resolution, 
	int windowFunctionNr,
	int waveletBaseTypeNr) 
	: AbstractWaveletTransformation(samplingRate, resolution, windowFunctionNr, waveletBaseTypeNr) 
{
	DBG(T("WaveletPacketBestBasisTransformation constructor started"));
	
	mSpectralDataInfo		= NULL;	
	mFrequencyResolution	= resolution;		//can not be estimated, since it changes dynamically. set to best possible resolution
	mTimeResolution		= resolution / 2;	//can not be estimated, since it changes dynamically. set to best possible resolution
	mSpectralDataInfo		= new SpectralDataInfo(samplingRate, resolution, mFrequencyResolution, mTimeResolution);

	DBG(T("WaveletPacketBestBasisTransformation constructor freqResolution=")	+ 
						  juce::String(mFrequencyResolution)								+
		T(",tres=")	+ juce::String(mTimeResolution)										+
		T(",fs=")	+ juce::String(mSamplingRate)											+
		T(",res=")	+ juce::String(mResolution)
	);

	ready			= true;
	calculated	= true;
};

WaveletPacketBestBasisTransformation::~WaveletPacketBestBasisTransformation() {
	ready			= false;
	DBG(T("WaveletPacketBestBasisTransformation destructed"));
}

void WaveletPacketBestBasisTransformation::calculate() {
	fillDWTInput();

	//to hold the result of the wavelet packet transformation (=DWPT coeffs)
	ArrayTreePer out_DWPT(mDWT_maxLevel);

	//DWPT (discrete wavelet packet transform), periodic
	Analysis(*mDWT_Input, out_DWPT, mDWT_filter_H, mDWT_filter_G, ConvDecPer);
	sortDWPTTreeByScaleDescending(out_DWPT);

	//calculate noise level for a chosen SNR //TODO should be provided in a better way e.g. by measurement...
	int SNR = 48;
	double noise_level		= sqrt(1.0/(SNR*mResolution));
	double oracCostFactor	= (1.0+sqrt((double)2*log((double)mDWT_maxLevel*mResolution)));//D&J Best Wavelet (BWB)

	//Find the best basis 
	HedgePer bestBasis; 
	ExtractBestBasis(out_DWPT, bestBasis, noise_level, oracCostFactor);

	if (bestBasis.num_of_levels <= 1) {
		DBG(T("WaveletPacketBestBasisTransformation::calculate best basis could not be found!"));	
		bestBasis = *mConstantLevelsHedge;
	}
	extractSpectrum(out_DWPT, bestBasis);
}