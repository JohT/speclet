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



### Build x86 on Mac M1 (not working)

These are the references and commands used to build x86_64 on a mac m1 (arm). After trying these and several other things it didn't work.

- [Run x86 Terminal Apps (Like Homebrew) on Your New M1 Mac](https://medium.com/swlh/run-x86-terminal-apps-like-homebrew-on-your-new-m1-mac-73bdc9b0f343)
- [StackTrace - x86 installation of brew](https://stackoverflow.com/questions/67386941/using-x86-libraries-and-openmp-on-macos-arm64-architecture#67418208)
- [StackTrace - proper way to build for macOS-x86_64 using cmake on Apple M1](https://stackoverflow.com/questions/69803659/what-is-the-proper-way-to-build-for-macos-x86-64-using-cmake-on-apple-m1-arm)

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

```shell
arch -x86_64 /usr/local/opt/cmake/bin/cmake --no-warn-unused-cli -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DJUCE_BUILD_EXTRAS=OFF -DJUCE_BUILD_EXAMPLES=OFF -DCMAKE_C_COMPILER:FILEPATH=/usr/local/opt/llvm/bin/clang -DCMAKE_CXX_COMPILER:FILEPATH=/usr/local/opt/llvm/bin/clang++ -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY -S/Users/johnny/Repositories/git/speclet -B/Users/johnny/Repositories/git/speclet/build -G Ninja
```