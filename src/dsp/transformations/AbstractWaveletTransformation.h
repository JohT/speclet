/*
  ==============================================================================
  This file is part of the VST spectrum analyzer plugin "speclet" (working title)
  Copyright 2011 by Johannes Troppacher
	------------------------------------------------------------------------------
  This file may use parts of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.
	------------------------------------------------------------------------------
  This file may use parts of the fftw library
  Copyright 2003 Matteo Frigo, Copyright 2003 Massachusetts Institute of Technology
	------------------------------------------------------------------------------
  This file may use parts of the wave++ library
  Copyright 1999 by S. E. Ferrando, L. A. Kolasa and N. Kovacevic
  ==============================================================================
*/
#pragma once
#include "../../../lib/wave++/source/libw.h"//TODO (JohT) include with cmake
#include "../../utilities/RenderingHelper.h"
#include "Transformation.h"
#include "WaveletParameters.h"
#include <memory>
#include <span>

class AbstractWaveletTransformation : public Transformation {
public:
    AbstractWaveletTransformation(
            double newSamplingRate,
            ResolutionType newResolution,
            WindowFunctionFactory::Method newWindowFunction = WindowFunctionFactory::Method::DEFAULT,
            WaveletParameters::WaveletBase newWaveletBase =  WaveletParameters::WaveletBase::DEFAULT);
    ~AbstractWaveletTransformation() override;

    AbstractWaveletTransformation(const AbstractWaveletTransformation &) = delete;                    //No copy contructor
    AbstractWaveletTransformation(AbstractWaveletTransformation &&) = delete;                         //No move contructor
    auto operator=(const AbstractWaveletTransformation &) -> AbstractWaveletTransformation & = delete;//No copy assignment
    auto operator=(AbstractWaveletTransformation &&) -> AbstractWaveletTransformation & = delete;     //No move assignment

    void setWaveletBase(const WaveletParameters::WaveletBase &newWaveletBase);
    using WaveletLevelType = unsigned int;

protected:
    void fillDWTInput();

    /**
     * @brief Sorts the tree by descending scale (ascending frequency)
     * 
     * @param tree 
     */
    static void sortWaveletFilterTreeByScaleDescending(const ArrayTreePer &tree);

    /**
     * @brief Executes the wavelet packet transform analysis.
     * 
     * @param analysisResult 
     */
    void analyse(ArrayTreePer &analysisResult);

    /**
     * @brief Executes the dyadic wavelet transform analysis.
     * 
     * @param analysisResult 
     */
    void analyse(Interval &analysisResult);

    void extractSpectrum(const Interval &outDWT);                                               // For "classic" (dyadic) Discrete Wavelet Transform
    void extractSpectrum(const ArrayTreePer &outWaveletPacketTree);                             // For Discrete Wavelet Packet Transform
    void extractSpectrum(const ArrayTreePer &outWaveletPacketTree, const HedgePer &levelsHedge);// For Discrete Wavelet Packet Transform with dynamic (best) basis

    /**
     * @brief Updates the member "constantLevelsHedge" for a given level (e.g. 4,4,4,4).
     * @param level WaveletLevelType
     */
    virtual void updateConstantLevelsHedge(WaveletLevelType level);

    auto getDwtInput() -> const Interval & {
        return dwtInput;
    }
    auto getWaveletFilterTreeMaxLevel() const -> WaveletLevelType {
        return waveletFilterTreeMaxLevel;
    }

private:
    enum TRANSFORM_RESULT_CLASS {
        TRANSFORM_RESULT_CLASS_INTERVAL = 0,
        TRANSFORM_RESULT_CLASS_ARRAYTREE
    };

    WaveletLevelType waveletFilterTreeMaxLevel;//Wavelet dimension (resolution = 2^DWT_MAX_LEVEL)
    Interval dwtInput;                         //wavelet transformation input data of wave++ library

    PQMF mDwtFilterH;//DWT/DWPT lowpass filter coeffs (result=scaling function);
    PQMF mDwtFilterG;//DWT/DWPT hipass  filter coeffs (result=wavelet function);

    std::unique_ptr<HedgePer> constantLevelsHedge;//Contains constant levels as hedge for a given level (e.g. 4,4,4,4)
    std::unique_ptr<HedgePer> dWTLevelsHedge;     //Contains falling levels (=DWT levels) as hedge (e.g. 8,7,6,5,4,3,2,1)

    static auto getMaxLevel(ResolutionType resolution) -> WaveletLevelType;
    static auto getMinLevel(const HedgePer &bestBasis) -> WaveletLevelType;

    /**
     * @brief Updates/fills the member "dWTLevelsHedge" with the levels, that are in use when a dyadic decimated discrete wavelet transform (DWT) is applied.
     */
    void updateDWTLevelsHedge();

    /**
     * @brief Swaps the right and the left child of a wavelet packet transform tree node ("block").

     * This is necessary to get the correct order of frequencies in the wavelet packet transform tree.
     * The "classic" dyadic decimated discrete wavelet transform only takes the result of the previous low pass filter and splits it into two parts.
     * The wavelet packet transform also splits the result of the high pass filter (where specified). 
     * Splitting it into two parts leads to a swapped order of frequencies. The highpass result contains the lower frequency an vice versa.
     * Therefore these need to be swapped.
     * @param tree 
     * @param level 
     * @param block 
     */
    static void swapWaveletFilterTreeChilds(const ArrayTreePer &tree, const WaveletLevelType &level, const unsigned int &block);

    void extractSpectrum(int transformResultClass, std::span<real_number> origin, const HedgePer &levelsHedge);
    auto getValue(int transformResultClass, std::span<real_number> origin, WaveletLevelType level, unsigned long blockNumber, unsigned long blockPosition) const -> double;
    auto getAvgValue(int transformResultClass, std::span<real_number> origin, WaveletLevelType level, unsigned long blockNumber, unsigned long blockposStart, unsigned long blockposEnd) -> double;
};
