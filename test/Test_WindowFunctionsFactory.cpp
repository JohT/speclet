#include "TestCases.h"

int test_WindowFunctionsFactory (TCommonSettings settings) {
	double inputData[] =		{	32.0, 10.0, 20.0, 38.0, 37.0, 28.0, 38.0, 34.0,
		18.0, 24.0, 18.0, 9.00, 23.0, 24.0, 28.0, 34.0 
	};

	queue<const WindowFunction*>	windowFunctionVector;

	printf("Every windowing class @resolution is gonna be read the first time and should be created....\n");
	for (long resolution = settings.resolution; resolution < settings.resolution*3; resolution+=settings.resolution) {
		for (int windowIndex = 1; windowIndex < SpectronParameters::WINDOWINGS_NumOptions; windowIndex++) {
			WindowFunction* windowFunction = WINFUNC->createWindowFunction(windowIndex, resolution);
			assert(windowFunction);
			windowFunctionVector.push(windowFunction);

			char* name = windowFunction->getName();
			printf("window [%4d] (N=%1d): %s loaded\n", resolution, windowIndex, name);
		}
	}

	printf("Every already created windowing class will be read again...\n");

	for (long resolution = settings.resolution; resolution < settings.resolution*3; resolution+=settings.resolution) {
		for (int windowIndex = 1; windowIndex <  SpectronParameters::WINDOWINGS_NumOptions; windowIndex++) {
			WindowFunction* windowFunction = WINFUNC->createWindowFunction(windowIndex, resolution);
			assert(windowFunction);
			assert(windowFunction == windowFunctionVector.front());	//the references should be exactly equal here
			windowFunctionVector.pop();

			char* name = windowFunction->getName();
			printf("window [%4d] (N=%1d): %s reloaded\n", resolution, windowIndex, name);
		}
	}

	printf("\nclosed\ntest successful!\n");
	fflush(stdout);

	char c;
	scanf_s(&c);
	return 0;
}