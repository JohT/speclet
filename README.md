# Speclet

VST Audio Spectrum Analyzer Plugin using Fourier- and Wavelet-Transformation.

- (c)2011 by Johannes Troppacher (Austria)   
- For graduation at HS Mittweida - University of applied science (Germany)  
- Paper (german): [Echtzeitspektralanalyse auf Basis der Fourier- und Wavelet-Transformation implementiert als VST-Plugin](https://monami.hs-mittweida.de/frontdoor/deliver/index/docId/3216/file/J.Troppacher_2011_Diplomarbeit.pdf)  
- License: [GNU GENERAL PUBLIC LICENSE v3](./LICENSE)

### Used Tools

Modernized 2022 using the following tools:

- [The LLVM Compiler Infrastructure](https://github.com/llvm/llvm-project)
- [Ninja](https://github.com/ninja-build/ninja)
- [CMake](https://gitlab.kitware.com/cmake/cmake)
- [Setup-free CMake dependency management](https://github.com/cpm-cmake/CPM.cmake)
- [MagicEnumC++ - static reflection for enums](https://github.com/Neargye/magic_enum)

This version is not ready yet. 
It contains the code from back then, that doesn't compile any more.   
It also contains the plugin made with the tutorial [Learn Modern C++ by Building an Audio Plugin (w/ JUCE Framework) - Full Course YouTube](https://www.youtube.com/watch?v=i_Iq4_Kd7Rc&list=PLi4rQ_T_X31Gd4pyUbvPltTVSyw8v_yYT&index=5&t=1051s)
as a template to start the modernization.
### Used Libraries

This plugin was made in 2011 (modernized 2022) using the following frameworks and libraries:

- VST SDK 2.4 rev2 (2011)
by Steinberg   
http://www.steinberg.net/en/company/developer.html   
visit the link above for licensing

- VST 3 Audio Plug-Ins SDK (2022)
by Steinberg   
http://www.steinberg.net/en/company/developer.html   
visit the link above for licensing

- JUCE    
by Raw Material Software (2011) 
http://www.rawmaterialsoftware.com/juce (2011) 
License: https://github.com/juce-framework/JUCE/blob/master/LICENSE.md (2022)

- FFTW   
by MIT (Matteo Frigo and Steven G. Johnson)
http://www.fftw.org   
GNU General Public License

- WAVE++   
by Ryerson Computrational Signal Analysis Group   
(c) 1999 by S. E. Ferrando, L. A. Kolasa and N. Kovacevic   
https://math.ryerson.ca/~lkolasa/CppWavelet.html   
"
Permission is granted for anyone to copy, use, or modify these
programs and documents for purposes of research or education,
provided this copyright notice is retained, and note is made of
any changes that have been made.  Use for commercial applications is only
allowed by permission of the copyright holders.
"

- Visual Leak Detector (VLD)   
(c) 2005-2011 Dan Moulding, Arkadiy Shapkin, Laurent Lessieux   
https://github.com/dmoulding/vld   
GNU Lesser General Public License v2.1

## References

### Digital Signal Processing

- [&#11088; Window Functions and Their Applications in Signal Processing (PDF)](https://library.oapen.org/handle/20.500.12657/41686)
- [Understanding FFT Windows (PDF, 2013)](https://www.egr.msu.edu/classes/me451/me451_labs/Fall_2013/Understanding_FFT_Windows.pdf)
### Templates and Examples

- [&#11088; Pamplejuce](https://github.com/sudara/pamplejuce)
- [Cross-platform CI for JUCE audio plugins with Github Actions](https://github.com/maxwellpollack/juce-plugin-ci)
- [JUCE CMake Repo Prototype](https://github.com/eyalamirmusic/JUCECmakeRepoPrototype/blob/master/CMakeLists.txt)
- [Example using CPM package manager with JUCE](https://github.com/robbert-vdh/diopser/blob/master/CMakeLists.txt)
- [CLAP CLever Audio Plugin](https://github.com/free-audio/clap)
- [Integration of Catch, CMake and CMake CodeCoverage](https://github.com/fkromer/catch_cmake_coverage)

### Performance Tracing/Logging (Profiling, Instrumentation)

- [Google Chrome Browser Trace Log Viewer](chrome://tracing/)
- [The Cherno - VISUAL BENCHMARKING in C++ (YouTube)](https://www.youtube.com/watch?v=xlAH4dbMVnU&t=197s)
- [&#11088; Instrumentor.h (Code Snippet)](https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e)
- [Instrumentor.h David Churchill Mod (Code Snippet)](https://pastebin.com/qw5Neq4U)
- [Instrumentor.h Mattias Aronsson Mod](https://gist.github.com/maronsson/073840bf94e4d6df94c5f294a6e96e03)

### Testing
- [&#11088; CMake's CTest](https://cmake.org/cmake/help/latest/manual/ctest.1.html)
- [&#11088; Catch2 CMake Integration](https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md)
- [&#11088; Plugin Validator](https://github.com/Tracktion/pluginval)
- [Catch2 Data Generators](https://github.com/catchorg/Catch2/blob/devel/docs/generators.md)
- [Coverage for tests with cmake and catch2](https://stackoverflow.com/questions/65603144/how-to-get-coverage-for-tests-with-cmake-and-catch2)
- [Findcodecov.cmake](https://github.com/catchorg/Catch2/blob/devel/CMake/Findcodecov.cmake)

### Build x86 on Mac M1 (not working)

These are the references used to build x86_64 on a mac m1 (arm). After trying these and several other things it didn't work.

- [Run x86 Terminal Apps (Like Homebrew) on Your New M1 Mac](https://medium.com/swlh/run-x86-terminal-apps-like-homebrew-on-your-new-m1-mac-73bdc9b0f343)
- [StackTrace - x86 installation of brew](https://stackoverflow.com/questions/67386941/using-x86-libraries-and-openmp-on-macos-arm64-architecture#67418208)
```shell
# launch x86_64 shell
arch -x86_64 zsh  
# install x86_64 variant of brew 
arch -x86_64 /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
# install x86_64 variant of clang
arch -x86_64 /usr/local/bin/brew install llvm 
# compile using x86_64 variant of clang
/usr/local/opt/llvm/bin/clang++ -arch x86_64 omp_ex.cpp
```
```shell
# arm64 (default) location
/opt/homebrew/bin/brew
# x86_64 location
/usr/local/bin/brew
```
```shell
# Apple arm64 (default) location
/usr/bin/clang
# brew arm64 location
/opt/homebrew/opt/llvm/bin/clang
# brew x86_64 location
/usr/local/opt/llvm/bin/clang
```
- [StackTrace - proper way to build for macOS-x86_64 using cmake on Apple M1](https://stackoverflow.com/questions/69803659/what-is-the-proper-way-to-build-for-macos-x86-64-using-cmake-on-apple-m1-arm)

### Other

[Example using CPM package manager with JUCE](https://github.com/robbert-vdh/diopser/blob/master/CMakeLists.txt)
[C++ Singleton](https://stackoverflow.com/questions/1008019/c-singleton-design-pattern)
