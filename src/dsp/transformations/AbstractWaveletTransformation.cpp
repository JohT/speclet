#include "AbstractWaveletTransformation.h"
#include "JuceHeader.h"
#include <memory>
#include <span>
#include <type_traits>
#include <vector>

AbstractWaveletTransformation::AbstractWaveletTransformation(double newSamplingRate, ResolutionType newResolution, int windowFunctionNr, WaveletBase newWaveletBase)
    : Transformation(newSamplingRate, newResolution, windowFunctionNr),
      waveletFilterTreeMaxLevel(getMaxLevel(newResolution)),
      dwtInput(Interval(0, static_cast<integer_number>(newResolution - 1))),
      constantLevelsHedge(nullptr),
      dWTLevelsHedge(nullptr),
      extractSpectrumTimer(PerformanceTimer("AbstractWaveletTransformation::extractSpectrum")) {

    setWaveletBase(newWaveletBase);
    updateConstantLevelsHedge(waveletFilterTreeMaxLevel / 2);
    updateDWTLevelsHedge();

    DBG("AbstractWaveletTransformation::initialize done with waveletNr=" + juce::String(newWaveletBase) + "maxLevel=" + juce::String(waveletFilterTreeMaxLevel));
}

AbstractWaveletTransformation::~AbstractWaveletTransformation() {
    constantLevelsHedge = nullptr;
    dWTLevelsHedge = nullptr;

    DBG("AbstractWaveletTransformation destructed");
}

auto AbstractWaveletTransformation::getMaxLevel(ResolutionType resolution) -> WaveletLevelType {
    assert(resolution > 0);
    auto maxlevel = log(resolution) / log(2.0F);
    DBG("AbstractWaveletTransformation::getMaxLevel resolution " + juce::String(resolution) + " leads to max level " + juce::String(maxlevel));
    return static_cast<WaveletLevelType>(lrint(maxlevel));
}

auto AbstractWaveletTransformation::getMinLevel(const HedgePer &bestBasis) -> WaveletLevelType {
    if (bestBasis.num_of_levels <= 0) {
        return 1;
    }
    long minBestBasisLevel = 0;

    auto levels = std::span(bestBasis.levels, static_cast<size_t>(bestBasis.num_of_levels));
    for (auto &level : levels) {
        assert(level >= 0);
        if (level == 1) {
            return 1;
        }
        if ((minBestBasisLevel == 0) || (level < minBestBasisLevel)) {
            minBestBasisLevel = level;
        }
    }
    return static_cast<WaveletLevelType>(minBestBasisLevel);
}

