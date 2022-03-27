# Speclet

VST Audio Spectrum Analyzer Plugin using Fourier- and Wavelet-Transformation.

- (c)2011 by Johannes Troppacher (Austria)   
- For graduation at HS Mittweida - University of applied science (Germany)  
- Original title (german): "Echtzeitspektralanalyse auf Basis der Fourier- und Wavelet-Transformation implementiert als VST-Plugin"  
- License: [GNU GENERAL PUBLIC LICENSE v3](./LICENSE)

### Used Tools

Modernized 2022 using the following tools:

- [The LLVM Compiler Infrastructure](https://github.com/llvm/llvm-project)
- [Ninja](https://github.com/ninja-build/ninja)
- [CMake](https://gitlab.kitware.com/cmake/cmake)
- [Setup-free CMake dependency management](https://github.com/cpm-cmake/CPM.cmake)

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

### References

- [Example using CPM package manager with JUCE](https://github.com/robbert-vdh/diopser/blob/master/CMakeLists.txt)
- [C++ Singleton](https://stackoverflow.com/questions/1008019/c-singleton-design-pattern)
- [10 Cmake Tips & Tricks](https://medium.com/codex/10-cmake-tips-tricks-7f00d407923d)
- [JUCE CMake Repo Prototype](https://github.com/eyalamirmusic/JUCECmakeRepoPrototype/blob/master/CMakeLists.txt)
- [Pamplejuce](https://github.com/sudara/pamplejuce)
- [Reference to non-static member function must be called](https://stackoverflow.com/questions/26331628/reference-to-non-static-member-function-must-be-called)
- [Pointer-to-Member Function](http://www.codeguru.com/cpp/cpp/article.php/c17401/C-Tutorial-PointertoMember-Function.htm)
- [CLAP CLever Audio Plugin](https://github.com/free-audio/clap)
- [Commit Message Guidelines](https://gist.github.com/robertpainsi/b632364184e70900af4ab688decf6f53)
- [Undefined symbol _kIOMainPortDefault](https://forum.juce.com/t/juce-6-1-3-undefined-symbol-kiomainportdefault/49335/9)

### References regarding Performance Tracing/Logging (Profiling, Instrumentation)

- [Google Chrome Browser Trace Log Viewer](chrome://tracing/)
- [The Cherno - VISUAL BENCHMARKING in C++ (YouTube)](https://www.youtube.com/watch?v=xlAH4dbMVnU&t=197s)
- [Instrumentor.h (Code Snippet)](https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e)
- [Instrumentor.h David Churchill Mod (Code Snippet)](https://pastebin.com/qw5Neq4U)
- [Instrumentor.h Mattias Aronsson Mod](https://gist.github.com/maronsson/073840bf94e4d6df94c5f294a6e96e03)

### References regarding unit testing
- [CMake's CTest](https://cmake.org/cmake/help/latest/manual/ctest.1.html)
- [Catch2 CMake Integration](https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md)
- [Catch2 Data Generators](https://github.com/catchorg/Catch2/blob/devel/docs/generators.md)
- [Integration of Catch, CMake and CMake CodeCoverage](https://github.com/fkromer/catch_cmake_coverage)
- [Coverage for tests with cmake and catch2](https://stackoverflow.com/questions/65603144/how-to-get-coverage-for-tests-with-cmake-and-catch2)
- [Findcodecov.cmake](https://github.com/catchorg/Catch2/blob/devel/CMake/Findcodecov.cmake)