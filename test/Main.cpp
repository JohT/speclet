#include "TestCases.h"

int main(int argc, char *argv[]) {
    //Initialisation
    int returnCode = 0;
    TCommonSettings settings = setCommonSettings();

    //Test 1: WindowFunctions
    printf("test_WindowFunctionsFactory started...\n");
    returnCode = test_WindowFunctionsFactory(settings);
    assert(returnCode == 0);
    printf("test_WindowFunctionsFactory ended  ...\n");
    fflush(stdout);
    fflush(stdin);

    //Test 2: FourierTransormation
    printf("test_FourierTransormation(new)  started...\n");
    returnCode = test_FourierTransormation(settings);
    assert(returnCode == 0);
    printf("test_FourierTransormation(new)  ended  ...\n");
    fflush(stdout);
    fflush(stdin);

    //Test 3: WaveletTransormation
    printf("test_WaveletTransormation(new)  started...\n");
    returnCode = test_WaveletTransormation(settings);
    assert(returnCode == 0);
    printf("test_WaveletTransormation(new)  ended  ...\n");
    fflush(stdout);
    fflush(stdin);

    //Test 4: RenderingHelper
    printf("test_RenderingHelper  started...\n");
    returnCode = test_RenderingHelper(settings);
    assert(returnCode == 0);
    printf("test_RenderingHelper  ended  ...\n");
    fflush(stdout);
    fflush(stdin);

    //Test 5: test_DWPT_BestBasis
    printf("test_DWPT_BestBasis started...\n");
    returnCode = test_DWPT_BestBasis(settings);
    assert(returnCode == 0);
    printf("test_DWPT_BestBasis ended  ...\n");
    fflush(stdout);
    fflush(stdin);

    ////Test 6: FourierPerformance
    //printf("test_FourierPerformance started...\n");
    //returnCode = test_FourierPerformance(settings);
    //assert(returnCode == 0);
    //printf("test_FourierPerformance ended  ...\n");
    //fflush(stdout);
    //fflush(stdin);

    //tear down
    TransformationFactory::getSingletonInstance().destruct();

    return 0;
}