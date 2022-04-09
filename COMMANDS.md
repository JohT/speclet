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

## Run Unit-Tests

From this directory start the unit test with the following command. 

```shell
ctest --test-dir build/test
```

[CTest](https://cmake.org/cmake/help/latest/manual/ctest.1.html)


## Build for x86_64 on Mac with M1 (not working yet)

```shell
arch -x86_64 /usr/local/opt/cmake/bin/cmake --no-warn-unused-cli -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DJUCE_BUILD_EXTRAS=OFF -DJUCE_BUILD_EXAMPLES=OFF -DCMAKE_C_COMPILER:FILEPATH=/usr/local/opt/llvm/bin/clang -DCMAKE_CXX_COMPILER:FILEPATH=/usr/local/opt/llvm/bin/clang++ -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY -S/Users/johnny/Repositories/git/speclet -B/Users/johnny/Repositories/git/speclet/build -G Ninja
```