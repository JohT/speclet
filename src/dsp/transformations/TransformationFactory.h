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

#include "../../dsp/WindowFunctions.h"
#include "../../plugin/SpectronParameters.h"
#include "FourierTransformation.h"
#include "Transformation.h"
#include "WaveletPacketBestBasisTransformation.h"
#include "WaveletPacketTransformation.h"
#include "WaveletTransformation.h"


//This special factory-variant does not only create different types of "Transformation"-implementing-objects by ID,
//it is also a singleton and therefore made to exists only once.
//Furthermore it holds a pointer to the transformation, that had been created the last time
class TransformationFactory {
public:
    static auto getSingletonInstance() -> TransformationFactory &;

    // No copy-and move-constructor and assignment operator are defined, because this class is a singleton.
    TransformationFactory(TransformationFactory const &) = delete;
    TransformationFactory(TransformationFactory &&) = delete;
    auto operator=(TransformationFactory const &) -> TransformationFactory & = delete;
    auto operator=(TransformationFactory const &&) -> TransformationFactory & = delete;

    void destruct();

    Transformation *createTransformation(
            int transformationTypeNr,
            double samplingRate,
            long resolution,
            int windowFunction = SpectronParameters::WINDOWING_DEFAULT,
            int waveletBaseTypeNr = SpectronParameters::WAVELET_DEFAULT,
            int resolutionRatioDWPT = SpectronParameters::RESOLUTION_RATIO_DEFAULT);
    Transformation *getCurrentTransformation(void) { return transformation; };
    void registerForTransformationResults(TransformationListener *value) { listenerToHandOverToEveryNewTransformation = value; };

private:
    TransformationFactory();
    ~TransformationFactory();

    Transformation *transformation = nullptr;
    TransformationListener *listenerToHandOverToEveryNewTransformation = nullptr;
    int transformationType = SpectronParameters::TRANSFORM_OFF;

    void deleteTransformation();
};