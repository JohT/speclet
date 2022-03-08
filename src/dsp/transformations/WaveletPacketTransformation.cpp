#include "WaveletPacketTransformation.h"
#include "JuceHeader.h"

WaveletPacketTransformation::WaveletPacketTransformation(
	double newSamplingRate, 
	ResolutionType newResolution, 
	int windowFunctionNr,
	int waveletBaseTypeNr, 
	int resolutionRatioDWPT) 
	: AbstractWaveletTransformation(newSamplingRate, newResolution, windowFunctionNr, waveletBaseTypeNr), 
    samplingRate(newSamplingRate)
{
	mSpectralDataInfo	= NULL;
	setResolutionRatioDWPT(resolutionRatioDWPT);
	updateConstantLevelsHedge(mDWPT_ResultTreeLevel);

    DBG("Wavelet initialize: N=" +
        juce::String(newResolution) +
        ",fres=" + juce::String(mFrequencyResolution) +
        ",tres=" + juce::String(mTimeResolution) +
        ",fs=" + juce::String(newSamplingRate) +
        ",maxLevel=" + juce::String(mDwtMaxLevel) +
        ",resultTree=" + juce::String(mDWPT_ResultTreeLevel));

    setReady();
    setCalculated();
};

WaveletPacketTransformation::~WaveletPacketTransformation() {
    setReady(false);
    DBG("WaveletPacketTransformation destructed");
}

int WaveletPacketTransformation::getFrequencyResolution (int waveletPacketResultTreeLevel) {
	return (int)1<<(waveletPacketResultTreeLevel);
}

int WaveletPacketTransformation::getTimeResolution() {
    return 1 << (mDwtMaxLevel - mDWPT_ResultTreeLevel);
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
    DBG("WaveletPacketTransformation::setResolutionRatioDWPT started");

    mResolutionRatioDWPT		= resolutionRatioDWPT;
	//since value 0 is no more allowed as enum, the new to "equal" corresponding value will be set manually to zero  
	if (mResolutionRatioDWPT == SpectronParameters::RESOLUTION_RATIO_Equal) mResolutionRatioDWPT = 0;

    mDWPT_ResultTreeLevel = getWaveletPacketResultTreeLevel(mDwtMaxLevel, mResolutionRatioDWPT);
    mFrequencyResolution = getFrequencyResolution(mDWPT_ResultTreeLevel);
    mTimeResolution = getTimeResolution();

    deleteAndZero(mSpectralDataInfo);
	mSpectralDataInfo = new SpectralDataInfo(samplingRate, getResolution(), mFrequencyResolution, mTimeResolution);
	assert(mSpectralDataInfo);
}

void WaveletPacketTransformation::calculate() {
	fillDWTInput();

    if (!mDwtInput) {
        DBG("WaveletPacketTransformation::calculate: mDWT_Input = null!");
        return;
    }

    //to hold the result of the wavelet packet transformation (=DWPT coeffs)
    ArrayTreePer out_DWPT(mDwtMaxLevel);
    //DBG("WaveletPacketTransformation::calculate before with mDWT_maxLevel= "	+
    //							  juce::String(mDWT_maxLevel)			+
    //	",inLen="		+ juce::String(mDWT_Input->length)	+
    //	",outDim="		+ juce::String(out_DWPT.dim)
    //);

    //DWPT (discrete wavelet packet transform), periodic
    Analysis(*mDwtInput, out_DWPT, mDwtFilterH, mDwtFilterG, ConvDecPer);
    sortDWPTTreeByScaleDescending(out_DWPT);
    extractSpectrum(out_DWPT, *mConstantLevelsHedge);
}