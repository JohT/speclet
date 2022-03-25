#include "../src/data/SpectralDataBuffer.h"
#include "../src/data/SpectralDataInfo.h"
#include "../src/dsp/transformations/TransformationFactory.h"
#include "../src/dsp/windowing/WindowFunctions.h"
#include "../src/plugin/SpectronParameters.h"
#include "../src/ui/ColourGradients.h"
#include "../src/utilities/PerformanceLogger.h"
#include <time.h>

//#include <vld.h>
typedef struct {
    double samplingRate;
    double samplingPeriod;
    double frequencyResolutionInHz;
    unsigned int resolution;
    short channels;
} TCommonSettings;

TCommonSettings setCommonSettings();
TCommonSettings setCommonSettings(double samplingRate, double resolution, short channels);
void printCommonSettings(TCommonSettings settings);
