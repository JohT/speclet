# vst3-first-steps 
This document lists the steps and commands that were executed to set up the project.
It also contains the commands that are used to build it.

## Install Tools

- [llvm (clang)](https://clang.llvm.org/get_started.html)
- [cmake](https://cmake.org)
- [ninja](https://ninja-build.org)

## Setup lib folder with JUCE as submodule

Inside the newly setup GIT repository:
```
mkdir lib
git submodule add https://github.com/juce-framework/JUCE
```

Then `add_subdirectory(JUCE)` was uncommented in [CMakeLists.txt](#./CMakeLists.txt) and changed to `add_subdirectory(lib/JUCE)`

## Initial build (including Mac OSX both Architectures)

```shell
cmake -Bbuild -DJUCE_BUILD_EXTRAS=ON -DJUCE_BUILD_EXAMPLES=ON -DCMAKE_OSX_ARCHITECTURES=arm64;x86_64
```

```shell
cmake --build build
```