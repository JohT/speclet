#include "CommonTestUtils.h"

TCommonSettings setCommonSettings() {
    return setCommonSettings(44100, 4096, 1);
}

TCommonSettings setCommonSettings(double samplingRate, double resolution, short channels) {
    TCommonSettings commonSettings;
    commonSettings.samplingRate = samplingRate;
    commonSettings.samplingPeriod = 1.0 / samplingRate;
    commonSettings.frequencyResolutionInHz = (double) samplingRate / (double) resolution;
    commonSettings.resolution = resolution;
    commonSettings.channels = channels;
    return commonSettings;
}

void printCommonSettings(TCommonSettings settings) {
    printf("commmon settings: resolution=%5d, channels=%2d, samplingrate=%6.0lf Hz, samplingPeriod=%0.8lf ms, deltaF_FFT=%6.2lf Hz \n",
           settings.resolution,
           settings.channels,
           settings.samplingRate,
           settings.samplingPeriod,
           settings.frequencyResolutionInHz);
}
