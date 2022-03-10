#include "TransformationFactory.h"
#include "Transformation.h"
#include "WaveletPacketTransformation.h"
#include <cstddef>

#ifndef __LOGGER__
#include "JuceHeader.h"
#define LOGGER CLogger::getSingletonInstance()
#endif

auto TransformationFactory::getSingletonInstance() -> TransformationFactory & {
    static TransformationFactory singletonInstance;// Guaranteed to be destroyed. Instantiated on first use.
    return singletonInstance;
}

TransformationFactory::TransformationFactory() {
    DBG("TransformationFactory constructed");
}

TransformationFactory::~TransformationFactory() {
    deleteTransformation();
    transformation = nullptr;
    listenerToHandOverToEveryNewTransformation = nullptr;
    DBG("TransformationFactory deconstructed");
}

void TransformationFactory::destruct() {
    listenerToHandOverToEveryNewTransformation = nullptr;
}

auto TransformationFactory::createTransformation(
        Type newTransformationType,
        double samplingRate,
        Transformation::ResolutionType resolution,
        int windowFunction,
        int waveletBaseTypeNr,
        WaveletPacketTransformation::ResolutionRatioOption resolutionRatio) -> Transformation * {
    DBG("TransformationFactory::createTransformation started. transformationNr=" +
        juce::String(newTransformationType) +
        ",pointer to transformation" + (transformation != nullptr ? "exists" : "does not exist"));

    deleteTransformation();

    switch (newTransformationType) {
        case Type::FAST_FOURIER_TRANSFORM: {
            transformation = new FourierTransformation(samplingRate, resolution, windowFunction);
            jassert(transformation);
            break;
        }
        case Type::FAST_WAVELET_TRANSFORM: {
            transformation = new WaveletTransformation(samplingRate, resolution, windowFunction, waveletBaseTypeNr);
            jassert(transformation);
            break;
        }
        case Type::FAST_WAVELET_PACKET_TRANSFORM: {
            transformation = new WaveletPacketTransformation(samplingRate, resolution, windowFunction, waveletBaseTypeNr, resolutionRatio);
            jassert(transformation);
            break;
        }
        case Type::FAST_WAVELET_PACKET_BEST_BASIS_TRANSFORM: {
            transformation = new WaveletPacketBestBasisTransformation(samplingRate, resolution, windowFunction, waveletBaseTypeNr);
            jassert(transformation);
            break;
        }
        case Type::BYPASS: {
            break;
        }
        case Type::NUMBER_OF_OPTIONS:
        default: {
            bool transformationUnknownError = false;
            jassert(transformationUnknownError);
        }
    }

    if (transformation != nullptr) {
        transformation->setTransformationNr(transformationType);
        transformation->setTransformResultListener(listenerToHandOverToEveryNewTransformation);
    }
    transformationType = newTransformationType;

    DBG("TransformationFactory::createTransformation done. transformationNr=" +
        juce::String(transformationType) +
        ",pointer to transformation" + (transformation != nullptr ? "exists" : "does not exist"));

    return transformation;
}

void TransformationFactory::deleteTransformation() {
    delete transformation;
    transformation = nullptr;
    transformationType = Type::BYPASS;
}