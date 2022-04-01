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

#include "../../plugin/SpecletParameters.h"
#include "../windowing/WindowFunctions.h"
#include "Transformation.h"
#include "WaveletParameters.h"

//This special factory-variant does not only create different types of "Transformation"-implementing-objects by ID,
//it is also a singleton and therefore made to exists only once.
//Furthermore it holds a pointer to the transformation, that had been created the last time
class TransformationFactory {
public:
    enum Type {
        FAST_FOURIER_TRANSFORM = 1,
        FAST_WAVELET_TRANSFORM,
        FAST_WAVELET_PACKET_TRANSFORM,
        FAST_WAVELET_PACKET_BEST_BASIS_TRANSFORM,
        BYPASS,

        NUMBER_OF_OPTIONS,
        DEFAULT = FAST_FOURIER_TRANSFORM
    };

    static auto getSingletonInstance() -> TransformationFactory &;

    // Copy-constructors and move- and assignment-operator are deleted, because this class is a singleton.
    TransformationFactory(TransformationFactory const &) = delete;
    TransformationFactory(TransformationFactory &&) = delete;
    auto operator=(TransformationFactory const &) -> TransformationFactory & = delete;
    auto operator=(TransformationFactory const &&) -> TransformationFactory & = delete;

    void destruct();

    auto createTransformation(
            Type newTransformationType,
            double samplingRate,
            Transformation::ResolutionType resolution,
            WindowFunctionFactory::Method windowFunction = WindowFunctionFactory::Method::DEFAULT,
            WaveletParameters::WaveletBase waveletBase = WaveletParameters::WaveletBase::DEFAULT,
            WaveletParameters::ResolutionRatioOption resolutionRatio = WaveletParameters::ResolutionRatioOption::DEFAULT) -> Transformation *;
    void registerForTransformationResults(TransformationListener *value);

private:
    TransformationFactory();
    ~TransformationFactory();

    Transformation *currentTransformation = nullptr;
    TransformationListener *listenerToHandOverToEveryNewTransformation = nullptr;
    Type transformationType = Type::BYPASS;

    void deleteTransformation();
};