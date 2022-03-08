#include "AbstractWaveletTransformation.h"
#include "JuceHeader.h"


AbstractWaveletTransformation::AbstractWaveletTransformation(double newSamplingRate, ResolutionType newResolution, int windowFunctionNr, int waveletBaseTypeNr)
    : Transformation(newSamplingRate, newResolution, windowFunctionNr),
      mWaveletBaseTypeNr(waveletBaseTypeNr), mDwtMaxLevel(getMaxLevel(newResolution)), mConstantLevelsHedge(nullptr), 
      mDWTLevelsHedge(nullptr), extractSpectrumTimer(PerformanceTimer("AbstractWaveletTransformation::extractSpectrum")) {


    mDwtInput = new Interval(0, newResolution - 1);//wavelet transformation input data

    setWaveletBase(mWaveletBaseTypeNr);
    updateConstantLevelsHedge(mDwtMaxLevel / 2);
    updateDWTLevelsHedge();

    DBG("AbstractWaveletTransformation::initialize done with waveletNr=" + juce::String(mWaveletBaseTypeNr) + "maxLevel=" + juce::String(mDwtMaxLevel));
};

AbstractWaveletTransformation::~AbstractWaveletTransformation() {
    delete (mDwtInput);
    delete (mConstantLevelsHedge);
    delete (mDWTLevelsHedge);

    mDwtInput = nullptr;
    mConstantLevelsHedge = nullptr;
    mDWTLevelsHedge = nullptr;

    DBG("AbstractWaveletTransformation destructed");
}

auto AbstractWaveletTransformation::getMaxLevel(int dimension) -> int {
    int maxlevel = static_cast<int>(log(dimension) / log(2));
    //	DBG("AbstractWaveletTransformation::getMaxLevel calc: logDim/log(2)", log((double)dimension), log((double)2.0);
    //	DBG("AbstractWaveletTransformation::getMaxLevel result: dim/maxLevel=", dimension, maxlevel;
    return maxlevel;
}

auto AbstractWaveletTransformation::getMinLevel(const HedgePer &bestBasis) -> int {
    if (bestBasis.num_of_levels <= 0) {
        return 1;
    }
    int minBestBasisLevel = 0;
    int level = 0;

    for (int levelNr = 0; levelNr < bestBasis.num_of_levels; levelNr++) {
        level = bestBasis.levels[levelNr];
        if (level == 1) {
            return 1;
        }
        if ((levelNr == 0) || (level < minBestBasisLevel)) {
            minBestBasisLevel = level;
        }
    }
    return minBestBasisLevel;
}

