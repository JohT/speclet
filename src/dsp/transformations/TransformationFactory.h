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

#include "../../parameter/SpecletParameters.h"
#include "../windowing/WindowParameters.h"
#include "Transformation.h"
#include "TransformationParameters.h"
#include "WaveletParameters.h"

//This special factory-variant does not only create different types of "Transformation"-implementing-objects by ID,
//it is also a singleton and therefore made to exists only once.
//Furthermore it holds a pointer to the transformation, that had been created the last time
class TransformationFactory {
public:
    static auto getSingletonInstance() -> TransformationFactory &;

    // Copy-constructors and move- and assignment-operator are deleted, because this class is a singleton.
    TransformationFactory(TransformationFactory const &) = delete;
    TransformationFactory(TransformationFactory &&) = delete;
    auto operator=(TransformationFactory const &) -> TransformationFactory & = delete;
    auto operator=(TransformationFactory const &&) -> TransformationFactory & = delete;

    void destruct();

    auto createTransformation(
            TransformationParameters::Type newTransformationType,
            double samplingRate,
            Transformation::ResolutionType resolution,
            WindowParameters::WindowFunction newWindowFunction = WindowParameters::WindowFunction::DEFAULT,
            WaveletParameters::WaveletBase waveletBase = WaveletParameters::WaveletBase::DEFAULT,
            WaveletParameters::ResolutionRatioOption resolutionRatio = WaveletParameters::ResolutionRatioOption::DEFAULT) -> Transformation *;
    void registerForTransformationResults(TransformationListener *value);

private:
    TransformationFactory();
    ~TransformationFactory();

    Transformation *currentTransformation = nullptr;
    TransformationListener *listenerToHandOverToEveryNewTransformation = nullptr;
    TransformationParameters::Type transformationType = TransformationParameters::Type::BYPASS;

    void deleteTransformation();
};