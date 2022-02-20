#include "AbstractWaveletTransformation.h"
#include "../../libs/juce/JuceLibraryCode/JuceHeader.h"
#include "../../utilities/PerformanceManager.h"

AbstractWaveletTransformation::AbstractWaveletTransformation(double samplingRate, long resolution, int windowFunctionNr,	int waveletBaseTypeNr) 
	: Transformation(samplingRate, resolution, windowFunctionNr) 
{
	mDWT_Input					= NULL;
	mConstantLevelsHedge		= NULL;
	mDWTLevelsHedge			= NULL;

	mWaveletBaseTypeNr	= waveletBaseTypeNr;	
	mDWT_maxLevel			= getMaxLevel(resolution);
	mDWT_Input				= new Interval(0, resolution-1);		//wavelet transformation input data 

	setWaveletBase(mWaveletBaseTypeNr);
	updateConstantLevelsHedge(mDWT_maxLevel / 2);
	updateDWTLevelsHedge();

	DBG(T("AbstractWaveletTransformation::initialize done with waveletNr=") 
		+ juce::String(mWaveletBaseTypeNr) + T("maxLevel=") + juce::String(mDWT_maxLevel));
};

AbstractWaveletTransformation::~AbstractWaveletTransformation() {
	if (mDWT_Input)				delete (mDWT_Input);	
	if (mConstantLevelsHedge)	delete (mConstantLevelsHedge);	
	if (mDWTLevelsHedge)			delete (mDWTLevelsHedge);	

	mDWT_Input					= NULL;
	mConstantLevelsHedge		= NULL;
	mDWTLevelsHedge			= NULL;

	DBG(T("AbstractWaveletTransformation destructed"));
}

int AbstractWaveletTransformation::getMaxLevel(int dimension) {
	int maxlevel = (int)(log((double)dimension) / log((double)2.0));
//	DBG(T("AbstractWaveletTransformation::getMaxLevel calc: logDim/log(2)", log((double)dimension), log((double)2.0));
//	DBG(T("AbstractWaveletTransformation::getMaxLevel result: dim/maxLevel=", dimension, maxlevel);
	return maxlevel;
}

int AbstractWaveletTransformation::getMinLevel(const HedgePer &bestBasis) {
	if ((&bestBasis == NULL) || (bestBasis.num_of_levels <= 0)) return 1;
	//return 1;

	int minBestBasisLevel, level	= 0;

	for(int levelNr = 0; levelNr < bestBasis.num_of_levels; levelNr++) {
		level = bestBasis.levels[levelNr];
		if (level == 1) return 1;
		if ((levelNr == 0) || (level < minBestBasisLevel)) {
			minBestBasisLevel = level;
		}
	}
	return minBestBasisLevel;
}

