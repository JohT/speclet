#include "TestCases.h"

int test_WindowFunctionsFactory(TCommonSettings settings) {
    double inputData[] = {32.0, 10.0, 20.0, 38.0, 37.0, 28.0, 38.0, 34.0,
                          18.0, 24.0, 18.0, 9.00, 23.0, 24.0, 28.0, 34.0};

    std::queue<std::shared_ptr<WindowFunction>> windowFunctionVector;

    printf("Every windowing class @resolution is gonna be read the first time and should be created....\n");
    for (long resolution = settings.resolution; resolution < settings.resolution * 3; resolution += settings.resolution) {
        for (int windowIndex = 1; windowIndex < WindowFunctionFactory::Method::NUMBER_OF_OPTIONS; windowIndex++) {
            std::shared_ptr<WindowFunction> windowFunction = WindowFunctionFactory::getSingletonInstance().getWindow(static_cast<WindowFunctionFactory::Method>(windowIndex), resolution);
            windowFunctionVector.push(windowFunction);

            const char *name = windowFunction->getName();
            printf("window [%4d] (N=%1d): %s loaded\n", resolution, windowIndex, name);
        }
    }

    printf("Every already created windowing class will be read again...\n");

    for (long resolution = settings.resolution; resolution < settings.resolution * 3; resolution += settings.resolution) {
        for (int windowIndex = 1; windowIndex < WindowFunctionFactory::Method::NUMBER_OF_OPTIONS; windowIndex++) {
            std::shared_ptr<WindowFunction> windowFunction = WindowFunctionFactory::getSingletonInstance().getWindow(static_cast<WindowFunctionFactory::Method>(windowIndex), resolution);
            assert(windowFunction == windowFunctionVector.front());//the references should be exactly equal here
            windowFunctionVector.pop();

            const char *name = windowFunction->getName();
            printf("window [%4d] (N=%1d): %s reloaded\n", resolution, windowIndex, name);
        }
    }

    printf("\nclosed\ntest successful!\n");
    fflush(stdout);

    return 0;
}