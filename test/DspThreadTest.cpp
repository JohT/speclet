#include "../src/dsp/DspThread.h"
#include "../src/dsp/transformations/TransformationFactory.h"
#include "../src/dsp/transformations/TransformationParameters.h"
#include "../src/dsp/windowing/WindowParameters.h"

#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <vector>

// M_PI is not part of the C++ standard; use a local constant instead.
namespace {
constexpr double pi = 3.14159265358979323846;
} // namespace

static TransformationRebuildParams makeFftParams(double sampleRate = 44100.0,
                                                  Transformation::ResolutionType resolution = 1024) {
    TransformationRebuildParams params;
    params.transformationType = TransformationParameters::Type::FAST_FOURIER_TRANSFORM;
    params.sampleRate = sampleRate;
    params.resolution = resolution;
    params.windowFunction = WindowParameters::WindowFunction::BLACKMAN_HARRIS;
    return params;
}

// Clears the TransformationFactory's owned transformation before JUCE's static
// destructors run, avoiding a crash in the WaitableEvent mutex teardown order.
static void cleanupFactory() {
    TransformationFactory::getSingletonInstance().createTransformation(
            TransformationParameters::Type::BYPASS,
            44100.0, 1024,
            WindowParameters::WindowFunction::BLACKMAN_HARRIS);
}

SCENARIO("DspThread starts and stops cleanly") {
    GIVEN("a DspThread backed by an empty FIFO") {
        juce::AbstractFifo fifo(1024);
        std::vector<float> buffer(1024, 0.0f);
        DspThread dspThread(fifo, buffer);

        WHEN("the thread is started and then stopped") {
            dspThread.startThread();
            juce::Thread::sleep(50); // wait for thread start
            dspThread.stopThread(3000);

            THEN("the thread is no longer running") {
                REQUIRE(!dspThread.isThreadRunning());
            }
        }
    }
}

SCENARIO("DspThread drains FIFO samples into transformation") {
    GIVEN("a DspThread with a 1024-sample FIFO and an FFT transformation") {
        const int fifoSize = 1024;
        juce::AbstractFifo fifo(fifoSize);
        std::vector<float> buffer(fifoSize, 0.0f);
        DspThread dspThread(fifo, buffer);

        dspThread.requestTransformationRebuild(makeFftParams());
        dspThread.startThread();
        // Note: sleep-based synchronisation is an approximation and may be flaky on slow/busy CI machines.
        // TODO: replace with deterministic synchronisation once DspThread exposes a "transformation ready" signal.
        juce::Thread::sleep(100); // let the thread build the transformation

        WHEN("2048 sine samples are written into the FIFO") {
            const double omega = 2.0 * pi * 440.0 / 44100.0;
            for (int chunk = 0; chunk < 4; ++chunk) {
                int start1, size1, start2, size2;
                fifo.prepareToWrite(fifoSize / 4, start1, size1, start2, size2);
                for (int i = 0; i < size1; ++i) {
                    buffer[static_cast<std::size_t>(start1 + i)] = static_cast<float>(0.1 * std::sin(omega * (chunk * (fifoSize / 4) + i)));
                }
                for (int i = 0; i < size2; ++i) {
                    buffer[static_cast<std::size_t>(start2 + i)] = static_cast<float>(0.1 * std::sin(omega * (chunk * (fifoSize / 4) + size1 + i)));
                }
                fifo.finishedWrite(size1 + size2);
                dspThread.notifyNewData();
            }
            // Note: sleep-based; see TODO at GIVEN block above.
            juce::Thread::sleep(500); // let DSP thread process

            THEN("the FIFO is drained (empty)") {
                REQUIRE(fifo.getNumReady() == 0);
            }
        }

        dspThread.stopThread(3000);
    }
    cleanupFactory();
}

SCENARIO("DspThread handles a parameter change rebuild request") {
    GIVEN("a running DspThread") {
        juce::AbstractFifo fifo(1024);
        std::vector<float> buffer(1024, 0.0f);
        DspThread dspThread(fifo, buffer);

        dspThread.requestTransformationRebuild(makeFftParams());
        dspThread.startThread();
        // Note: sleep-based; see TODO in the "DspThread drains FIFO" scenario.
        juce::Thread::sleep(100);

        WHEN("a new transformation type is requested") {
            TransformationRebuildParams newParams = makeFftParams();
            newParams.transformationType = TransformationParameters::Type::FAST_WAVELET_TRANSFORM;
            dspThread.requestTransformationRebuild(newParams);
            juce::Thread::sleep(200); // wait for rebuild

            THEN("the thread does not crash and is still running") {
                REQUIRE(dspThread.isThreadRunning());
            }
        }

        dspThread.stopThread(3000);
    }
    cleanupFactory();
}

SCENARIO("DspThread handles rapid parameter changes without crashing") {
    GIVEN("a running DspThread") {
        juce::AbstractFifo fifo(2048);
        std::vector<float> buffer(2048, 0.0f);
        DspThread dspThread(fifo, buffer);

        dspThread.requestTransformationRebuild(makeFftParams());
        dspThread.startThread();
        // Note: sleep-based; see TODO in the "DspThread drains FIFO" scenario.
        juce::Thread::sleep(100);

        WHEN("many rebuild requests are fired in rapid succession") {
            for (int i = 0; i < 20; ++i) {
                auto params = makeFftParams();
                params.windowFunction = static_cast<WindowParameters::WindowFunction>((i % 7) + 1);
                dspThread.requestTransformationRebuild(params);
            }
            juce::Thread::sleep(500); // wait for all rebuilds to settle

            THEN("the thread is still running without crash") {
                REQUIRE(dspThread.isThreadRunning());
            }
        }

        dspThread.stopThread(3000);
    }
    cleanupFactory();
}
