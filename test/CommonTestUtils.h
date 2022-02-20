#include <tchar.h>
#include <Windows.h>
#include <time.h>
#include "WindowFunctions.h"
#include "TransformationFactory.h"
#include "PerformanceManager.h"
#include "../user interface/ColourGradients.h"
#include "SpectralDataBuffer.h"
#include "SpectralDataInfo.h"
#include "..\plugin\SpectronParameters.h"
//#include <vld.h>

#define WINFUNC				WindowFunctionsFactory::getSingletonInstance()
#define TRANSFORM_FACTORY	TransformationFactory::getSingletonInstance()
#define WAVELET				AbstractWaveletTransformation
using namespace std;

typedef struct {
	double			samplingRate;
	double			samplingPeriod;
	double			frequencyResolutionInHz;
	unsigned int	resolution;
	short				channels;
} TCommonSettings;

TCommonSettings setCommonSettings ();
TCommonSettings setCommonSettings (double samplingRate, double resolution, short channels);
void printCommonSettings (TCommonSettings settings);
