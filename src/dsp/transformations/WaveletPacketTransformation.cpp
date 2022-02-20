#include "WaveletPacketTransformation.h"
#include "../../libs/juce/JuceLibraryCode/JuceHeader.h"

WaveletPacketTransformation::WaveletPacketTransformation(
	double samplingRate, 
	long resolution, 
	int windowFunctionNr,
	int waveletBaseTypeNr, 
	int resolutionRatioDWPT) 
	: AbstractWaveletTransformation(samplingRate, resolution, windowFunctionNr, waveletBaseTypeNr) 
{
	mSpectralDataInfo	= NULL;
	setResolutionRatioDWPT(resolutionRatioDWPT);
	updateConstantLevelsHedge(mDWPT_ResultTreeLevel);

	DBG(T("Wavelet initialize: N=")										+ 
								juce::String(mResolution)					+
		T(",fres=")			+ juce::String(mFrequencyResolution)	+
		T(",tres=")			+ juce::String(mTimeResolution)			+
		T(",fs=")			+ juce::String(mSamplingRate)				+
		T(",maxLevel=")	+ juce::String(mDWT_maxLevel)				+
		T(",resultTree=")	+ juce::String(mDWPT_ResultTreeLevel)		
	);

	ready			= true;
	calculated	= true;
};

WaveletPacketTransformation::~WaveletPacketTransformation() {
	ready	= false;
	DBG(T("WaveletPacketTransformation destructed"));
}

int WaveletPacketTransformation::getFrequencyResolution (int waveletPacketResultTreeLevel) {
	return (int)1<<(waveletPacketResultTreeLevel);
}

int WaveletPacketTransformation::getTimeResolution() {
	return 1<<(mDWT_maxLevel - mDWPT_ResultTreeLevel);
}

int WaveletPacketTransformation::getWaveletPacketResultTreeLevel (int maxLevel, int resolutionRatioDWPT) {
	//the best possible time & freq resolution ratio can be read out of the (DWPT tree-)level,
	//that results in a square of blocks and block-elements (and therefor an equal freq & time resolution)
	//This is only possible, if the dimension is even (see tab below).
	//To get the possibility to change this toward a better frequency- or time-resolution,
	//the parameter resolutionRatioDWPT can be set (+1 -> freq.Res. x2 , -2 -> time.Res. x4).
	//
	//---------------------------------------------------------------------------
	//N		DIM		Level
	//256	(8)		4	(2*128, 4*64, 8*32, 16*16)
	//512	(9)		5?	(2*256, 4*128, 8*64, 16*32, 32*16) !
	//1024	(10)	5	(2*512, 4*256, 8*128, 16*64, 32*32)
	//2048	(11)	6?	(2*1024, 4*512, 8*256, 16*128, 32*64, 64*32) !
	//4096	(12)	6	(2*2048, 4*1024, 8*512, 16*256, 32*128, 64*64)
	//..
	//Level = ceil_round(Dim / 2) 
	//---------------------------------------------------------------------------
	return (int)ceil((float)maxLevel / 2.0) + resolutionRatioDWPT;
}

void WaveletPacketTransformation::setResolutionRatioDWPT	(int resolutionRatioDWPT) {
	DBG(T("WaveletPacketTransformation::setResolutionRatioDWPT started"));

	mResolutionRatioDWPT		= resolutionRatioDWPT;
	//since value 0 is no more allowed as enum, the new to "equal" corresponding value will be set manually to zero  
	if (mResolutionRatioDWPT == SpectronParameters::RESOLUTION_RATIO_Equal) mResolutionRatioDWPT = 0;

	mDWPT_ResultTreeLevel	= getWaveletPacketResultTreeLevel(mDWT_maxLevel, mResolutionRatioDWPT);
	mFrequencyResolution		= getFrequencyResolution(mDWPT_ResultTreeLevel);
	mTimeResolution			= getTimeResolution();	

	deleteAndZero(mSpectralDataInfo);
	mSpectralDataInfo = new SpectralDataInfo(mSamplingRate, mResolution, mFrequencyResolution, mTimeResolution);
	assert(mSpectralDataInfo);
}

void WaveletPacketTransformation::calculate() {
	fillDWTInput();

	if (!mDWT_Input) {
		DBG(T("WaveletPacketTransformation::calculate: mDWT_Input = null!"));
		return;
	}

	//to hold the result of the wavelet packet transformation (=DWPT coeffs)
	ArrayTreePer out_DWPT(mDWT_maxLevel);
	//DBG(T("WaveletPacketTransformation::calculate before with mDWT_maxLevel= ")	+ 
	//							  juce::String(mDWT_maxLevel)			+
	//	T(",inLen=")		+ juce::String(mDWT_Input->length)	+
	//	T(",outDim=")		+ juce::String(out_DWPT.dim)
	//);

	//DWPT (discrete wavelet packet transform), periodic
	Analysis(*mDWT_Input, out_DWPT, mDWT_filter_H, mDWT_filter_G, ConvDecPer);
	sortDWPTTreeByScaleDescending(out_DWPT);
	extractSpectrum(out_DWPT, *mConstantLevelsHedge);
}