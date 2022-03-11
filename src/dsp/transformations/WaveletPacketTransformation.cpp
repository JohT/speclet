#include "WaveletPacketTransformation.h"
#include "JuceHeader.h"

WaveletPacketTransformation::WaveletPacketTransformation(
        double newSamplingRate,
        ResolutionType newResolution,
        int windowFunctionNr,
        WaveletBase newWaveletBase,
        ResolutionRatioOption newResolutionRatioOption)
    : AbstractWaveletTransformation(newSamplingRate, newResolution, windowFunctionNr, newWaveletBase),
      samplingRate(newSamplingRate),
      timeFrequencyResolutionTreeLevelOffset(toTimeFrequencyResolutionTreeLevelOffset(newResolutionRatioOption)),
      resultTreeLevel(getWaveletPacketResultTreeLevel(mDwtMaxLevel, timeFrequencyResolutionTreeLevelOffset)),
      //TODO(JohT) Fully calculate spectral data info in "calculateSpectralDataInfo" without setResolutionRatioOption
      spectralDataInfo(calculateSpectralDataInfo()) {

    setResolutionRatioOption(newResolutionRatioOption);
    updateConstantLevelsHedge(resultTreeLevel);

    DBG("Wavelet initialize: N=" +
        juce::String(newResolution) +
        ", " + spectralDataInfo.toString() +
        ",sampling rate=" + juce::String(newSamplingRate) +
        ",maxLevel=" + juce::String(mDwtMaxLevel) +
        ",resultTree=" + juce::String(resultTreeLevel));

    setReady();
    setCalculated();
}

WaveletPacketTransformation::~WaveletPacketTransformation() {
    setReady(false);
    DBG("WaveletPacketTransformation destructed");
}

auto WaveletPacketTransformation::getFrequencyResolution(unsigned int waveletPacketResultTreeLevel) -> Transformation::ResolutionType {
    return 1U << waveletPacketResultTreeLevel;
}

auto WaveletPacketTransformation::getTimeResolution() -> Transformation::ResolutionType {
    return 1U << static_cast<unsigned int>(mDwtMaxLevel - resultTreeLevel);
}

auto WaveletPacketTransformation::getWaveletPacketResultTreeLevel(unsigned int maxLevel, int resolutionRatioOffset) -> unsigned int {
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
    assert(maxLevel > 0);
    assert(resolutionRatioOffset != 99);
    auto waveletPacketResultTreeLevel = maxLevel / 2 + resolutionRatioOffset;
    //auto waveletPacketResultTreeLevel   static_cast<unsigned int>(ceil(static_cast<double>(maxLevel) * 0.5F) + resolutionRatioDWPT);
    DBG("AbstractWaveletTransformation::getWaveletPacketResultTreeLevel: maxLevel=" + juce::String(maxLevel) +
        ",resolutionRatio=" + juce::String(resolutionRatioOffset) +
        ",WaveletPacketResultTreeLevel=" + juce::String(waveletPacketResultTreeLevel));
    return waveletPacketResultTreeLevel;
}

void WaveletPacketTransformation::setResolutionRatioOption(ResolutionRatioOption newResolutionRatio) {
    DBG("WaveletPacketTransformation::setResolutionRatioOption to " + juce::String(newResolutionRatio));
    timeFrequencyResolutionTreeLevelOffset = toTimeFrequencyResolutionTreeLevelOffset(newResolutionRatio);
    resultTreeLevel = getWaveletPacketResultTreeLevel(mDwtMaxLevel, timeFrequencyResolutionTreeLevelOffset);
    spectralDataInfo = calculateSpectralDataInfo();
}

auto WaveletPacketTransformation::calculateSpectralDataInfo() -> SpectralDataInfo {
    DBG("WaveletPacketTransformation::calculateSpectralDataInfo: waveletTreeLevel=" + juce::String(resultTreeLevel));
    return {samplingRate, getResolution(), getFrequencyResolution(resultTreeLevel), getTimeResolution()};
}

auto WaveletPacketTransformation::toTimeFrequencyResolutionTreeLevelOffset(const ResolutionRatioOption &resolutionRatioOption) -> int {
    if (resolutionRatioOption == ResolutionRatioOption::EQUAL) {
        return 0;
    }
    return resolutionRatioOption;
}

void WaveletPacketTransformation::calculate() {
    fillDWTInput();

    //to hold the result of the wavelet packet transformation (=DWPT coeffs)
    ArrayTreePer out_DWPT(mDwtMaxLevel);
    //DBG("WaveletPacketTransformation::calculate before with mDWT_maxLevel= "	+
    //							  juce::String(mDWT_maxLevel)			+
    //	",inLen="		+ juce::String(mDWT_Input->length)	+
    //	",outDim="		+ juce::String(out_DWPT.dim)
    //);

    //DWPT (discrete wavelet packet transform), periodic
    Analysis(getDwtInput(), out_DWPT, mDwtFilterH, mDwtFilterG, ConvDecPer);
    sortDWPTTreeByScaleDescending(out_DWPT);
    extractSpectrum(out_DWPT, *mConstantLevelsHedge);
}