void AbstractWaveletTransformation::setWaveletBase(const WaveletBase &newWaveletBase) {
    setReady(false);

    switch (newWaveletBase) {
        case WaveletBase::DAUBECHIES_02: {
            mDwtFilterG.Set(d02doqf, d02dalpha, d02domega);
            mDwtFilterH.Set(d02soqf, d02salpha, d02somega);
            break;
        }
        case WaveletBase::DAUBECHIES_04: {
            mDwtFilterG.Set(d04doqf, d04dalpha, d04domega);
            mDwtFilterH.Set(d04soqf, d04salpha, d04somega);
            break;
        }
        case WaveletBase::DAUBECHIES_06: {
            mDwtFilterG.Set(d06doqf, d06dalpha, d06domega);
            mDwtFilterH.Set(d06soqf, d06salpha, d06somega);
            break;
        }
        case WaveletBase::DAUBECHIES_08: {
            mDwtFilterG.Set(d08doqf, d08dalpha, d08domega);
            mDwtFilterH.Set(d08soqf, d08salpha, d08somega);
            break;
        }
        case WaveletBase::DAUBECHIES_10: {
            mDwtFilterG.Set(d10doqf, d10dalpha, d10domega);
            mDwtFilterH.Set(d10soqf, d10salpha, d10somega);
            break;
        }
        case WaveletBase::DAUBECHIES_12: {
            mDwtFilterG.Set(d12doqf, d12dalpha, d12domega);
            mDwtFilterH.Set(d12soqf, d12salpha, d12somega);
            break;
        }
        case WaveletBase::DAUBECHIES_14: {
            mDwtFilterG.Set(d14doqf, d14dalpha, d14domega);
            mDwtFilterH.Set(d14soqf, d14salpha, d14somega);
            break;
        }
        case WaveletBase::DAUBECHIES_16: {
            mDwtFilterG.Set(d16doqf, d16dalpha, d16domega);
            mDwtFilterH.Set(d16soqf, d16salpha, d16somega);
            break;
        }
        case WaveletBase::DAUBECHIES_18: {
            mDwtFilterG.Set(d18doqf, d18dalpha, d18domega);
            mDwtFilterH.Set(d18soqf, d18salpha, d18somega);
            break;
        }
        case WaveletBase::DAUBECHIES_20: {
            mDwtFilterG.Set(d20doqf, d20dalpha, d20domega);
            mDwtFilterH.Set(d20soqf, d20salpha, d20somega);
            break;
        }
        case WaveletBase::COIFMAN_06: {
            mDwtFilterG.Set(c06doqf, c06dalpha, c06domega);
            mDwtFilterH.Set(c06soqf, c06salpha, c06somega);
            break;
        }
        case WaveletBase::COIFMAN_12: {
            mDwtFilterG.Set(c12doqf, c12dalpha, c12domega);
            mDwtFilterH.Set(c12soqf, c12salpha, c12somega);
            break;
        }
        case WaveletBase::COIFMAN_18: {
            mDwtFilterG.Set(c18doqf, c18dalpha, c18domega);
            mDwtFilterH.Set(c18soqf, c18salpha, c18somega);
            break;
        }
        case WaveletBase::COIFMAN_24: {
            mDwtFilterG.Set(c24doqf, c24dalpha, c24domega);
            mDwtFilterH.Set(c24soqf, c24salpha, c24somega);
            break;
        }
        case WaveletBase::COIFMAN_30: {
            mDwtFilterG.Set(c30doqf, c30dalpha, c30domega);
            mDwtFilterH.Set(c30soqf, c30salpha, c30somega);
            break;
        }
        case WaveletBase::BEYLKIN_18: {
            mDwtFilterG.Set(b18doqf, b18dalpha, b18domega);
            mDwtFilterH.Set(b18soqf, b18salpha, b18somega);
            break;
        }
        case WaveletBase::VAIDYANATHAN_18: {
            mDwtFilterG.Set(v24doqf, v24dalpha, v24domega);
            mDwtFilterH.Set(v24soqf, v24salpha, v24somega);
            break;
        }
        case WaveletBase::NUMBER_OF_OPTIONS:
        default: {
            bool unknownWavelet = false;
            assert(unknownWavelet);
        }
    }

    DBG("AbstractWaveletTransformation::setWaveletBase done with waveletBaseNr=" +
        juce::String(newWaveletBase) + ",coeffSize=" + juce::String(mDwtFilterG.pcoef_size));

    setReady(true);
}

//--------------------------------------------------------------------//
// Utils for Wavelet Transform                                        //
//--------------------------------------------------------------------//

//Copying every single sample from input-queue to wavelet input-array
void AbstractWaveletTransformation::fillDWTInput() {
    auto *windowFunction = getWindowFunction();
    for (unsigned int i = 0; i < getResolution(); i++) {
        auto nextSample = getInputQueue().front();
        (dwtInput)[i] = nextSample * windowFunction->getFactor(i);
        getInputQueue().pop();
    }
}

void AbstractWaveletTransformation::sortWaveletFilterTreeByScaleDescending(const ArrayTreePer &tree) {
    if (tree.origin == nullptr) {
        return;
    }
    assert(tree.dim > 0);
    for (unsigned int level = 1; level <= (tree.maxlevel - 1); level++) {
        auto blockLength = tree.block_length(level);
        assert(blockLength > 0);
        auto blocksCount = tree.dim / blockLength;

        for (unsigned int block = 1; block < blocksCount; block += 2) {
            //process only the odd elements (=right element, =results from G wavelet hipass filter)
            swapWaveletFilterTreeChilds(tree, level, block);
        }
    }
}

