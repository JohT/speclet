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
#include "../../../lib/wave++/includes/libw.h"//TODO include with cmake
#include "../../plugin/SpectronParameters.h"
#include "../../utilities/RenderingHelper.h"
#include "Transformation.h"


class AbstractWaveletTransformation : public Transformation {
public:
    AbstractWaveletTransformation(
            double samplingRate,
            long resolution,
            int windowFunctionNr = SpectronParameters::WINDOWING_DEFAULT,
            int waveletBaseTypeNr = SpectronParameters::WAVELET_DEFAULT);
    ~AbstractWaveletTransformation() override;

    void setWaveletBase(int waveletBasNr);

protected:
    virtual auto getMaxLevel(int dimension) -> int;
    virtual auto getMinLevel(const HedgePer &bestBasis) -> int;
    virtual void fillDWTInput();
    virtual void sortDWPTTreeByScaleDescending(const ArrayTreePer &tree);
    virtual void swapDWPTTreeChilds(const ArrayTreePer &tree, const integer &L, const integer &B);

    virtual void extractSpectrum(const Interval &out_DWT);
    virtual void extractSpectrum(const ArrayTreePer &out_DWPT, const HedgePer &levelsHedge);
    virtual void updateConstantLevelsHedge(int resolutionRatioDWPT = 0);
    virtual void updateDWTLevelsHedge(void);

    int mWaveletBaseTypeNr;//Waveletbase-type, see enum WAVELET_BASE_NR
    int mDwtMaxLevel;      //Wavelet dimension (resolution = 2^DWT_MAX_LEVEL)
    PQMF mDwtFilterH;      //DWT/DWPT lowpass filter coeffs (result=scaling function);
    PQMF mDwtFilterG;      //DWT/DWPT hipass  filter coeffs (result=wavelet function);
    Interval *mDwtInput;   //Pointer to wave++'s wavelet transformation input data

    HedgePer *mConstantLevelsHedge;//Contains constant levels as hedge for a given level (e.g. 4,4,4,4)
    HedgePer *mDWTLevelsHedge;     //Contains falling levels (=DWT levels) as hedge (e.g. 8,7,6,5,4,3,2,1)
    RenderingHelper renderingHelper;

private:
    enum TRANSFORM_RESULT_CLASS {
        TRANSFORM_RESULT_CLASS_INTERVAL = 0,
        TRANSFORM_RESULT_CLASS_ARRAYTREE
    };
    void extractSpectrum(int transformResultClass, real_DWT *origin, const HedgePer &bestBasis);
    auto getValue(int transformResultClass, real_DWT *origin, int level, int blocknr, int blockpos) -> float;
    auto getAvgValue(int transformResultClass, real_DWT *origin, int level, int blocknr, int blockposStart, int blockposEnd) -> float;
};