void AbstractWaveletTransformation::setWaveletBase(int waveletBaseNr) {
    setReady(false);

    switch (waveletBaseNr) {
        case SpectronParameters::WAVELET_DAUBECHIES_02: {
            mDwtFilterG.Set(d02doqf, d02dalpha, d02domega);
            mDwtFilterH.Set(d02soqf, d02salpha, d02somega);
            break;
        }
        case SpectronParameters::WAVELET_DAUBECHIES_04: {
            mDwtFilterG.Set(d04doqf, d04dalpha, d04domega);
            mDwtFilterH.Set(d04soqf, d04salpha, d04somega);
            break;
        }
        case SpectronParameters::WAVELET_DAUBECHIES_06: {
            mDwtFilterG.Set(d06doqf, d06dalpha, d06domega);
            mDwtFilterH.Set(d06soqf, d06salpha, d06somega);
            break;
        }
        case SpectronParameters::WAVELET_DAUBECHIES_08: {
            mDwtFilterG.Set(d08doqf, d08dalpha, d08domega);
            mDwtFilterH.Set(d08soqf, d08salpha, d08somega);
            break;
        }
        case SpectronParameters::WAVELET_DAUBECHIES_10: {
            mDwtFilterG.Set(d10doqf, d10dalpha, d10domega);
            mDwtFilterH.Set(d10soqf, d10salpha, d10somega);
            break;
        }
        case SpectronParameters::WAVELET_DAUBECHIES_12: {
            mDwtFilterG.Set(d12doqf, d12dalpha, d12domega);
            mDwtFilterH.Set(d12soqf, d12salpha, d12somega);
            break;
        }
        case SpectronParameters::WAVELET_DAUBECHIES_14: {
            mDwtFilterG.Set(d14doqf, d14dalpha, d14domega);
            mDwtFilterH.Set(d14soqf, d14salpha, d14somega);
            break;
        }
        case SpectronParameters::WAVELET_DAUBECHIES_16: {
            mDwtFilterG.Set(d16doqf, d16dalpha, d16domega);
            mDwtFilterH.Set(d16soqf, d16salpha, d16somega);
            break;
        }
        case SpectronParameters::WAVELET_DAUBECHIES_18: {
            mDwtFilterG.Set(d18doqf, d18dalpha, d18domega);
            mDwtFilterH.Set(d18soqf, d18salpha, d18somega);
            break;
        }
        case SpectronParameters::WAVELET_DAUBECHIES_20: {
            mDwtFilterG.Set(d20doqf, d20dalpha, d20domega);
            mDwtFilterH.Set(d20soqf, d20salpha, d20somega);
            break;
        }
        case SpectronParameters::WAVELET_COIFMAN_06: {
            mDwtFilterG.Set(c06doqf, c06dalpha, c06domega);
            mDwtFilterH.Set(c06soqf, c06salpha, c06somega);
            break;
        }
        case SpectronParameters::WAVELET_COIFMAN_12: {
            mDwtFilterG.Set(c12doqf, c12dalpha, c12domega);
            mDwtFilterH.Set(c12soqf, c12salpha, c12somega);
            break;
        }
        case SpectronParameters::WAVELET_COIFMAN_18: {
            mDwtFilterG.Set(c18doqf, c18dalpha, c18domega);
            mDwtFilterH.Set(c18soqf, c18salpha, c18somega);
            break;
        }
        case SpectronParameters::WAVELET_COIFMAN_24: {
            mDwtFilterG.Set(c24doqf, c24dalpha, c24domega);
            mDwtFilterH.Set(c24soqf, c24salpha, c24somega);
            break;
        }
        case SpectronParameters::WAVELET_COIFMAN_30: {
            mDwtFilterG.Set(c30doqf, c30dalpha, c30domega);
            mDwtFilterH.Set(c30soqf, c30salpha, c30somega);
            break;
        }
        case SpectronParameters::WAVELET_BEYLKIN_18: {
            mDwtFilterG.Set(b18doqf, b18dalpha, b18domega);
            mDwtFilterH.Set(b18soqf, b18salpha, b18somega);
            break;
        }
        case SpectronParameters::WAVELET_VAIDYANATHAN_18: {
            mDwtFilterG.Set(v24doqf, v24dalpha, v24domega);
            mDwtFilterH.Set(v24soqf, v24salpha, v24somega);
            break;
        }
        default: {
            bool unknownWavelet = false;
            assert(unknownWavelet);
        }
    }

    DBG("AbstractWaveletTransformation::setWaveletBase done with waveletBaseNr=" +
        juce::String(waveletBaseNr) + ",coeffSize=" + juce::String(mDwtFilterG.pcoef_size));

    setReady(true);
}

//--------------------------------------------------------------------//
// Utils for Wavelet Transform                                        //
//--------------------------------------------------------------------//

//Copying every single sample from input-queue to wavelet input-array
void AbstractWaveletTransformation::fillDWTInput() {
    if (mDwtInput == nullptr) {
        DBG("AbstractWaveletTransformation::fillDWTInput: mDWT_Input = null !");
        return;
    }
    auto *windowFunction = getWindowFunction();
    for (unsigned long i = 0; i < getResolution(); i++) {
        double nextSample = mInputQueue->front();
        (*mDwtInput)[i] = nextSample * windowFunction->getFactor(i);
        mInputQueue->pop();
    }
}

