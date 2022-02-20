#include "TestCases.h"

int test_DWPT_BestBasis (TCommonSettings settings) {
	TCommonSettings localSettings = setCommonSettings(settings.samplingRate, 16, settings.channels);

	Transformation* transform = TRANSFORM_FACTORY->createTransformation(
		SpectronParameters::TRANSFORM_FWPT_BB,
		localSettings.samplingRate, 
		localSettings.resolution, 
		SpectronParameters::WINDOWING_SQUARE,
		SpectronParameters::WAVELET_DAUBECHIES_08
	);
	assert(transform);

	double inputData[] =		{	32.0, 10.0, 20.0, 38.0, 37.0, 28.0, 38.0, 34.0,
									18.0, 24.0, 18.0, 9.00, 23.0, 24.0, 28.0, 34.0 
	};

	for (int i = 0; i < localSettings.resolution; i++) {
		transform->setNextInputSample(inputData[i]/100.0);
	}
	
	SpectralDataBuffer* spectralDataQueue = transform->getSpectralDataBuffer();
	int dataAvailableCounter = 1;
	while (transform->isOutputAvailable()) {
		SpectralDataBuffer::ItemType spectrum;
		spectralDataQueue->read(&spectrum);

		long spectralLinesCount	= spectrum.size();
		bool em = spectrum.empty();
		assert(spectralLinesCount);

		for (int i = 0; i < spectralLinesCount; i++) {
			printf("[%2d,%2d]=%6.2lf ", dataAvailableCounter, i,sqrt(spectrum[i]));
			if (((i+1)%4) == 0) printf("\n");
		}
		printf("-------------------------------------------------------------------\n");
		dataAvailableCounter++;
	}

	printf("\nclosed\ntest successful!\n");
	fflush(stdout);

	char c;
	scanf_s(&c);
	return 0;
}