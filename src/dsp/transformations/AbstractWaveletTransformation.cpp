#include "AbstractWaveletTransformation.h"
#include "JuceHeader.h"
#include <memory>
#include <span>
#include <vector>

AbstractWaveletTransformation::AbstractWaveletTransformation(double newSamplingRate, ResolutionType newResolution, int windowFunctionNr, WaveletBase newWaveletBase)
    : Transformation(newSamplingRate, newResolution, windowFunctionNr),
      mDwtMaxLevel(getMaxLevel(newResolution)),
      mConstantLevelsHedge(nullptr),
      mDWTLevelsHedge(nullptr),
      dwtInput(Interval(0, static_cast<integer_number>(newResolution - 1))),
      extractSpectrumTimer(PerformanceTimer("AbstractWaveletTransformation::extractSpectrum")) {

    setWaveletBase(newWaveletBase);
    updateConstantLevelsHedge(mDwtMaxLevel / 2);
    updateDWTLevelsHedge();

    DBG("AbstractWaveletTransformation::initialize done with waveletNr=" + juce::String(newWaveletBase) + "maxLevel=" + juce::String(mDwtMaxLevel));
}

AbstractWaveletTransformation::~AbstractWaveletTransformation() {
    delete (mConstantLevelsHedge);
    delete (mDWTLevelsHedge);

    mConstantLevelsHedge = nullptr;
    mDWTLevelsHedge = nullptr;

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

// sorts the tree by descending scale (ascending frequency)
void AbstractWaveletTransformation::sortWaveletFilterTreeByScaleDescending(const ArrayTreePer &tree) {
    if (tree.origin == nullptr) {
        return;
    }
    assert(tree.dim > 0);
    for (int level = 1; level <= (tree.maxlevel - 1); level++) {
        auto blockLength = tree.block_length(level);
        assert(blockLength > 0);
        auto blocksCount = tree.dim / blockLength;

        for (auto block = 1; block < blocksCount; block += 2) {
            //process only the odd elements (=right elemet, =results from G wavelet hipass filter)
            swapWaveletFilterTreeChilds(tree, level, block);
        }
    }
}

// swaps the right and the left child of a wavelet packet transform tree node ("block")
void AbstractWaveletTransformation::swapWaveletFilterTreeChilds(const ArrayTreePer &tree, const integer_number &l, const integer_number &b) {
    assert(0 <= l && l < tree.maxlevel && 0 <= b && b < (1 << l));

    auto *leftChild = tree.left_child(l, b);
    auto *rightChild = tree.right_child(l, b);

    real_number helper = 0;
    auto blocklength = tree.block_length(l + 1);

    //swap child elements and underlying sublevel-elements
    for (int sublevel = 0; sublevel < tree.maxlevel - l; sublevel++) {
        for (int element = 0; element < blocklength; element++) {
            auto index = element + sublevel * tree.dim;
            helper = *(leftChild + index);
            *(leftChild + index) = *(rightChild + index);
            *(rightChild + index) = helper;
        }
    }
}

void AbstractWaveletTransformation::extractSpectrum(const Interval &outDWT) {
    assert(outDWT.length > 0);
    extractSpectrum(TRANSFORM_RESULT_CLASS_INTERVAL, outDWT.origin, *mDWTLevelsHedge);
}

void AbstractWaveletTransformation::extractSpectrum(const ArrayTreePer &outWaveletPacketTree) {
    extractSpectrum(outWaveletPacketTree, *mConstantLevelsHedge);
}

void AbstractWaveletTransformation::extractSpectrum(const ArrayTreePer &outWaveletPacketTree, const HedgePer &levelsHedge) {
    assert(outWaveletPacketTree.maxlevel > 0);
    extractSpectrum(TRANSFORM_RESULT_CLASS_ARRAYTREE, outWaveletPacketTree.origin, levelsHedge);
}

void AbstractWaveletTransformation::extractSpectrum(int transformResultClass, real_number *origin, const HedgePer &levelsHedge) {
    extractSpectrumTimer.start();

    SpectralDataBuffer::ItemType spectrum;

    unsigned int blockSize = 0;
    long blockPosition = 0;
    long blockPositionEnd = 0;
    long blockNumber = 0;
    long basisPosition = 0;
    long freqDuplicates = 0;
    WaveletLevelType minBestBasisLevel = getMinLevel(levelsHedge);
    auto timeResolution = 1U << (mDwtMaxLevel - minBestBasisLevel);
    auto timeStepSize = timeResolution / TIME_RESOLUTION_LIMIT;
    float value = 0.0;
    auto frequencyResolution = getSpectralDataInfo().getFrequencyResolution();
    auto realToFullResolution = static_cast<double>(frequencyResolution) / static_cast<double>(getResolution());

    auto levels = std::span(levelsHedge.levels, static_cast<unsigned long>(levelsHedge.num_of_levels));
    
    for (unsigned int time = 0; time < timeResolution; time++) {
        basisPosition = 0;
        spectrum.clear();

        for (auto level : levels) {
            assert(mDwtMaxLevel >= level);
            assert(level > 0);
            blockSize = 1U << static_cast<unsigned int>(mDwtMaxLevel - level);
            blockPosition = lrint(static_cast<double>(blockSize) / static_cast<double>(timeResolution) * static_cast<double>(time));
            blockPositionEnd = lrint(static_cast<double>(blockSize) / static_cast<double>(timeResolution) * static_cast<double>(time + timeStepSize));
            blockNumber = lrint(static_cast<double>(basisPosition) / static_cast<double>(blockSize));
            freqDuplicates = lrint(realToFullResolution * static_cast<double>(blockSize));

            value = getAvgValue(transformResultClass, origin, static_cast<WaveletLevelType>(level), blockNumber, blockPosition, blockPositionEnd);

            for (int freqDuplicateNr = 0; freqDuplicateNr < freqDuplicates; freqDuplicateNr++) {
                spectrum.push_back(value * value);
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
        real_number *origin,
        WaveletLevelType level,
        long blockNumber,
        long blockposStart,
        long blockposEnd) -> float {

    if (origin == nullptr) {
        return 0.0F;
    }

    auto stepSize = 2;//skips every nth value. no mathematically exact averaging, but necessary for performance
    auto count = 0;
    auto averageValue = 0.0;
    real_number *values = nullptr;

    if (transformResultClass == TRANSFORM_RESULT_CLASS_ARRAYTREE) {
        auto resolution = getResolution();
        values = origin + (level * resolution + blockNumber * ((resolution) >> (level)));
    }
    if (transformResultClass == TRANSFORM_RESULT_CLASS_INTERVAL) {
        values = origin + (1U << (mDwtMaxLevel - level)) + (blockNumber - 1);
    }
    if ((blockposStart + stepSize - 1) >= blockposEnd) {
        return *(values + blockposStart);
    }

    for (int blockpos = blockposStart; blockpos < blockposEnd; blockpos += stepSize) {
        averageValue += abs(*(values + blockpos));
        count++;
    }
    return static_cast<float>(averageValue / count);
}

//returns the value of the specified result tree position
auto AbstractWaveletTransformation::getValue(int transformResultClass, const real_number *origin, WaveletLevelType level, int blockNumber, int blockPosition) const -> float {
    if (origin == nullptr) {
        return 0.0F;
    }
    if (transformResultClass == TRANSFORM_RESULT_CLASS_ARRAYTREE) {
        auto resolution = getResolution();
        return *(origin + (level * resolution + blockNumber * ((resolution) >> (level))) + blockPosition);
    }
    if (transformResultClass == TRANSFORM_RESULT_CLASS_INTERVAL) {
        return *(origin + (1 << (mDwtMaxLevel - level)) + (blockNumber - 1) + blockPosition);
    }
    DBG("AbstractWaveletTransformation::getValue(..): Unkown transformResultClass");
    return 0.0;
}

//Updates the member "mConstantLevelsHedge" for a given level (e.g. 4,4,4,4)
void AbstractWaveletTransformation::updateConstantLevelsHedge(WaveletLevelType level) {
    assert(level > 0);
    assert(level <= mDwtMaxLevel);

    auto levelCount = 1U << level;

    auto levels = std::vector<integer_number>(levelCount);
    for (unsigned int i = 0; i < levelCount; i++) {
        levels[i] = level;
    }

    delete (mConstantLevelsHedge);

    mConstantLevelsHedge = new HedgePer(getResolution(), levelCount, levels.data());
    assert(mConstantLevelsHedge);

    DBG("AbstractWaveletTransformation::updateConstantLevelsHedge done with level=" +
        juce::String(level) + ",count=" + juce::String(levelCount));
}

//Updates/fills the member "mDWTLevelsHedge" with the levels, that are in use when a DWT is applied
void AbstractWaveletTransformation::updateDWTLevelsHedge() {
    assert(mDwtMaxLevel > 0);
    
    auto levels = std::vector<integer_number>(mDwtMaxLevel + 1);
    levels[0] = mDwtMaxLevel;
    
    for (unsigned int i = 0; i < static_cast<unsigned int>(mDwtMaxLevel); i++) {
        levels[i + 1] = mDwtMaxLevel - i;
    }

    delete (mDWTLevelsHedge);

    mDWTLevelsHedge = new HedgePer(getResolution(), mDwtMaxLevel + 1, levels.data());

    DBG("AbstractWaveletTransformation::updateDWTLevelsHedge done with mDwtMaxLevel " + juce::String(mDwtMaxLevel));
}