void AbstractWaveletTransformation::swapWaveletFilterTreeChilds(const ArrayTreePer &tree, const WaveletLevelType &level, const unsigned int &block) {
    assert(0 <= level);
    assert(level < tree.maxlevel);
    assert(0 <= block);
    assert(block < (1U << level));

    assert(tree.dim > 0);
    auto treeDimension = static_cast<unsigned long>(tree.dim);

    assert(tree.block_length(level + 1) > 0);
    auto blocklength = static_cast<unsigned long>(tree.block_length(level + 1));

    auto maxSubLevel = static_cast<unsigned long>(tree.maxlevel - level);
    auto maxIndex = static_cast<unsigned long>(blocklength + maxSubLevel * treeDimension);

    auto *leftChild = tree.left_child(level, block);
    auto *rightChild = tree.right_child(level, block);
    auto leftChildSpan = std::span<real_number>(leftChild, maxIndex);
    auto rightChildSpan = std::span<real_number>(rightChild, maxIndex);

    //swap child elements and underlying sublevel-elements
    for (unsigned long sublevel = 0; sublevel < maxSubLevel; sublevel++) {
        for (unsigned long element = 0; element < blocklength; element++) {
            auto index = element + sublevel * treeDimension;
            std::swap(leftChildSpan[index], rightChildSpan[index]);
        }
    }
}

void AbstractWaveletTransformation::analyse(ArrayTreePer &analysisResult) {
    Analysis(getDwtInput(), analysisResult, mDwtFilterH, mDwtFilterG, ConvDecPer);
}

void AbstractWaveletTransformation::analyse(Interval &analysisResult) {
    WaveTrans(getDwtInput(), analysisResult, mDwtFilterH, mDwtFilterG, ConvDecPer);
}

void AbstractWaveletTransformation::extractSpectrum(const Interval &outDWT) {
    assert(outDWT.length > 0);
    assert(outDWT.origin != nullptr);
    auto waveletTransformOutput = std::span<real_number>(outDWT.origin, static_cast<unsigned long>(outDWT.length));
    extractSpectrum(TRANSFORM_RESULT_CLASS_INTERVAL, waveletTransformOutput, *dWTLevelsHedge);
}

void AbstractWaveletTransformation::extractSpectrum(const ArrayTreePer &outWaveletPacketTree) {
    extractSpectrum(outWaveletPacketTree, *constantLevelsHedge);
}

void AbstractWaveletTransformation::extractSpectrum(const ArrayTreePer &outWaveletPacketTree, const HedgePer &levelsHedge) {
    assert(outWaveletPacketTree.maxlevel > 0);
    assert(outWaveletPacketTree.dim > 0);
    assert(outWaveletPacketTree.origin != nullptr);
    auto dataLength = outWaveletPacketTree.dim * (outWaveletPacketTree.maxlevel + 1);
    auto waveletTransformOutput = std::span<real_number>(outWaveletPacketTree.origin, static_cast<unsigned long>(dataLength));
    extractSpectrum(TRANSFORM_RESULT_CLASS_ARRAYTREE, waveletTransformOutput, levelsHedge);
}

void AbstractWaveletTransformation::extractSpectrum(int transformResultClass, std::span<real_number> origin, const HedgePer &levelsHedge) {
    extractSpectrumTimer.start();

    SpectralDataBuffer::ItemType spectrum;

    unsigned int blockSize = 0;
    long blockPosition = 0;
    long blockPositionEnd = 0;
    long blockNumber = 0;
    long basisPosition = 0;
    long freqDuplicates = 0;
    
    WaveletLevelType minBestBasisLevel = getMinLevel(levelsHedge);
    auto timeResolution = 1U << (waveletFilterTreeMaxLevel - minBestBasisLevel);
    auto timeStepSize = timeResolution / TIME_RESOLUTION_LIMIT;
    auto frequencyResolution = getSpectralDataInfo().getFrequencyResolution();
    auto realToFullResolution = static_cast<double>(frequencyResolution) / static_cast<double>(getResolution());
    auto levels = std::span(levelsHedge.levels, static_cast<unsigned long>(levelsHedge.num_of_levels));

    double value = 0.0;
    for (unsigned int time = 0; time < timeResolution; time++) {
        basisPosition = 0;
        spectrum.clear();

        for (auto level : levels) {
            assert(waveletFilterTreeMaxLevel >= level);
            assert(level > 0);
            blockSize = 1U << static_cast<unsigned int>(waveletFilterTreeMaxLevel - level);
            blockPosition = lrint(static_cast<double>(blockSize) / static_cast<double>(timeResolution) * static_cast<double>(time));
            blockPositionEnd = lrint(static_cast<double>(blockSize) / static_cast<double>(timeResolution) * static_cast<double>(time + timeStepSize));
            blockNumber = lrint(static_cast<double>(basisPosition) / static_cast<double>(blockSize));

            value = getAvgValue(transformResultClass, origin, static_cast<WaveletLevelType>(level), blockNumber, blockPosition, blockPositionEnd);

            freqDuplicates = lrint(realToFullResolution * static_cast<double>(blockSize));
            for (int freqDuplicateNr = 0; freqDuplicateNr < freqDuplicates; freqDuplicateNr++) {
                spectrum.push_back(static_cast<SpectralDataBuffer::ValueType>(value * value));
            }

            basisPosition += blockSize;
        }
        getSpectralDataBuffer()->write(spectrum);

        //DBG("AbstractWaveletTransformation::extractSpectrum spectrum-size=",(int)spectrum.size();
        //(*1) since the full time resolution causes serious performance problems
        //and is too big for the display,
        //it must be currently limited for high settings.
        if (timeResolution > TIME_RESOLUTION_LIMIT) {
            time += timeStepSize - 1;
        }
    }

    extractSpectrumTimer.stop();
}

