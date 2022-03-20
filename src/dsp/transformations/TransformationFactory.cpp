#include "TransformationFactory.h"
#include "AbstractWaveletTransformation.h"
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
    currentTransformation = nullptr;
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
        WindowFunctionFactory::Method windowFunction,
        AbstractWaveletTransformation::WaveletBase waveletBase,
        WaveletPacketTransformation::ResolutionRatioOption resolutionRatio) -> Transformation * {
            
    DBG("TransformationFactory::createTransformation started. transformationNr=" +
        juce::String(newTransformationType) +
        ",pointer to transformation" + (currentTransformation != nullptr ? "exists" : "does not exist"));

    deleteTransformation();

    switch (newTransformationType) {
        case Type::FAST_FOURIER_TRANSFORM: {
            currentTransformation = new FourierTransformation(samplingRate, resolution, windowFunction);
            assert(currentTransformation);
            break;
        }
        case Type::FAST_WAVELET_TRANSFORM: {
            currentTransformation = new WaveletTransformation(samplingRate, resolution, windowFunction, waveletBase);
            assert(currentTransformation);
            break;
        }
        case Type::FAST_WAVELET_PACKET_TRANSFORM: {
            currentTransformation = new WaveletPacketTransformation(samplingRate, resolution, windowFunction, waveletBase, resolutionRatio);
            assert(currentTransformation);
            break;
        }
        case Type::FAST_WAVELET_PACKET_BEST_BASIS_TRANSFORM: {
            currentTransformation = new WaveletPacketBestBasisTransformation(samplingRate, resolution, windowFunction, waveletBase);
            assert(currentTransformation);
            break;
        }
        case Type::BYPASS: {
            break;
        }
        case Type::NUMBER_OF_OPTIONS:
        default: {
            bool transformationUnknownError = false;
            assert(transformationUnknownError);
        }
    }

    if (currentTransformation != nullptr) {
        currentTransformation->setTransformationNr(transformationType);
        currentTransformation->setTransformResultListener(listenerToHandOverToEveryNewTransformation);
    }
    transformationType = newTransformationType;

    DBG("TransformationFactory::createTransformation done. transformationNr=" +
        juce::String(transformationType) +
        ",pointer to transformation" + (currentTransformation != nullptr ? "exists" : "does not exist"));

    return currentTransformation;
}

void TransformationFactory::registerForTransformationResults(TransformationListener *value) {
    listenerToHandOverToEveryNewTransformation = value;
    currentTransformation->setTransformResultListener(listenerToHandOverToEveryNewTransformation);
}

void TransformationFactory::deleteTransformation() {
    delete currentTransformation;
    currentTransformation = nullptr;
    transformationType = Type::BYPASS;
}