// sorts the tree by descending scale (ascending frequency)
void AbstractWaveletTransformation::sortDWPTTreeByScaleDescending(const ArrayTreePer &tree) {
    if (!tree.origin) return;

    for (int level = 1; level <= (tree.maxlevel - 1); level++) {
        int blocksCount = tree.dim / tree.block_length(level);

        for (int block = 1; block < blocksCount; block += 2) {
            //process only the odd elements (=right elemet, =results from G wavelet hipass filter)
            swapDWPTTreeChilds(tree, level, block);
        }
    }
}

// swaps the right and the left child of a wavelet packet transform tree node ("block")
void AbstractWaveletTransformation::swapDWPTTreeChilds(const ArrayTreePer &tree, const integer_number &L, const integer_number &B) {
    assert(0 <= L && L < tree.maxlevel && 0 <= B && B < (1 << L));

    real_number *leftChild = tree.left_child(L, B);
    real_number *rightChild = tree.right_child(L, B);

    real_number helper;
    integer_number blocklength = tree.block_length(L + 1);

    //swap child elements and underlying sublevel-elements
    for (int sublevel = 0; sublevel < tree.maxlevel - L; sublevel++) {
        for (int element = 0; element < blocklength; element++) {
            int index = element + sublevel * tree.dim;
            helper = *(leftChild + index);
            *(leftChild + index) = *(rightChild + index);
            *(rightChild + index) = helper;
        }
    }
}

void AbstractWaveletTransformation::extractSpectrum(const Interval &out_DWT) {
    assert(&out_DWT);
    assert(out_DWT.length > 0);
    extractSpectrum(TRANSFORM_RESULT_CLASS_INTERVAL, out_DWT.origin, *mDWTLevelsHedge);
}

void AbstractWaveletTransformation::extractSpectrum(const ArrayTreePer &out_DWPT, const HedgePer &levelsHedge) {
    assert(&out_DWPT);
    assert(out_DWPT.maxlevel > 0);
    extractSpectrum(TRANSFORM_RESULT_CLASS_ARRAYTREE, out_DWPT.origin, levelsHedge);
}

void AbstractWaveletTransformation::extractSpectrum(int transformResultClass, real_number *origin, const HedgePer &levelsHedge) {
    extractSpectrumTimer.start();

    SpectralDataBuffer::ItemType spectrum;

    int blocksize = 0;
    int blockpos = 0;
    int blockpos_end = 0;
    int blocknr = 0;
    int basisPos = 0;
    int freqDuplicats = 0;
    int minBestBasisLevel = getMinLevel(levelsHedge);
    long timeResolution = 1U << (mDwtMaxLevel - minBestBasisLevel);
    int timeStepSize = timeResolution / TIME_RESOLUTION_LIMIT;
    float value = 0.0;
    auto frequencyResolution = getSpectralDataInfo()->getFrequencyResolution();
    auto realToFullResolution = static_cast<double>(frequencyResolution) / static_cast<double>(getResolution());

    for (int time = 0; time < timeResolution; time++) {
        basisPos = 0;
        spectrum.clear();

        for (int levelNr = 0; levelNr < levelsHedge.num_of_levels; levelNr++) {
            int level = levelsHedge.levels[levelNr];
            blocksize = 1 << (mDwtMaxLevel - level);
            blockpos = (int) ((float) blocksize / (float) timeResolution * (float) time);
            blockpos_end = (int) ((float) blocksize / (float) timeResolution * (float) (time + timeStepSize));
            blocknr = basisPos / blocksize;
            freqDuplicats = (int) (realToFullResolution * static_cast<double>(blocksize));

            //value = getValue(transformResultClass, origin, level, blocknr, blockpos);
            value = getAvgValue(transformResultClass, origin, level, blocknr, blockpos, blockpos_end);

            for (int freqDuplicateNr = 0; freqDuplicateNr < freqDuplicats; freqDuplicateNr++) {
                spectrum.push_back(value * value);
            }

            basisPos += blocksize;
        }
        getSpectralDataBuffer()->write(spectrum);

        //DBG("AbstractWaveletTransformation::extractSpectrum spectrum-size=",(int)spectrum.size();
        //(*1) since the full time resolution causes serious performance problems
        //and is to big for the display,
        //it must be currently limited for high settings.
        if (timeResolution > TIME_RESOLUTION_LIMIT) time += timeStepSize - 1;
    }

    extractSpectrumTimer.stop();
}

