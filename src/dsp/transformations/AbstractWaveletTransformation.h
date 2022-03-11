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
#include "../../utilities/PerformanceTimer.h"
#include "../../utilities/RenderingHelper.h"
#include "Transformation.h"

class AbstractWaveletTransformation : public Transformation {
public:
    enum WaveletBase {
        DAUBECHIES_02 = 1,
        DAUBECHIES_04,
        DAUBECHIES_06,
        DAUBECHIES_08,
        DAUBECHIES_10,
        DAUBECHIES_12,
        DAUBECHIES_14,
        DAUBECHIES_16,
        DAUBECHIES_18,
        DAUBECHIES_20,
        COIFMAN_06,
        COIFMAN_12,
        COIFMAN_18,
        COIFMAN_24,
        COIFMAN_30,
        BEYLKIN_18,
        VAIDYANATHAN_18,

        NUMBER_OF_OPTIONS,
        HAAR = DAUBECHIES_02,
        DEFAULT = VAIDYANATHAN_18
    };
    AbstractWaveletTransformation(
            double newSamplingRate,
            ResolutionType newResolution,
            int windowFunctionNr,
            WaveletBase newWaveletBase = WaveletBase::DEFAULT);
    ~AbstractWaveletTransformation() override;

    AbstractWaveletTransformation(const AbstractWaveletTransformation &) = delete;                    //No copy contructor
    AbstractWaveletTransformation(AbstractWaveletTransformation &&) = delete;                         //No move contructor
    auto operator=(const AbstractWaveletTransformation &) -> AbstractWaveletTransformation & = delete;//No copy assignment
    auto operator=(AbstractWaveletTransformation &&) -> AbstractWaveletTransformation & = delete;     //No move assignment

    void setWaveletBase(const WaveletBase &newWaveletBase);

protected:
    virtual auto getMaxLevel(int dimension) -> int;
    virtual auto getMinLevel(const HedgePer &bestBasis) -> int;
    virtual void fillDWTInput();
    virtual void sortDWPTTreeByScaleDescending(const ArrayTreePer &tree);
    virtual void swapDWPTTreeChilds(const ArrayTreePer &tree, const integer_number &L, const integer_number &B);

    virtual void extractSpectrum(const Interval &out_DWT);
    virtual void extractSpectrum(const ArrayTreePer &out_DWPT, const HedgePer &levelsHedge);
    virtual void updateConstantLevelsHedge(unsigned int resolutionRatioDWPT = 0);
    virtual void updateDWTLevelsHedge();

    auto getDwtInput() -> const Interval & {
        return dwtInput;
    }

    int mDwtMaxLevel;   //Wavelet dimension (resolution = 2^DWT_MAX_LEVEL)
    PQMF mDwtFilterH;   //DWT/DWPT lowpass filter coeffs (result=scaling function);
    PQMF mDwtFilterG;   //DWT/DWPT hipass  filter coeffs (result=wavelet function);

    HedgePer *mConstantLevelsHedge;//Contains constant levels as hedge for a given level (e.g. 4,4,4,4)
    HedgePer *mDWTLevelsHedge;     //Contains falling levels (=DWT levels) as hedge (e.g. 8,7,6,5,4,3,2,1)
    RenderingHelper renderingHelper;

private:
    enum TRANSFORM_RESULT_CLASS {
        TRANSFORM_RESULT_CLASS_INTERVAL = 0,
        TRANSFORM_RESULT_CLASS_ARRAYTREE
    };

    WaveletBase wavelet;
    Interval dwtInput;//wavelet transformation input data of wave++ library

    PerformanceTimer extractSpectrumTimer;
    void extractSpectrum(int transformResultClass, real_number *origin, const HedgePer &bestBasis);
    auto getValue(int transformResultClass, real_number *origin, int level, int blocknr, int blockpos) -> float;
    auto getAvgValue(int transformResultClass, real_number *origin, int level, int blocknr, int blockposStart, int blockposEnd) -> float;
};
