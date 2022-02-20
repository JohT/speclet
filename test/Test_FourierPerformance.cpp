#include "TestCases.h"
#define SPEKTREN_PRO_GENERIERUNG 10
#define ANZAHL_SPEKTRALANALYSEN 500
#define MAX_FEHLGESCHLAGENE 10

//forward declarations
void generateInput(TCommonSettings, Transformation*, double, int);

int test_FourierPerformance (TCommonSettings settings) {
	int failCounter = 0;
	//Initialisierung des Zufallsgenerators
	srand (time(NULL));

	//Laden der Transformation mit den jeweiligen Einstellungen
	Transformation* transformation = TRANSFORM_FACTORY->createTransformation(
		SpectronParameters::TRANSFORM_FFT, 
		settings.samplingRate, 
		settings.resolution);
	assert(transformation);

	SpectralDataBuffer*	spectralDataBuffer = transformation->getSpectralDataBuffer();
	assert(spectralDataBuffer);

	printCommonSettings(settings);

	//Schleife 
	for (int wiederholungen = 1; wiederholungen <= (ANZAHL_SPEKTRALANALYSEN / SPEKTREN_PRO_GENERIERUNG); wiederholungen++) {
		//Ermitteln einer zufaelligen Frequenz in Hz und generieren des Eingabesignals
		double frequency = (rand() % (((int)settings.samplingRate / 2) - 1)) + 1;	
		generateInput(settings, transformation, frequency, SPEKTREN_PRO_GENERIERUNG);

		int processedSpectralDataBufferEntries = 0;

		while (transformation->isOutputAvailable()) {
			//Einlesen der Spektraldaten
			SpectralDataBuffer::ItemType	spectralData;
			spectralDataBuffer->read(&spectralData);
			assert(spectralData.size()==(settings.resolution/2+1));

			//Einlesen der Statistiken
			SpectralDataBuffer::ItemStatisticsType statistics = spectralDataBuffer->getStatistics(&spectralData);

			//Ermitteln der Frequenz der hoechsten Spektrallinie (bzw. pruefen der Vorahnung)
			double	frequencyOfMax	= 0;
			int	indexOfMainSpectralLine = floor(frequency / settings.frequencyResolutionInHz);
			assert (indexOfMainSpectralLine > 0);
			assert (indexOfMainSpectralLine < spectralData.size());

			//Kurze Konsolenausgabe
			printf("W[%2d,%2d] @%6.0lf Hz", 
				processedSpectralDataBufferEntries, 
				wiederholungen, 
				frequency
			);

			//Automatische Verifizierung, ob hoechste Spektrallinie = generierte Frequenz 
			if (spectralData[indexOfMainSpectralLine] == statistics.max) {
				printf(" ...... ok\n");
			} else {
				printf(" ...... failed\n");
				printf("->indexOfMainSpectralLine=%d", indexOfMainSpectralLine);
				failCounter++;
				if (failCounter > MAX_FEHLGESCHLAGENE) {
					return 1;
					break;
				}
			}

			processedSpectralDataBufferEntries++;
		}

		fflush(stdout);
		//Sleep(500);
	}

	printf("\nclosed\ntest ");
	if (failCounter > 0) printf("not fully (%d)", failCounter);	
	printf("successful!\n");

	fflush(stdout);

	char c;
	scanf_s(&c);
	return 0;
}

void generateInput(TCommonSettings settings, Transformation* transformation, double frequency, int spectrumCount) {
	double			omega					= 2.0f * M_PI * frequency;
	double			periode				= 1.0f / frequency;
	double			samplesPerPeriode	= settings.samplingRate * periode; 
	printf("\nfrequency=%lf, periode=%lf ms, samplesPerPeriode=%lf\n", frequency, periode, samplesPerPeriode);

	for (int t = 0; t < settings.resolution * spectrumCount; t++) {
		double sample = sin(omega * t * settings.samplingPeriod);
		for (int channel = 0; channel < settings.channels; channel++) {
			assert(sample >= -1.0);
			assert(sample <=  1.0);
			transformation->setNextInputSample(sample);
		}
	}
}