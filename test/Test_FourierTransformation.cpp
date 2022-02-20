#include "TestCases.h"
#define TRANSFORM	FFT	//aktuell zu testende Transformation

int test_FourierTransormation (TCommonSettings settings) {
	//Initialisierung
	Transformation* transformation = TRANSFORM_FACTORY->createTransformation(
		SpectronParameters::TRANSFORM_FFT, 
		settings.samplingRate, 
		settings.resolution
	);
	assert(transformation);

	printCommonSettings(settings);

	for (int wiederholungen = 1; wiederholungen <= 10; wiederholungen++) {
		double			frequency			= settings.resolution / 100.0f * pow(wiederholungen,2.6);
		double			omega					= 2.0f * M_PI * frequency;
		double			periode				= 1.0f / frequency;
		double			samplesPerPeriode	= settings.samplingRate * periode; 
		printf("\nfrequency=%lf, periode=%lf ms, samplesPerPeriode=%lf\n", frequency, periode, samplesPerPeriode);

		for (int t = 0; t < settings.resolution * 10; t++) {
		//Schleife mit Testdaten (generiertes Sinus-Signal)
			double sample = sin(omega * t * settings.samplingPeriod);
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
		//for (int i = 0; i < spectralDataBuffer->size(); i++) {
			SpectralDataBuffer::ItemType	spectralData;
			spectralDataBuffer->read(&spectralData);
			assert(spectralData.size()==(settings.resolution/2+1));

			SpectralDataBuffer::ItemStatisticsType statistics = spectralDataBuffer->getStatistics(&spectralData);

			double	frequencyOfMax	= 0;

			int	indexOfMainSpectralLine = floor(frequency / settings.frequencyResolutionInHz);
			assert (indexOfMainSpectralLine > 0);
			assert (indexOfMainSpectralLine <= spectralData.size());

			if (spectralData[indexOfMainSpectralLine] == statistics.max) {
				frequencyOfMax = (double)indexOfMainSpectralLine * settings.frequencyResolutionInHz;
			}

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
			} else {
				printf(" ...... failed\n");
				printf("->indexOfMainSpectralLine=%d", indexOfMainSpectralLine);

				return 1;
				break;
			}

			processedSpectralDataBufferEntries++;
		}

		fflush(stdout);
		Sleep(500);
	}

	printf("\nclosed\ntest successful!\n");
	fflush(stdout);

	char c;
	scanf_s(&c);
	return 0;
}

