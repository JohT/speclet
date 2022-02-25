#include "TransformationFactory.h"
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
        int transformationTypeNr,
        double samplingRate,
        long resolution,
        int windowFunction,
        int waveletBaseTypeNr,
        int resolutionRatioDWPT) -> Transformation * {
    DBG("TransformationFactory::createTransformation started. transformationNr=" +
        juce::String(transformationTypeNr) +
        ",pointer to transformation" + (transformation != nullptr ? "exists" : "does not exist"));

    deleteTransformation();

    switch (transformationTypeNr) {
        case SpectronParameters::TRANSFORM_FFT: {
            transformation = new FourierTransformation(samplingRate, resolution, windowFunction);
            jassert(transformation);
            break;
        }
        case SpectronParameters::TRANSFORM_FWT: {
            transformation = new WaveletTransformation(samplingRate, resolution, windowFunction, waveletBaseTypeNr);
            jassert(transformation);
            break;
        }
        case SpectronParameters::TRANSFORM_FWPT: {
            transformation = new WaveletPacketTransformation(samplingRate, resolution, windowFunction, waveletBaseTypeNr, resolutionRatioDWPT);
            jassert(transformation);
            break;
        }
        case SpectronParameters::TRANSFORM_FWPT_BB: {
            transformation = new WaveletPacketBestBasisTransformation(samplingRate, resolution, windowFunction, waveletBaseTypeNr);
            jassert(transformation);
            break;
        }
        case SpectronParameters::TRANSFORM_OFF: {
            break;
        }
        default: {
            bool transformationUnknownError = false;
            jassert(transformationUnknownError);
        }
    }

    if (transformation != nullptr) {
        transformation->setTransformationNr(transformationTypeNr);
        transformation->setTransformResultListener(listenerToHandOverToEveryNewTransformation);
    }
    transformationType = transformationTypeNr;

    DBG("TransformationFactory::createTransformation done. transformationNr=" +
        juce::String(transformationTypeNr) +
        ",pointer to transformation" + (transformation != nullptr ? "exists" : "does not exist"));

    return transformation;
}

void TransformationFactory::deleteTransformation() {
    delete transformation;
    transformation = nullptr;
    transformationType = SpectronParameters::TRANSFORM_OFF;
}