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
#include "..\..\libs\juce\JuceLibraryCode\JuceHeader.h"
#define PARAMETERS SpectronParameters::getSingletonInstance()


class SpectronParameters {
// --------------- constants --------------- //
public:
	//---------------------------------------------------------------------------
	//IMPORTANT: Since juce components like combo boxes can't handle index = 0, 
	//           enums must not contain a zero value 
	//           Only exception: PARAMETER_INDEX (since it has to do with VST SDK)
	//---------------------------------------------------------------------------

	static enum Constants {
		TIMEOUT_WAIT_BEFORE_SET = 5000
	};
	static enum Parameters {
	//Has to start with index=0 and end with TOTAL_NUMBER_OF_PARAMS
	//Be aware of that when you extend it
		PARAMETER_INDEX_Routing = 0,
		PARAMETER_INDEX_Transformation,
		PARAMETER_INDEX_Resolution,
		PARAMETER_INDEX_WaveletPaketBase,
		PARAMETER_INDEX_Windowing,
		PARAMETER_INDEX_Wavelet,
		PARAMETER_INDEX_Generator,
		PARAMETER_INDEX_GeneratorFrequency,
		PARAMETER_INDEX_LogFrequency,
		PARAMETER_INDEX_LogMagnitude,
		PARAMETER_INDEX_ColorMode,

		TOTAL_NUMBER_OF_PARAMS
	};
	static enum OptionsColormode {
		COLORMODE_BLUE			= 1,
		COLORMODE_GREEN			,
		COLORMODE_RAINBOW			,
		COLORMODE_FIRE				,

		COLORMODES_NumOptions,
		COLORMODE_DEFAULT		= COLORMODE_BLUE
	};
	static enum OptionsGenerator {
		GENERATOR_SINE			= 1,
		GENERATOR_TRANGLE			,
		GENERATOR_RAMP				,
		GENERATOR_SQUARE			,
		GENERATOR_NOISE			,

		GENERATORS_NumOptions,
		GENERATOR_DEFAULT		= GENERATOR_SINE
	};
	static enum OptionsPlotAxis {
		PLOT_AXIS_LINEAR = 1,
		PLOT_AXIS_LOGARITHMIC,
		
		PLOT_AXIS_NumOptions,
		PLOT_AXIS_DEFAULT	= PLOT_AXIS_LOGARITHMIC
	};
	static enum OptionsResolution  {
		RESOLUTION_256			= 256,
		RESOLUTION_512			= 512,
		RESOLUTION_1024		= 1024,
		RESOLUTION_2048		= 2048,
		RESOLUTION_4096		= 4096,
		RESOLUTION_8192		= 8192,
		RESOLUTION_16384		= 16384,
		RESOLUTION_32768		= 32768,
		RESOLUTION_65536		= 65536,

		RESOLUTION_MAX			= RESOLUTION_65536,
		RESOLUTION_DEFAULT	= RESOLUTION_4096
	};
	static enum OptionsResolutionRatio {
		RESOLUTION_RATIO_Equal	=	99,	
		RESOLUTION_RATIO_TimeX2 =  -2,
		RESOLUTION_RATIO_TimeX4	=	-1,
		RESOLUTION_RATIO_FreqX2	=	1,
		RESOLUTION_RATIO_FreqX4	=	2,

		RESOLUTION_RATIO_NumOptions = 5,
		RESOLUTION_RATIO_DEFAULT = RESOLUTION_RATIO_Equal
	};
	static enum OptionsRouting {
		ROUTING_GENERATOR = 1,
		ROUTING_L,
		ROUTING_R,
		ROUTING_MID,
		ROUTING_SIDE,

		ROUTING_NumOptions,
		ROUTING_DEFAUTL = ROUTING_MID
	};
	static enum OptionsTransform {
		TRANSFORM_FFT = 1,
		TRANSFORM_FWT,
		TRANSFORM_FWPT,
		TRANSFORM_FWPT_BB,
		TRANSFORM_OFF,

		TRANSFORM_NumOptions,
		TRANSFORM_DEFAULT = TRANSFORM_FFT
	};
	static enum OptionsWavelet {
		WAVELET_DAUBECHIES_02 =	1,
		WAVELET_DAUBECHIES_04,
		WAVELET_DAUBECHIES_06,
		WAVELET_DAUBECHIES_08,
		WAVELET_DAUBECHIES_10,
		WAVELET_DAUBECHIES_12,
		WAVELET_DAUBECHIES_14,
		WAVELET_DAUBECHIES_16,
		WAVELET_DAUBECHIES_18,
		WAVELET_DAUBECHIES_20,
		WAVELET_COIFMAN_06,
		WAVELET_COIFMAN_12,
		WAVELET_COIFMAN_18,
		WAVELET_COIFMAN_24,
		WAVELET_COIFMAN_30,
		WAVELET_BEYLKIN_18,
		WAVELET_VAIDYANATHAN_18,

