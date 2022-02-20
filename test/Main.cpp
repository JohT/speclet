#include "TestCases.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//Initialisation
	int returnCode = 0;
	TCommonSettings settings = setCommonSettings();

	//Test 1: WindowFunctions
	printf("test_WindowFunctionsFactory startet...\n");
	returnCode = test_WindowFunctionsFactory(settings);
	assert(returnCode == 0);
	printf("test_WindowFunctionsFactory endet  ...\n");
	fflush(stdout);
	fflush(stdin);

	//Test 2: FourierTransormation
	printf("test_FourierTransormation(new)  startet...\n");
	returnCode = test_FourierTransormation(settings);
	assert(returnCode == 0);
	printf("test_FourierTransormation(new)  endet  ...\n");
	fflush(stdout);
	fflush(stdin);

	//Test 3: WaveletTransormation
	printf("test_WaveletTransormation(new)  startet...\n");
	returnCode = test_WaveletTransormation(settings);
	assert(returnCode == 0);
	printf("test_WaveletTransormation(new)  endet  ...\n");
	fflush(stdout);
	fflush(stdin);

	//Test 4: RenderingHelper
	printf("test_RenderingHelper  startet...\n");
	returnCode = test_RenderingHelper(settings);
	assert(returnCode == 0);
	printf("test_RenderingHelper  endet  ...\n");
	fflush(stdout);
	fflush(stdin);

	//Test 5: test_DWPT_BestBasis
	printf("test_DWPT_BestBasis startet...\n");
	returnCode = test_DWPT_BestBasis(settings);
	assert(returnCode == 0);
	printf("test_DWPT_BestBasis endet  ...\n");
	fflush(stdout);
	fflush(stdin);

	////Test 6: FourierPerformance
	//printf("test_FourierPerformance startet...\n");
	//returnCode = test_FourierPerformance(settings);
	//assert(returnCode == 0);
	//printf("test_FourierPerformance endet  ...\n");
	//fflush(stdout);
	//fflush(stdin);

	//tear down
	TransformationFactory::getSingletonInstance()->destruct();
	WindowFunctionsFactory::getSingletonInstance()->destruct();
	PerformanceManager::getSingletonInstance()->destruct();
	ColourGradients::getSingletonInstance()->destruct();
	SpectronParameters::getSingletonInstance()->destruct();

	return 0;
}