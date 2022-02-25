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

#include "../data/SpectralDataInfo.h"
#include <JuceHeader.h>
#include <list>
#include <vector>


class SpectralDataBuffer {
public:
    static const long CAPACITY = 5000;//50000
    static const int MAXHISTORYELEMENTS = 50;
    static const int SIZECHECKCOUNT = 500;

    using ItemType = std::vector<float>;
    using ItemSizeType = std::vector<float>::size_type;
    using ItemIteratorType = std::vector<float>::iterator;

    //types for per spectrum statistic
    using ItemStatisticsType = struct {
        float min;
        float max;
        float avg;
    };

    SpectralDataBuffer();
    ~SpectralDataBuffer();
    SpectralDataBuffer(const SpectralDataBuffer &) = default;
    auto operator=(const SpectralDataBuffer &) -> SpectralDataBuffer & = default;

    void write(ItemType item);
    void read(ItemType *pItem);

    auto size() -> ItemSizeType;
    auto unread() -> ItemSizeType;

    auto getStatistics(ItemType *pItem) -> ItemStatisticsType;

private:
#if __USE_BOOST
    Bounded_Buffer<ItemType> *buffer;
#else
    std::list<ItemType> *buffer;
    //CriticalSection			criticalSection;
#endif

    ItemSizeType mItemSize;
    bool mWriteAccess;
    int sizeCheckCounter;
};
