#include "TestCases.h"

#define ARRAYLENGTH 128
#define WINDOWHEIGHT 10

int test_RenderingHelper(TCommonSettings settings) {
    //override global settings for this especially this test case
    settings.resolution = ARRAYLENGTH;

    //create sut and dependencies
    RenderingHelper *render = new RenderingHelper();
    SpectralDataInfo info(settings.samplingRate, settings.resolution, settings.resolution);

    printf("method pixelToIndex lin.....\n");
    for (auto pixel = 0; pixel <= WINDOWHEIGHT; pixel++) {
        auto index = render->pixelToIndex(pixel, WINDOWHEIGHT, info, false);
        printf("[%3d]: %3d |", pixel, index);
        if (((pixel + 1) % 4) == 0) printf("\n");
    }

    printf("\n\nmethod pixelToIndex log.....\n");
    for (int pixel = 0; pixel <= WINDOWHEIGHT; pixel++) {
        auto index = render->pixelToIndex(pixel, WINDOWHEIGHT, info, true);
        printf("[%3d]: %3d |", pixel, index);
        if (((pixel + 1) % 4) == 0) printf("\n");
    }

    delete (render);

    printf("\nclosed\ntest successful!\n");
    fflush(stdout);

    return 0;
}