void AbstractWaveletTransformation::setWaveletBase(int waveletBaseNr) {
	ready = false;

	switch (waveletBaseNr) {
	case SpectronParameters::WAVELET_DAUBECHIES_02: {
		mDWT_filter_G.Set(d02doqf, d02dalpha, d02domega);
		mDWT_filter_H.Set(d02soqf, d02salpha, d02somega);
		break;
										 }
	case SpectronParameters::WAVELET_DAUBECHIES_04: {
		mDWT_filter_G.Set(d04doqf, d04dalpha, d04domega);
		mDWT_filter_H.Set(d04soqf, d04salpha, d04somega);
		break;
										 }
	case SpectronParameters::WAVELET_DAUBECHIES_06: {
		mDWT_filter_G.Set(d06doqf, d06dalpha, d06domega);
		mDWT_filter_H.Set(d06soqf, d06salpha, d06somega);
		break;
										 }
	case SpectronParameters::WAVELET_DAUBECHIES_08: {
		mDWT_filter_G.Set(d08doqf, d08dalpha, d08domega);
		mDWT_filter_H.Set(d08soqf, d08salpha, d08somega);
		break;
										 }
	case SpectronParameters::WAVELET_DAUBECHIES_10: {
		mDWT_filter_G.Set(d10doqf, d10dalpha, d10domega);
		mDWT_filter_H.Set(d10soqf, d10salpha, d10somega);
		break;
										 }
	case SpectronParameters::WAVELET_DAUBECHIES_12: {
		mDWT_filter_G.Set(d12doqf, d12dalpha, d12domega);
		mDWT_filter_H.Set(d12soqf, d12salpha, d12somega);
		break;
										 }
	case SpectronParameters::WAVELET_DAUBECHIES_14: {
		mDWT_filter_G.Set(d14doqf, d14dalpha, d14domega);
		mDWT_filter_H.Set(d14soqf, d14salpha, d14somega);
		break;
										 }
	case SpectronParameters::WAVELET_DAUBECHIES_16: {
		mDWT_filter_G.Set(d16doqf, d16dalpha, d16domega);
		mDWT_filter_H.Set(d16soqf, d16salpha, d16somega);
		break;
										 }
	case SpectronParameters::WAVELET_DAUBECHIES_18: {
		mDWT_filter_G.Set(d18doqf, d18dalpha, d18domega);
		mDWT_filter_H.Set(d18soqf, d18salpha, d18somega);
		break;
										 }
	case SpectronParameters::WAVELET_DAUBECHIES_20: {
		mDWT_filter_G.Set(d20doqf, d20dalpha, d20domega);
		mDWT_filter_H.Set(d20soqf, d20salpha, d20somega);
		break;
										 }
	case SpectronParameters::WAVELET_COIFMAN_06: {
		mDWT_filter_G.Set(c06doqf, c06dalpha, c06domega);
		mDWT_filter_H.Set(c06soqf, c06salpha, c06somega);
		break;
									 }
	case SpectronParameters::WAVELET_COIFMAN_12: {
		mDWT_filter_G.Set(c12doqf, c12dalpha, c12domega);
		mDWT_filter_H.Set(c12soqf, c12salpha, c12somega);
		break;
									 }
	case SpectronParameters::WAVELET_COIFMAN_18: {
		mDWT_filter_G.Set(c18doqf, c18dalpha, c18domega);
		mDWT_filter_H.Set(c18soqf, c18salpha, c18somega);
		break;
									 }
	case SpectronParameters::WAVELET_COIFMAN_24: {
		mDWT_filter_G.Set(c24doqf, c24dalpha, c24domega);
		mDWT_filter_H.Set(c24soqf, c24salpha, c24somega);
		break;
									 }
	case SpectronParameters::WAVELET_COIFMAN_30: {
		mDWT_filter_G.Set(c30doqf, c30dalpha, c30domega);
		mDWT_filter_H.Set(c30soqf, c30salpha, c30somega);
		break;
									 }
	case SpectronParameters::WAVELET_BEYLKIN_18: {
		mDWT_filter_G.Set(b18doqf, b18dalpha, b18domega);
		mDWT_filter_H.Set(b18soqf, b18salpha, b18somega);
		break;
									 }
	case SpectronParameters::WAVELET_VAIDYANATHAN_18: {
		mDWT_filter_G.Set(v24doqf, v24dalpha, v24domega);
		mDWT_filter_H.Set(v24soqf, v24salpha, v24somega);
		break;
											}
	default: {
		bool unbekanntes_wavelet = false;
		assert(unbekanntes_wavelet);
				}
	}

	DBG(T("AbstractWaveletTransformation::setWaveletBase done with waveletBaseNr=") +  
		juce::String(waveletBaseNr) + ",coeffSize=" + juce::String(mDWT_filter_G.pcoef_size));

	ready = true;
}

//--------------------------------------------------------------------//
// Utils for Wavelet Transform                                        //
//--------------------------------------------------------------------//

