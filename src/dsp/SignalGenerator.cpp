#include "SignalGenerator.h"
#include "ranlib.h"

#include <math.h>
#include <assert.h>
#define PIx2	6.2831853071795865

SignalGenerator::SignalGenerator(short signalType, double signalFrequency, double samplingFrequency) {
	mSignalType						= signalType;
	mSignalFrequency				= signalFrequency;
	mSamplingFrequency				= samplingFrequency;
	mLastSignalGeneratorArgument	= 0.0;
	mLastSignalGeneratorSample		= 0.0;

	if (signalType == SpectronParameters::GENERATOR_NOISE) {
		long iseed1 =333;
		long iseed2 = (long) (.987654321 * (double)iseed1);
		setall(iseed1,(long) iseed2);
	}
}

SignalGenerator::~SignalGenerator(void){
}

double SignalGenerator::getNextSample(void) {
	switch (mSignalType) {
		case SpectronParameters::GENERATOR_SINE: 		return generateSine();
		case SpectronParameters::GENERATOR_TRANGLE: 	return generateTriangle();
		case SpectronParameters::GENERATOR_RAMP: 		return generateRamp();
		case SpectronParameters::GENERATOR_SQUARE: 	return generateSquare();
		case SpectronParameters::GENERATOR_NOISE: 	return generateNoise();
		default: {
		//ignore if wrong: use sine as default
			return generateSine();
		}
	}
}

double	SignalGenerator::generateSine (void) {
	double value = sin(mLastSignalGeneratorArgument);
	mLastSignalGeneratorSample = value;
	
	mLastSignalGeneratorArgument = mLastSignalGeneratorArgument + (PIx2 / mSamplingFrequency * mSignalFrequency);
	if (mLastSignalGeneratorArgument > PIx2) mLastSignalGeneratorArgument-=PIx2;
		
	return value;
}

double	SignalGenerator::generateTriangle (void) {
	if (mLastSignalGeneratorArgument == 0.0) {
	//first call - short cut
		mLastSignalGeneratorArgument = 4.0*mSignalFrequency/mSamplingFrequency; 
		mLastSignalGeneratorSample = 0.0;
		return 0.0;
	}

	double value = mLastSignalGeneratorSample + mLastSignalGeneratorArgument;
	
	if ((mLastSignalGeneratorArgument >= 0.0)
	&&  (value > 1.0)) {
		mLastSignalGeneratorArgument*=-1.0;
		value = mLastSignalGeneratorSample + mLastSignalGeneratorArgument;
	}
	if ((mLastSignalGeneratorArgument < 0.0)
	&&  (value < -1.0)) {
		mLastSignalGeneratorArgument*=-1.0;
		value = mLastSignalGeneratorSample + mLastSignalGeneratorArgument;
	}

	mLastSignalGeneratorSample = value;
	return value;
}

double	SignalGenerator::generateRamp (void) {
	if (mLastSignalGeneratorArgument == 0.0) {
	//first call - short cut
		mLastSignalGeneratorArgument = 2.0*mSignalFrequency/mSamplingFrequency; 
		mLastSignalGeneratorSample = -1.0;
		return -1.0;
	}

	double value = mLastSignalGeneratorSample + mLastSignalGeneratorArgument;

	if (value > 1.0) {
		value = -1.0;
	}

	mLastSignalGeneratorSample = value;
	return value;
}

double	SignalGenerator::generateSquare (void) {
	double value = 0.0;

	if (mLastSignalGeneratorArgument <= (0.5 / mSignalFrequency)) {
		value = 1.0;
		mLastSignalGeneratorArgument = mLastSignalGeneratorArgument + 1.0 / mSamplingFrequency;
	} else {
		if (mLastSignalGeneratorArgument < (1.0 / mSignalFrequency)) {
			value = -1.0;
			mLastSignalGeneratorArgument = mLastSignalGeneratorArgument + 1.0 / mSamplingFrequency;
		} else {
			value = 1.0;
			mLastSignalGeneratorArgument = 0.0;
		}
	}

	mLastSignalGeneratorSample = value;
	return value;
}

double SignalGenerator::generateNoise (void) {
	return (double)gennor(0,1);
}
