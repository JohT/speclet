#include "TestCases.h"

int test_WaveletTransormation (TCommonSettings settings) {
	//this test will be done at a lower resolution for an easier result-checking
	//settings.resolution = 16;	

	Transformation* transformation = TRANSFORM_FACTORY->createTransformation(
		SpectronParameters::TRANSFORM_FWT,
		settings.samplingRate, 
		settings.resolution,
		SpectronParameters::WINDOWING_SQUARE,
		SpectronParameters::WAVELET_VAIDYANATHAN_18
	);
	assert(transformation);

	printCommonSettings(settings);

	int countOk = 0;
	int countDriftedFrequency = 0;
	int countDoubleFrequency = 0;
	int maxWiederholungen = 1;

	for (int wiederholungen = 1; wiederholungen <= maxWiederholungen; wiederholungen++) {
		double			frequency			= settings.resolution / 100.0f * pow(wiederholungen,2.6);
		double			omega					= 2.0f * M_PI * frequency;
		double			periode				= 1.0f / frequency;
		double			samplesPerPeriode	= settings.samplingRate * periode; 
		printf("\nfrequency=%lf, periode=%lf ms, samplesPerPeriode=%lf\n", frequency, periode, samplesPerPeriode);

		for (int t = 0; t < settings.resolution * 10; t++) {
		//Schleife mit Testdaten (generiertes Sinus-Signal)
			double sample = 0.1f * sin(omega * t * settings.samplingPeriod);
			for (int channel = 0; channel < settings.channels; channel++) {
				assert(sample >= -1.0);
				assert(sample <=  1.0);
				transformation->setNextInputSample(sample);
			}
		}

		double max_magnitudeDB				= -1000.0f;
		double frequencyOfSpectralLine	= 0.0f;

		SpectralDataBuffer*	spectralDataBuffer	= transformation->getSpectralDataBuffer();
		SpectralDataInfo*		spectralDataInfo		= transformation->getSpectralDataInfo();
		assert(spectralDataBuffer);
		assert(spectralDataInfo);

		int processedSpectralDataBufferEntries = 0;

		while (transformation->isOutputAvailable()) {
			SpectralDataBuffer::ItemType	spectralData;
			spectralDataBuffer->read(&spectralData);
			assert(spectralData.size()==settings.resolution);

			SpectralDataBuffer::ItemStatisticsType statistics = spectralDataBuffer->getStatistics(&spectralData);

			double	frequencyOfMax	= 0;
			int	indexOfMainSpectralLine = 0;

			// find max frequency & amplitude
			for (int specLine = 0; specLine < spectralData.size(); specLine++) {
				if (spectralData[specLine] == statistics.max) {
					indexOfMainSpectralLine = specLine;
					frequencyOfMax = (double)specLine * (settings.samplingRate / settings.resolution / 2.0f);
				}
			}
			assert (indexOfMainSpectralLine > 0);
			assert (indexOfMainSpectralLine < spectralData.size());

			printf("W[%1d,%1d]: min=%8.4lf, max=%8.4lf (@%6.2lf Hz), avg=%8.4lf\n", 
				processedSpectralDataBufferEntries, 
				wiederholungen, 
				statistics.min, 
				statistics.max, 
				frequencyOfMax,
				statistics.avg
			);
			printf("        DataInfo: fRes=%3d, tRes=%3d", 
				spectralDataInfo->getFrequencyResolution(), 
				spectralDataInfo->getTimeResolution() 
			);

			//test auto check
			if (((frequencyOfMax + settings.frequencyResolutionInHz) > frequency)
			&&  ((frequencyOfMax - settings.frequencyResolutionInHz) < frequency)) {
				printf(" ...... ok\n");
				countOk++;
			} else {
				printf(" ...... wrong frequency\n");
				printf("->indexOfMainSpectralLine=%d\n", indexOfMainSpectralLine);
				countDriftedFrequency++;
			}
			if (((frequencyOfMax / 2.0f + settings.frequencyResolutionInHz) > frequency)
			&&  ((frequencyOfMax / 2.0f - settings.frequencyResolutionInHz) < frequency)) {
				countDoubleFrequency++;
			}
		}

		fflush(stdout);
		Sleep(500);
	}

	printf("OK: %i, fequency<>source: %i, frequency=source*2: %i", countOk, countDriftedFrequency, countDoubleFrequency);
	if (countOk < countDriftedFrequency) {
	printf("\nclosed\ntest failed!\n");
	fflush(stdout);
		return 1;
	}
	printf("\nclosed\ntest successful!\n");
	fflush(stdout);

	char c;
	scanf_s(&c);
	return 0;
}

