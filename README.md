# Speclet

VST Audio Spectrum Analyzer Plugin using Fourier- and Wavelet-Transformation.

- (c)2011 by Johannes Troppacher (Austria)   
- For graduation at HS Mittweida - University of applied science (Germany)  
- Paper (german): [Echtzeitspektralanalyse auf Basis der Fourier- und Wavelet-Transformation implementiert als VST-Plugin](https://monami.hs-mittweida.de/frontdoor/deliver/index/docId/3216/file/J.Troppacher_2011_Diplomarbeit.pdf)  
- License: [GNU GENERAL PUBLIC LICENSE v3](./LICENSE)

## Build the project

### Prerequisites
- [Installing GIT](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)
- [Installing CMake](https://cmake.org/install/)
- [Download Ninja](https://github.com/ninja-build/ninja/releases)
- (Windows only) [Install Visual Studio Build Tools](https://visualstudio.microsoft.com/de/downloads/?q=build+tools) with Visual Studio Installer, select Desktop Development for C++ and add all optional "Clang" components
- (Optional) [Download Visual Studio Code](https://code.visualstudio.com/download)

### Clone
Download the project as [zip](https://github.com/JohT/speclet/archive/refs/heads/main.zip)
or clone it using GIT:
```shell
git clone https://github.com/JohT/speclet.git
```

### Command Line

See [COMMANDS.md](./COMMANDS.md) if you prefer to use the command line.

### Visual Studio Code (MacOS)
- [Using Clang in Visual Studio Code](https://code.visualstudio.com/docs/cpp/config-clang-mac)
- Install recommended extensions specified in [speclet.code-workspace](./speclet.code-workspace)
- Select the kit that fits to your machine (`STRG+SHIFT+P`, Type "CMAKE kit")

### Visual Studio Code (Windows)
- Install [Visual Studio Build Tools](https://visualstudio.microsoft.com/de/downloads/?q=build+tools) with Visual Studio Installer, select Desktop Development for C++ and add all optional Clang Features
- Install recommended extensions specified in [speclet.code-workspace](./speclet.code-workspace)
- Select the kit that fits your machine (e.g. amd64) (`STRG+SHIFT+P`, Type "CMAKE kit").
Successfully tested with "Visual Studio Build Tools 2022 Release - amd64".

## Testing

### Run Unit Tests within your command line interface (CLI):
```shell
ctest --test-dir build/test
```

### Run Unit Tests in Visual Studio Code
Build the project and use extension "matepek.vscode-catch2-test-adapter" to show all the Catch2 CTest cases
under "Testing" (left "activity bar", below extensions button).

### Build AudioPluginHost for exploratory testing
```shell
cmake.exe --build build --config Debug --target AudioPluginHost
```

## Used Tools

Modernized 2022 using the following tools:

- [The LLVM Compiler Infrastructure](https://github.com/llvm/llvm-project)
- [Ninja](https://github.com/ninja-build/ninja)
- [CMake](https://gitlab.kitware.com/cmake/cmake)
- [Setup-free CMake dependency management](https://github.com/cpm-cmake/CPM.cmake)
- [Plugin Validator](https://github.com/Tracktion/pluginval)

This version is not ready yet. 
It contains the code from back then, that doesn't compile any more.   
It also contains the plugin made with the tutorial [Learn Modern C++ by Building an Audio Plugin (w/ JUCE Framework) - Full Course YouTube](https://www.youtube.com/watch?v=i_Iq4_Kd7Rc&list=PLi4rQ_T_X31Gd4pyUbvPltTVSyw8v_yYT&index=5&t=1051s)
as a template to start the modernization.

## Used Libraries

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

- Implementation of C++20's std::span for older compilers
(c) 2019 by Tristan Brindle
https://github.com/tcbrindle/span
Boost Software License 1.0

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

### Windows Build
- [Visual Studio Build Tools Download](https://visualstudio.microsoft.com/de/downloads/?q=build+tools)

### Other
- [C++ Singleton](https://stackoverflow.com/questions/1008019/c-singleton-design-pattern)
- [10 Cmake Tips & Tricks](https://medium.com/codex/10-cmake-tips-tricks-7f00d407923d)
- [Reference to non-static member function must be called](https://stackoverflow.com/questions/26331628/reference-to-non-static-member-function-must-be-called)
- [Pointer-to-Member Function](http://www.codeguru.com/cpp/cpp/article.php/c17401/C-Tutorial-PointertoMember-Function.htm)
- [Commit Message Guidelines](https://gist.github.com/robertpainsi/b632364184e70900af4ab688decf6f53)
- [Undefined symbol _kIOMainPortDefault](https://forum.juce.com/t/juce-6-1-3-undefined-symbol-kiomainportdefault/49335/9)
- [CheckTypeSize (FFTW failed for multiple architectures)](https://cmake.org/cmake/help/latest/module/CheckTypeSize.html)
- [std::span implementation for C++11 and later](https://github.com/tcbrindle/span)
- [Article CMake Ninja Combo: The Gist](https://www.incredibuild.com/blog/cmake-ninja-combo-the-gist)
