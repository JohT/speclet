#include "TransformationFactory.h"

#ifndef __LOGGER__
	#include "../../libs/juce/JuceLibraryCode/JuceHeader.h"
	#define LOGGER CLogger::getSingletonInstance()
#endif

// Singleton instance variable (only one instance of this class) 
TransformationFactory* TransformationFactory::singletonInstance = 0;

TransformationFactory::TransformationFactory() {
	transformation											= NULL;
	listenerToHandOverToEveryNewTransformation	= NULL;
	transformationType									= SpectronParameters::TRANSFORM_OFF;
	DBG(T("TransformationFactory constructed"));
}

TransformationFactory::~TransformationFactory(void) {
	deleteTransformation();
	transformation											= NULL;
	listenerToHandOverToEveryNewTransformation	= NULL;
	DBG(T("TransformationFactory deconstructed"));
}

void TransformationFactory::destruct() {
	if (!singletonInstance) return;

	listenerToHandOverToEveryNewTransformation = 0;
	singletonInstance->deleteTransformation();
	
	delete(singletonInstance);
	singletonInstance = 0;
}

TransformationFactory* TransformationFactory::getSingletonInstance() {
// Method to get the single instance of this class (Singleton)
	if (TransformationFactory::singletonInstance == 0) {
		TransformationFactory::singletonInstance = new TransformationFactory();
	}
	return TransformationFactory::singletonInstance;
}

Transformation* TransformationFactory::createTransformation(
	int transformationTypeNr, 
	double samplingRate, 
	long resolution, 
	int windowFunction,
	int waveletBaseTypeNr,
	int resolutionRatioDWPT) 
{
	DBG(T("TransformationFactory::createTransformation started. transformationNr=")	+ 
						  juce::String(transformationTypeNr)	+
		T(",ptr=")	+ juce::String((int)transformation)
	);

	deleteTransformation();
	
	switch (transformationTypeNr) {
	case SpectronParameters::TRANSFORM_FFT	: {
			transformation = new FourierTransformation(samplingRate, resolution, windowFunction);	
			assert(transformation);
			break;
										  }	
		case SpectronParameters::TRANSFORM_FWT	: {
			transformation = new WaveletTransformation(samplingRate, resolution, windowFunction, waveletBaseTypeNr);	
			assert(transformation);
			break;
										  }													  
		case SpectronParameters::TRANSFORM_FWPT : {
			transformation = new WaveletPacketTransformation(samplingRate, resolution, windowFunction, waveletBaseTypeNr, resolutionRatioDWPT);	
			assert(transformation);
			break;
											}
		case SpectronParameters::TRANSFORM_FWPT_BB : {
			transformation = new WaveletPacketBestBasisTransformation(samplingRate, resolution, windowFunction, waveletBaseTypeNr);	
			assert(transformation);
			break;
														}
		case SpectronParameters::TRANSFORM_OFF	: {
			break;
										  }
		default : {
			bool transformation_unknown_error = false; 
			assert(transformation_unknown_error);
					 }
	}

	if (transformation) {
		transformation->setTransformationNr(transformationTypeNr);	
		transformation->setTransformResultListener(listenerToHandOverToEveryNewTransformation);
	}
	transformationType	= transformationTypeNr;

	DBG(T("TransformationFactory::createTransformation done. transformationNr=")	+ 
						  juce::String(transformationTypeNr)	+
		T(",ptr=")	+ juce::String((int)transformation)
	);

	return transformation;
}

void TransformationFactory::deleteTransformation(void) {
	if (transformation) delete transformation;
	transformation	= 0;
	transformationType = SpectronParameters::TRANSFORM_OFF;
}