		WAVELETS_NumOptions,
		WAVELET_HAAR = WAVELET_DAUBECHIES_02,
		WAVELET_DEFAULT = WAVELET_DAUBECHIES_08
	};
	static enum OptionsWindowing {
		WINDOWING_BARTLETT	= 1	,
		WINDOWING_BLACKMAN			,
		WINDOWING_BLACKMAN_HARRIS	,
		WINDOWING_HAMMING				,
		WINDOWING_HANNING				,
		WINDOWING_PARZEN				,
		WINDOWING_WELCH				,
		WINDOWING_SQUARE				,
		WINDOWINGS_NumOptions		,
		WINDOWING_DEFAULT	= WINDOWING_BLACKMAN_HARRIS
	};

	const static juce::String PROPERTY_VALUE;
	const static juce::String PARAMETER_COLORMODE;
	const static juce::String PARAMETER_LOGMAGNITUDE;
	const static juce::String PARAMETER_LOGFREQUENCY;
	const static juce::String PARAMETER_RESOLUTION;
	const static juce::String PARAMETER_ROUTING;
	const static juce::String PARAMETER_GENERATOR;
	const static juce::String PARAMETER_GENERATORFREQUENCY;
	const static juce::String PARAMETER_TRANSFORMATION;
	const static juce::String PARAMETER_WAVELET;
	const static juce::String PARAMETER_WAVELETPAKETBASE;
	const static juce::String PARAMETER_WINDOWING;

private:
	static enum ChildIndizes {
		CHILD_INDEX_Parameters = 0,
		CHILD_INDEX_Metadata,
		
		CHILD_INDEX_NumIndizes
	};

// --------------- members --------------- //
private:
	static SpectronParameters* singletonInstance;

	juce::ScopedPointer<juce::ValueTree>		properties;
	juce::ScopedPointer<juce::WaitableEvent>	waitForParameterChange;
	juce::CriticalSection							criticalSection;

// --------------- methods --------------- //
public:
	static SpectronParameters* getSingletonInstance();
	void destruct();

	void	blockParameterChanges(void) {waitForParameterChange->reset();};
	void	unblockParameterChanges(void) {waitForParameterChange->signal();};

	void	setParameter(int index, float newValue);
	void	setParameter(juce::String name, float newValue);
	float	getParameter(int index);
	float	getParameter(juce::String name);	
	juce::String getParameterName(int index);
	int	getParameterIndex(juce::String name);	

	int	getColorMode()				{return (int)getParameter(PARAMETER_INDEX_ColorMode);}
	int	getGenerator()				{return (int)getParameter(PARAMETER_INDEX_Generator);}
	float	getGeneratorFrequency()	{return		 getParameter(PARAMETER_INDEX_GeneratorFrequency);}
	bool	getLogMagnitude()			{return (bool)getParameter(PARAMETER_INDEX_LogMagnitude);}
	bool	getLogFrequency()			{return (bool)getParameter(PARAMETER_INDEX_LogFrequency);}
	long	getResolution()			{return (long)getParameter(PARAMETER_INDEX_Resolution);}
	int	getRouting()				{return (int)getParameter(PARAMETER_INDEX_Routing);}
	int	getTransformation()		{return (int)getParameter(PARAMETER_INDEX_Transformation);}
	int	getWavelet()				{return (int)getParameter(PARAMETER_INDEX_Wavelet);}
	int	getWaveletPaketBase()	{return (int)getParameter(PARAMETER_INDEX_WaveletPaketBase);}
	int	getWindowing()				{return (int)getParameter(PARAMETER_INDEX_Windowing);}

	//Adds a listener by delegating it to juce::ValueTree (see juce API documentation)
	void addListener		(juce::ValueTree::Listener* listener, bool sendAllParametersForInitialisation = true);
	//Removes a listener by delegating it to juce::ValueTree (see juce API documentation)
	void removeListener	(juce::ValueTree::Listener* listener);
	//read and write to XML
	void					readFromXML(const XmlElement &xml);
	juce::XmlElement*	writeToXML() const {return properties->createXml();};

private:
	SpectronParameters(void);
	~SpectronParameters(void);
	SpectronParameters(const SpectronParameters&);

};

