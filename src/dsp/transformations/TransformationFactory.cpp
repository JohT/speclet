#include "TransformationFactory.h"
#include "AbstractWaveletTransformation.h"
#include "FourierTransformation.h"
#include "Transformation.h"
#include "TransformationParameters.h"
#include "WaveletPacketBestBasisTransformation.h"
#include "WaveletPacketTransformation.h"
#include "WaveletTransformation.h"
#include <cstddef>
#include <string>

#ifndef __LOGGER__
#include "juce_core/juce_core.h"
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
        TransformationParameters::Type newTransformationType,
        double samplingRate,
        Transformation::ResolutionType resolution,
        WindowParameters::WindowFunction newWindowFunction,
        WaveletParameters::WaveletBase waveletBase,
        WaveletParameters::ResolutionRatioOption resolutionRatio) -> Transformation * {

    auto newTransformationTypeName = std::string(TransformationParameters::TypeNames::map.find(newTransformationType)->second);
    DBG("TransformationFactory::createTransformation started. transformationType=" +  newTransformationTypeName +
        ", old transformation=" + (currentTransformation != nullptr ? currentTransformation->getName() : "does not exist"));

    deleteTransformation();

    switch (newTransformationType) {
        case TransformationParameters::Type::FAST_FOURIER_TRANSFORM: {
            currentTransformation = new FourierTransformation(samplingRate, resolution, newWindowFunction);
            assert(currentTransformation);
            break;
        }
        case TransformationParameters::Type::FAST_WAVELET_TRANSFORM: {
            currentTransformation = new WaveletTransformation(samplingRate, resolution, newWindowFunction, waveletBase);
            assert(currentTransformation);
            break;
        }
        case TransformationParameters::Type::FAST_WAVELET_PACKET_TRANSFORM: {
            currentTransformation = new WaveletPacketTransformation(samplingRate, resolution, newWindowFunction, waveletBase, resolutionRatio);
            assert(currentTransformation);
            break;
        }
        case TransformationParameters::Type::FAST_WAVELET_PACKET_BEST_BASIS_TRANSFORM: {
            currentTransformation = new WaveletPacketBestBasisTransformation(samplingRate, resolution, newWindowFunction, waveletBase);
            assert(currentTransformation);
            break;
        }
        case TransformationParameters::Type::BYPASS: {
            break;
        }
        case TransformationParameters::Type::NUMBER_OF_OPTIONS:
        default: {
            bool transformationUnknownError = false;
            assert(transformationUnknownError);
        }
    }

    if (currentTransformation != nullptr) {
        currentTransformation->setTransformResultListener(listenerToHandOverToEveryNewTransformation);
    }
    transformationType = newTransformationType;

    DBG("TransformationFactory::createTransformation done. transformationType=" + newTransformationTypeName +
        ", current transformation="  + (currentTransformation != nullptr ? currentTransformation->getName() : "does not exist"));

    return currentTransformation;
}

void TransformationFactory::registerForTransformationResults(TransformationListener *value) {
    listenerToHandOverToEveryNewTransformation = value;
    if (currentTransformation != nullptr) {
        currentTransformation->setTransformResultListener(listenerToHandOverToEveryNewTransformation);
    }
}

void TransformationFactory::deleteTransformation() {
    delete currentTransformation;
    currentTransformation = nullptr;
    transformationType = TransformationParameters::Type::BYPASS;
}