//returns the average value of the specified result tree positions
float AbstractWaveletTransformation::getAvgValue(
        int transformResultClass,
        real_number *origin,
        int level,
        int blocknr,
        int blockpos_start,
        int blockpos_end) {
    if (!origin) return 0.0f;

    int stepSize = 2;//skips every nth value. no mathematically exact averaging, but necessary for performance
    int count = 0;
    double averageValue = 0.0;
    real_number *values = NULL;

    if (transformResultClass == TRANSFORM_RESULT_CLASS_ARRAYTREE) {
        auto resolution = getResolution();
        values = origin + (level * resolution + blocknr * ((resolution) >> (level)));
    }
    if (transformResultClass == TRANSFORM_RESULT_CLASS_INTERVAL) {
        values = origin + (1 << (mDwtMaxLevel - level)) + (blocknr - 1);
    }
    if ((blockpos_start + stepSize - 1) >= blockpos_end) return *(values + blockpos_start);

    for (int blockpos = blockpos_start; blockpos < blockpos_end; blockpos += stepSize) {
        averageValue += abs(*(values + blockpos));
        count++;
    }
    return (float) (averageValue / count);
}

//returns the value of the specified result tree position
float AbstractWaveletTransformation::getValue(int transformResultClass, real_number *origin, int level, int blocknr, int blockpos) {
    if (transformResultClass == TRANSFORM_RESULT_CLASS_ARRAYTREE) {
        auto resolution = getResolution();
        return (origin) ? *(origin + (level * resolution + blocknr * ((resolution) >> (level))) + blockpos) : 0.0f;
    }
    if (transformResultClass == TRANSFORM_RESULT_CLASS_INTERVAL) {
        return (origin) ? *(origin + (1 << (mDwtMaxLevel - level)) + (blocknr - 1) + blockpos) : 0.0f;
    }
    DBG("AbstractWaveletTransformation::getValue(..): Unkown transformResultClass");
    return 0.0;
}

//Updates the member "mConstantLevelsHedge" for a given level (e.g. 4,4,4,4)
void AbstractWaveletTransformation::updateConstantLevelsHedge(int level) {
    assert(level > 0);
    assert(level <= mDwtMaxLevel);

    long levelCount = (long) 1 << (level);

    integer_number *levels = new integer_number[levelCount];
    for (int i = 0; i < levelCount; i++) {
        levels[i] = level;
    }

    if (mConstantLevelsHedge) delete (mConstantLevelsHedge);
    mConstantLevelsHedge = new HedgePer(getResolution(), levelCount, levels);
    assert(mConstantLevelsHedge);

    delete[](levels);

    DBG("AbstractWaveletTransformation::updateConstantLevelsHedge done with level=" +
        juce::String(level) + ",count=" + juce::String(levelCount));
}

//Updates/fills the member "mDWTLevelsHedge" with the levels, that are in use when a DWT is applied
void AbstractWaveletTransformation::updateDWTLevelsHedge(void) {
    integer_number *levels = new integer_number[mDwtMaxLevel + 1];

    levels[0] = mDwtMaxLevel;
    for (int i = 0; i < mDwtMaxLevel; i++) {
        levels[i + 1] = mDwtMaxLevel - i;
    }

    if (mDWTLevelsHedge) delete (mDWTLevelsHedge);
    mDWTLevelsHedge = new HedgePer(getResolution(), mDwtMaxLevel + 1, levels);
    assert(mDWTLevelsHedge);

    delete[](levels);
    DBG("AbstractWaveletTransformation::updateDWTLevelsHedge done with count" + juce::String(mDwtMaxLevel));
}