//returns the average value of the specified result tree positions
auto AbstractWaveletTransformation::getAvgValue(
        int transformResultClass,
        std::span<real_number> origin,
        WaveletLevelType level,
        unsigned long blockNumber,
        unsigned long blockposStart,
        unsigned long blockposEnd) -> double {

    //TODO (JohT) Stepsize hack still needed?
    unsigned long stepSize = 2;//skips every nth value. no mathematically exact averaging, but necessary for performance
    auto count = 0;
    auto averageValue = 0.0;
    std::span<real_number> values;

    if (transformResultClass == TRANSFORM_RESULT_CLASS_ARRAYTREE) {
        auto resolution = getResolution();
        auto offset = (level * resolution + blockNumber * ((resolution) >> (level)));
        values = origin.subspan(offset);
    }
    if (transformResultClass == TRANSFORM_RESULT_CLASS_INTERVAL) {
        auto offset = (1U << (waveletFilterTreeMaxLevel - level)) + (blockNumber - 1);
        values = origin.subspan(offset);
    }
    if ((blockposStart + stepSize - 1) >= blockposEnd) {
        return values[blockposStart];
    }

    for (auto blockpos = blockposStart; blockpos < blockposEnd; blockpos += stepSize) {
        averageValue += abs(values[blockpos]);
        count++;
    }
    return averageValue / count;
}

//returns the value of the specified result tree position
auto AbstractWaveletTransformation::getValue(int transformResultClass, std::span<real_number> origin, WaveletLevelType level, unsigned long blockNumber, unsigned long blockPosition) const -> double {
    if (transformResultClass == TRANSFORM_RESULT_CLASS_ARRAYTREE) {
        auto resolution = getResolution();
        auto offset = (level * resolution + blockNumber * ((resolution) >> (level))) + blockPosition;
        return origin[offset];
    }
    if (transformResultClass == TRANSFORM_RESULT_CLASS_INTERVAL) {
        auto offset = (1U << (waveletFilterTreeMaxLevel - level)) + (blockNumber - 1) + blockPosition;
        return origin[offset];
    }
    DBG("AbstractWaveletTransformation::getValue(..): Unkown transformResultClass");
    return 0.0;
}

void AbstractWaveletTransformation::updateConstantLevelsHedge(WaveletLevelType level) {
    assert(level > 0);
    assert(level <= waveletFilterTreeMaxLevel);

    auto levelCount = 1U << level;

    auto levels = std::vector<integer_number>(levelCount);
    for (unsigned int i = 0; i < levelCount; i++) {
        levels[i] = level;
    }

    constantLevelsHedge = std::make_unique<HedgePer>(getResolution(), levelCount, levels.data());

    DBG("AbstractWaveletTransformation::updateConstantLevelsHedge done with level=" +
        juce::String(level) + ",count=" + juce::String(levelCount));
}

void AbstractWaveletTransformation::updateDWTLevelsHedge() {
    assert(waveletFilterTreeMaxLevel > 0);

    auto levels = std::vector<integer_number>(waveletFilterTreeMaxLevel + 1);
    levels[0] = waveletFilterTreeMaxLevel;

    for (unsigned int i = 0; i < static_cast<unsigned int>(waveletFilterTreeMaxLevel); i++) {
        levels[i + 1] = waveletFilterTreeMaxLevel - i;
    }

    dWTLevelsHedge = std::make_unique<HedgePer>(getResolution(), waveletFilterTreeMaxLevel + 1, levels.data());

    DBG("AbstractWaveletTransformation::updateDWTLevelsHedge done with mDwtMaxLevel " + juce::String(waveletFilterTreeMaxLevel));
}