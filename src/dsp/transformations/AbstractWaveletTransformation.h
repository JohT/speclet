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
#include "Transformation.h"
#include "..\..\plugin\SpectronParameters.h"
#include "..\..\utilities\RenderingHelper.h"
#include "libw.h"

class AbstractWaveletTransformation : public Transformation {
public:
	AbstractWaveletTransformation(
		double samplingRate, 
		long resolution, 
		int windowFunctionNr		= SpectronParameters::WINDOWING_DEFAULT,
		int waveletBaseTypeNr	= SpectronParameters::WAVELET_DEFAULT
	);
	virtual ~AbstractWaveletTransformation(void);

	void	setWaveletBase(int waveletBasNr);

protected:
	virtual int		getMaxLevel								(int dimension);
	virtual int		getMinLevel								(const HedgePer &bestBasis);
	virtual void	fillDWTInput							(void); 
	virtual void	sortDWPTTreeByScaleDescending		(const ArrayTreePer &tree);
	virtual void	swapDWPTTreeChilds					(const ArrayTreePer &tree, const integer &L, const integer &B);

	virtual void	extractSpectrum(const Interval &out_DWT);
	virtual void	extractSpectrum(const ArrayTreePer &out_DWPT, const HedgePer &levelsHedge);
	virtual void	updateConstantLevelsHedge			(int resolutionRatioDWPT = 0);
	virtual void	updateDWTLevelsHedge					(void);

	int			mWaveletBaseTypeNr;	//Waveletbase-type, see enum WAVELET_BASE_NR
	int			mDWT_maxLevel;			//Wavelet dimension (resolution = 2^DWT_MAX_LEVEL)
	PQMF			mDWT_filter_H;			//DWT/DWPT lowpass filter coeffs (result=scaling function);
	PQMF			mDWT_filter_G;			//DWT/DWPT hipass  filter coeffs (result=wavelet function);
	Interval*	mDWT_Input;				//Pointer to wave++'s wavelet transformation input data

	HedgePer*	mConstantLevelsHedge;//Contains constant levels as hedge for a given level (e.g. 4,4,4,4)
	HedgePer*	mDWTLevelsHedge;		//Contains falling levels (=DWT levels) as hedge (e.g. 8,7,6,5,4,3,2,1)  
	RenderingHelper renderingHelper;

private:
	static enum TRANSFORM_RESULT_CLASS {
		TRANSFORM_RESULT_CLASS_INTERVAL =	0,
		TRANSFORM_RESULT_CLASS_ARRAYTREE
	};
	void		extractSpectrum(int transformResultClass, real_DWT* origin, const HedgePer &bestBasis);
	float		getValue(int transformResultClass, real_DWT* origin, int level, int blocknr, int blockpos);
	float		getAvgValue(int transformResultClass, real_DWT* origin, int level, int blocknr, int blockpos_start, int blockpos_end);
};