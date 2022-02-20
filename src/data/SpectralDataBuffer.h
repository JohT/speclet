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

#include <list>
#include "../../libs/juce/JuceLibraryCode/JuceHeader.h"
#include "../data/SpectralDataInfo.h"
#include <vector>
#include "assert.h"

class SpectralDataBuffer {
public:
	static const long CAPACITY					= 5000;	//50000
	static const int	MAXHISTORYELEMENTS	= 50;
	static const int	SIZECHECKCOUNT			= 500;

	typedef std::vector<float>					ItemType;
	typedef std::vector<float>::size_type	ItemSizeType;
	typedef std::vector<float>::iterator	ItemIteratorType;

	//types for per spectrum statistic
	typedef struct {
		float min;
		float max;
		float avg;
	} ItemStatisticsType;

public:
	SpectralDataBuffer();
	~SpectralDataBuffer();

	void						write(ItemType item);
	void						read(ItemType* pItem);	

	ItemSizeType			size(void);
	ItemSizeType			unread(void);

	ItemStatisticsType	getStatistics(ItemType* pItem);

private:
	SpectralDataBuffer(const SpectralDataBuffer&);					// no copy constructor
	SpectralDataBuffer& operator = (const SpectralDataBuffer&); // no assign operator

	#if __USE_BOOST
		Bounded_Buffer<ItemType>*	buffer;
	#else
		std::list<ItemType>*		buffer;
		//CriticalSection			criticalSection;
	#endif
	
	ItemSizeType					mItemSize;
	bool								mWriteAccess;
	int								sizeCheckCounter;
};


