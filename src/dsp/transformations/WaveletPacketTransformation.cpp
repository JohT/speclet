#include "WaveletPacketTransformation.h"
#include "../../utilities/PerformanceLogger.h"
#include "TransformationParameters.h"
#include <cmath>

WaveletPacketTransformation::WaveletPacketTransformation(
        double newSamplingRate,
        ResolutionType newResolution,
        WindowFunctionFactory::Method newWindowFunction,
        WaveletParameters::WaveletBase newWaveletBase,
        WaveletParameters::ResolutionRatioOption newResolutionRatioOption)
    : AbstractWaveletTransformation(newSamplingRate, newResolution, TransformationParameters::Type::FAST_WAVELET_PACKET_TRANSFORM, newWindowFunction, newWaveletBase),
      samplingRate(newSamplingRate),
      timeFrequencyResolutionTreeLevelOffset(toTimeFrequencyResolutionTreeLevelOffset(newResolutionRatioOption)),
      resultTreeLevel(getWaveletPacketResultTreeLevel(getWaveletFilterTreeMaxLevel(), timeFrequencyResolutionTreeLevelOffset)),
      spectralDataInfo(calculateSpectralDataInfo()) {

    setResolutionRatioOption(newResolutionRatioOption);
    updateConstantLevelsHedge(resultTreeLevel);

    DBG("WaveletPacketTransformation constructed: resolution=" +
        juce::String(newResolution) +
        ",sampling frequency=" + juce::String(newSamplingRate) +
        ",maxLevel=" + juce::String(getWaveletFilterTreeMaxLevel()) +
        ",resultTree=" + juce::String(resultTreeLevel));

    setReady();
    setCalculated();
}

WaveletPacketTransformation::~WaveletPacketTransformation() {
    setReady(false);
    DBG("WaveletPacketTransformation destructed");
}

auto WaveletPacketTransformation::getFrequencyResolution(WaveletLevelType waveletPacketResultTreeLevel) -> Transformation::ResolutionType {
    return 1U << waveletPacketResultTreeLevel;
}

auto WaveletPacketTransformation::getTimeResolution() -> Transformation::ResolutionType {
    return 1U << static_cast<unsigned int>(getWaveletFilterTreeMaxLevel() - resultTreeLevel);
}

auto WaveletPacketTransformation::getWaveletPacketResultTreeLevel(WaveletLevelType maxLevel, int resolutionRatioOffset) -> WaveletLevelType {
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

void WaveletPacketTransformation::setResolutionRatioOption(WaveletParameters::ResolutionRatioOption newResolutionRatio) {
    DBG("WaveletPacketTransformation::setResolutionRatioOption to " + std::string(WaveletParameters::resolutionRatioOptionNames.at(newResolutionRatio)));
    timeFrequencyResolutionTreeLevelOffset = toTimeFrequencyResolutionTreeLevelOffset(newResolutionRatio);
    resultTreeLevel = getWaveletPacketResultTreeLevel(getWaveletFilterTreeMaxLevel(), timeFrequencyResolutionTreeLevelOffset);
    spectralDataInfo = calculateSpectralDataInfo();
}

auto WaveletPacketTransformation::calculateSpectralDataInfo() -> SpectralDataInfo {
    DBG("WaveletPacketTransformation::calculateSpectralDataInfo: waveletTreeLevel=" + juce::String(resultTreeLevel));
    return {samplingRate, getResolution(), getFrequencyResolution(resultTreeLevel), getTimeResolution()};
}

auto WaveletPacketTransformation::toTimeFrequencyResolutionTreeLevelOffset(const WaveletParameters::ResolutionRatioOption &resolutionRatioOption) -> int {
    if (resolutionRatioOption == WaveletParameters::ResolutionRatioOption::EQUAL) {
        return 0;
    }
    using ResolutionRatioOptionValueType = std::underlying_type<WaveletParameters::ResolutionRatioOption>::type;
    return static_cast<ResolutionRatioOptionValueType>(resolutionRatioOption);
}

void WaveletPacketTransformation::calculate() {
    LOG_PERFORMANCE_OF_SCOPE("WaveletPacketTransformation calculate");
    fillDWTInput();

    //to hold the result of the wavelet packet transformation (=DWPT coeffs)
    ArrayTreePer outDWPT(getWaveletFilterTreeMaxLevel());
    //DBG("WaveletPacketTransformation::calculate before with mDWT_maxLevel= "	+
    //							  juce::String(mDWT_maxLevel)			+
    //	",inLen="		+ juce::String(mDWT_Input->length)	+
    //	",outDim="		+ juce::String(out_DWPT.dim)
    //);

    //DWPT (discrete wavelet packet transform), periodic
    analyse(outDWPT);
    sortWaveletFilterTreeByScaleDescending(outDWPT);
    extractSpectrum(outDWPT);
}