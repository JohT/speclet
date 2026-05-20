## Plan: Relieve Audio Thread (Issue #33)

**TL;DR:** Move all DSP (FFT/wavelet) off the audio thread onto a dedicated `juce::Thread`. The audio thread writes samples only into a lock-free FIFO; the background thread drains it, runs calculations, handles transformation rebuilds on parameter changes, and writes results. Also fix `SpectralDataBuffer`'s broken write guard and decouple UI rendering to the message thread.

---

### Phase 1 — Lock-Free Input FIFO

1. Add `juce::AbstractFifo inputFifo` + `std::vector<float> inputCircularBuffer` to `SpecletPluginProcessor`. Pre-allocate in `prepareToPlay()` — note `samplesPerBlock` is currently *ignored* there; use it.
2. In `processBlock()`: remove `ScopedLock criticalSection` and all `currentTransformation->setNextInputSample()` calls. Write samples into FIFO instead (overwrite oldest when full — your chosen policy). Signal the DSP thread wake event.
3. `SpecletPluginProcessor` no longer owns or touches `currentTransformation` directly.

---

### Phase 2 — DSP Background Thread (`DspThread`)

4. New `src/dsp/DspThread.h/.cpp` extending `juce::Thread`.
5. `DspThread` owns: reference to the input FIFO, `std::unique_ptr<Transformation>`, `juce::WaitableEvent newDataAvailable`, and a pending-parameter snapshot.
6. `run()` loop: wait → check for pending parameter change (rebuild transformation if needed) → drain FIFO via `setNextInputSample()` → repeat.
7. Started in `prepareToPlay()`, stopped in `SpecletPluginProcessor` destructor.

*Depends on Phase 1.*

---

### Phase 3 — Fix `SpectralDataBuffer` Thread Safety

8. Change `bool mWriteAccess` → `std::atomic<bool> mWriteAccess{false}` in `src/data/SpectralDataBuffer.h`.
9. Fix the `read()` / `write()` guards accordingly. (The existing `CriticalSection` is commented out; this replaces it with the minimal correct fix for SPSC.)

*Can run parallel with Phase 2.*

---

### Phase 4 — Parameter Changes Off the Audio Thread

10. Replace the shared `juce::CriticalSection criticalSection` (currently contended between `processBlock` and `parameterChanged`) with:
    - `std::atomic<bool> transformationChangePending`
    - A parameter snapshot struct guarded by a non-RT lock (non-audio threads only)
11. `parameterChanged()`: write snapshot, set flag, signal DSP thread.
12. DSP `run()` loop: if flag set, rebuild transformation under non-RT lock, swap `unique_ptr`, clear flag.
13. `processBlock()` needs no lock at all — only touches the lock-free FIFO.

*Depends on Phase 2.*

---

### Phase 5 — Decouple UI Rendering from DSP Thread

14. `SpecletDrawer::onTransformationEvent()` currently renders into `spectrumImage` directly (was on audio thread, will now be on DSP thread — still wrong for a JUCE Image).
15. Remove rendering from `onTransformationEvent`; set an availability flag instead.
16. Move rendering into `timerCallback()` (already fires every 20ms): drain `SpectralDataBuffer` → `appendSpectralImage()` per entry → `repaint()`.
17. Remove `waitForFinishedSpectrum` `WaitableEvent` from `src/ui/SpecletDrawer.h` (it synchronized audio→paint, no longer needed).

*Depends on Phase 3.*

---

### Relevant Files

| File | Change |
|---|---|
| `src/SpecletPluginProcessor.h` / `.cpp` | Remove criticalSection from processBlock, add FIFO, delegate to DspThread |
| `src/dsp/DspThread.h/.cpp` | **New** — background DSP worker |
| `src/data/SpectralDataBuffer.h` / `.cpp` | Fix atomic bool |
| `src/dsp/transformations/Transformation.h` / `.cpp` | `setNextInputSample`, `calculationFrame`, listener notification |
| `src/ui/SpecletDrawer.h` / `.cpp` | Move rendering to timerCallback, remove waitForFinishedSpectrum |

---

### Unit Tests

Write new unit tests for critical components (add to `test/` directory, following the pattern of `RenderingHelperTest.cpp`):

1. **`DspThreadTest.cpp`** — Test `DspThread`:
   - Verify DSP thread starts/stops cleanly
   - Verify FIFO is drained and samples reach the transformation
   - Verify parameter change triggers transformation rebuild
   - Verify thread handles rapid parameter changes without crashing

2. **`SpectralDataBufferTest.cpp`** — Test thread safety:
   - Concurrent write/read from different threads (SPSC scenario)
   - Verify no race conditions with `std::atomic<bool> mWriteAccess`
   - Test buffer auto-clear when capacity exceeded
   - Verify `read()` and `write()` are truly lock-free or safely guarded

3. **`SpecletAudioProcessorTest.cpp`** (extend existing if present):
   - Verify `processBlock()` no longer blocks on any lock
   - Verify `prepareToPlay()` correctly pre-allocates FIFO based on `samplesPerBlock`
   - Verify parameter changes don't lock the audio thread

4. **`SpecletDrawerTest.cpp`** (if UI rendering is testable):
   - Verify `timerCallback()` correctly drains `SpectralDataBuffer` and renders
   - Verify `onTransformationEvent()` no longer performs rendering
   - Verify no image corruption from concurrent thread access

---

### Verification

1. Run all unit tests: `ctest --test-dir build/test` — all existing and new tests pass
2. Load in JUCE AudioPluginHost → switch transforms under load → no xruns
3. Enable highest-resolution wavelet → audio thread CPU stays flat
4. Rapidly switch transform type → no crash, no UI freeze
5. ThreadSanitizer (`-fsanitize=thread`) — especially around `SpectralDataBuffer` and parameter snapshot
6. Waterfall display still renders correctly (no blank columns)

---

### Scope

**In:** Lock-free audio→DSP handoff, `DspThread`, `SpectralDataBuffer` fix, parameter changes off audio thread, UI rendering on message thread.  
**Out:** `TransformationFactory` singleton refactor, sampleRate change handling in `prepareToPlay`, new transform types.

---

### General Guidelines

#### Think Before Coding

**Don't assume. Don't hide confusion. Surface tradeoffs.**

Before implementing:
- State your assumptions explicitly. If uncertain, ask.
- If multiple interpretations exist, present them - don't pick silently.
- If a simpler approach exists, say so. Push back when warranted.
- If something is unclear, stop. Name what's confusing. Ask.

#### Simplicity First

**Minimum code that solves the problem. Nothing speculative.**

- No features beyond what was asked.
- No abstractions for single-use code.
- No "flexibility" or "configurability" that wasn't requested.
- No error handling for impossible scenarios.
- If you write 200 lines and it could be 50, rewrite it.

Ask yourself: "Would a senior engineer say this is overcomplicated?" If yes, simplify.

#### Surgical Changes

**Touch only what you must. Clean up only your own mess.**

When editing existing code:
- Ask before improving adjacent code, comments, or formatting.
- Don't refactor things that aren't broken.
- Match existing style, even if you'd do it differently.
- If you notice unrelated dead code, mention it - don't delete it.

When your changes create orphans:
- Remove imports/variables/functions that YOUR changes made unused.
- Ask before removing pre-existing dead code

The test: Every changed line should trace directly to the user's request.