//Copying every single sample from input-queue to wavelet input-array
void AbstractWaveletTransformation::fillDWTInput() {
	if (!mDWT_Input) {
		DBG(T("AbstractWaveletTransformation::fillDWTInput: mDWT_Input = null !"));
		return;
	}
	for (long i = 0; i < mResolution; i++) {
		double nextSample = mInputQueue->front();
		(*mDWT_Input)[i] = nextSample * mWindowFunction->getFactor(i); 
		mInputQueue->pop();			
	}
}

// sorts the tree by descending scale (ascending frequency)
void AbstractWaveletTransformation::sortDWPTTreeByScaleDescending(const ArrayTreePer &tree) {
	if (!tree.origin)	return;

	for (int level = 1; level <= (tree.maxlevel - 1); level++) {
		int	blocksCount	= tree.dim / tree.block_length(level);

		for (int block = 1; block < blocksCount; block+=2) {
			//process only the odd elements (=right elemet, =results from G wavelet hipass filter)
			swapDWPTTreeChilds(tree, level, block);
		}
	}
}

// swaps the right and the left child of a wavelet packet transform tree node ("block")
void AbstractWaveletTransformation::swapDWPTTreeChilds(const ArrayTreePer &tree, const integer &L, const integer &B) {
	assert( 0<=L && L<tree.maxlevel && 0<=B && B<(1<<L) );

	real_DWT*	leftChild	= tree.left_child(L, B);
	real_DWT*	rightChild	= tree.right_child(L, B);

	real_DWT helper;
	integer blocklength = tree.block_length(L+1);

	//swap child elements and underlying sublevel-elements
	for (int sublevel = 0; sublevel < tree.maxlevel - L; sublevel++) {
		for (int element = 0; element < blocklength; element++) {
			int index = element + sublevel*tree.dim;
			helper				= *(leftChild+index);
			*(leftChild+index)	= *(rightChild+index);
			*(rightChild+index)	= helper;
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

void AbstractWaveletTransformation::extractSpectrum(int transformResultClass, real_DWT* origin, const HedgePer &levelsHedge) {
	PerformanceManager::getSingletonInstance()->start(T("waveletExtract"));

	SpectralDataBuffer::ItemType spectrum;

	int		blocksize					= 0;
	int		blockpos						= 0;
	int		blockpos_end				= 0;
	int		blocknr						= 0;
	int		basisPos						= 0;
	int		freqDuplicats				= 0;
	int		minBestBasisLevel			= getMinLevel(levelsHedge);
	long		timeResolution				= (long)1<<(mDWT_maxLevel - minBestBasisLevel);
	int		timeStepSize				= timeResolution / TIME_RESOLUTION_LIMIT;
	float		value							= 0.0;
	float		realToFullResolution		= (float)mFrequencyResolution / (float)mResolution;

	for(int time = 0; time < timeResolution; time++) {
		basisPos	= 0;
		spectrum.clear();

		for(int levelNr = 0; levelNr < levelsHedge.num_of_levels; levelNr++) {
			int level		= levelsHedge.levels[levelNr];
			blocksize		= 1<<(mDWT_maxLevel - level); 
			blockpos			= (int)((float)blocksize / (float)timeResolution * (float)time);
			blockpos_end	= (int)((float)blocksize / (float)timeResolution  * (float)(time + timeStepSize));
			blocknr			= basisPos / blocksize ;
			freqDuplicats	= (int)(realToFullResolution * (float)blocksize);

			//value = getValue(transformResultClass, origin, level, blocknr, blockpos);
			value = getAvgValue(transformResultClass, origin, level, blocknr, blockpos, blockpos_end);

			for(int freqDuplicateNr = 0; freqDuplicateNr < freqDuplicats; freqDuplicateNr++) {				
				spectrum.push_back(value*value);
			}

			basisPos += blocksize;
		}
		getSpectralDataBuffer()->write(spectrum);
		
		//DBG(T("AbstractWaveletTransformation::extractSpectrum spectrum-size=",(int)spectrum.size());
		//(*1) since the full time resolution causes serious performance problems
		//and is to big for the display,  
		//it must be currently limited for high settings.
		if (timeResolution > TIME_RESOLUTION_LIMIT) time+=timeStepSize - 1;
	}

	PerformanceManager::getSingletonInstance()->stop(T("waveletExtract"));
}

//returns the average value of the specified result tree positions
float AbstractWaveletTransformation::getAvgValue(
	int transformResultClass, 
	real_DWT* origin, 
	int level, 
	int blocknr, 
	int blockpos_start, 
	int blockpos_end)
{
	if (!origin) return 0.0f;

	int stepSize = 2;	//skips every nth value. no mathematically exact averaging, but necessary for performance
	int count = 0;
	double averageValue = 0.0;
	real_DWT* values = NULL;
	
	if (transformResultClass == TRANSFORM_RESULT_CLASS_ARRAYTREE) {
		values = origin + (level * mResolution + blocknr * ( (mResolution) >> (level) ));
	}
	if (transformResultClass == TRANSFORM_RESULT_CLASS_INTERVAL) {
		values = origin + (1<<(mDWT_maxLevel - level)) + (blocknr - 1);
	}
	if ((blockpos_start + stepSize - 1) >= blockpos_end) return *(values + blockpos_start);

	for (int blockpos = blockpos_start; blockpos < blockpos_end; blockpos+=stepSize) {
		averageValue += abs(*(values + blockpos));
		count++;
	}
	return (float)(averageValue / count);
}

//returns the value of the specified result tree position
float AbstractWaveletTransformation::getValue(int transformResultClass, real_DWT* origin, int level, int blocknr, int blockpos) {
	if (transformResultClass == TRANSFORM_RESULT_CLASS_ARRAYTREE) {
		return (origin)? *(origin + (level * mResolution + blocknr * ( (mResolution) >> (level) )) + blockpos) : 0.0f;
	}
	if (transformResultClass == TRANSFORM_RESULT_CLASS_INTERVAL) {
		return (origin)? *(origin + (1<<(mDWT_maxLevel - level)) + (blocknr - 1) + blockpos) : 0.0f;
	}
	DBG(T("AbstractWaveletTransformation::getValue(..): Unkown transformResultClass"));
	return 0.0;
}

//Updates the member "mConstantLevelsHedge" for a given level (e.g. 4,4,4,4)
void AbstractWaveletTransformation::updateConstantLevelsHedge(int level) {
	assert(level > 0);
	assert(level <= mDWT_maxLevel);

	long levelCount = (long)1<<(level);	

	integer *levels = new integer [levelCount];
	for (int i = 0; i < levelCount; i++) {
		levels[i] = level;
	}

	if (mConstantLevelsHedge) delete (mConstantLevelsHedge);
	mConstantLevelsHedge = new HedgePer(mResolution, levelCount, levels);
	assert(mConstantLevelsHedge);

	delete [] (levels);

	DBG(T("AbstractWaveletTransformation::updateConstantLevelsHedge done with level=") +
		juce::String(level) + ",count=" + juce::String(levelCount));
}

//Updates/fills the member "mDWTLevelsHedge" with the levels, that are in use when a DWT is applied
void AbstractWaveletTransformation::updateDWTLevelsHedge(void) {
	integer* levels = new integer [mDWT_maxLevel + 1];

	levels[0] = mDWT_maxLevel;
	for (int i = 0; i < mDWT_maxLevel; i++) {
		levels[i+1] = mDWT_maxLevel - i;
	}

	if (mDWTLevelsHedge) delete (mDWTLevelsHedge);
	mDWTLevelsHedge = new HedgePer(mResolution, mDWT_maxLevel + 1, levels);
	assert(mDWTLevelsHedge);

	delete [] (levels);
	DBG(T("AbstractWaveletTransformation::updateDWTLevelsHedge done with count" + juce::String(mDWT_maxLevel)));
}