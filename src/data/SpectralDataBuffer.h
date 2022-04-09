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
#include <list>
#include <vector>

class SpectralDataBuffer {
public:
    static const long CAPACITY = 5000;//50000
    static const int MAXHISTORYELEMENTS = 50;
    static const int SIZECHECKCOUNT = 500;

    using ValueType = float;
    using ItemType = std::vector<ValueType>;
    using ItemSizeType = std::vector<ValueType>::size_type;

    //types for per spectrum statistic
    struct ItemStatisticsType {
        ValueType min = 0.0;
        ValueType max = 0.0;
        ValueType avg = 0.0;

        ItemStatisticsType(ItemType &item);
    };

    SpectralDataBuffer();
    ~SpectralDataBuffer();
    SpectralDataBuffer(const SpectralDataBuffer &) = delete;                    //No copy contructor
    SpectralDataBuffer(SpectralDataBuffer &&) = delete;                         //No move contructor
    auto operator=(const SpectralDataBuffer &) -> SpectralDataBuffer & = delete;//No copy assignment
    auto operator=(SpectralDataBuffer &&) -> SpectralDataBuffer & = delete;     //No move assignment

    void write(const ItemType &item);
    void read(ItemType *pItem);

    auto size() -> ItemSizeType;
    auto unread() -> ItemSizeType;

private:
    std::list<ItemType> *buffer;

    bool mWriteAccess;
    int sizeCheckCounter;
};
