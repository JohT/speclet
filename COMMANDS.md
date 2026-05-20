# Speclet Commands

This document lists the steps and commands that were executed to set up the project or that can be used to build and test the project.

## Install Tools

- [llvm (clang)](https://clang.llvm.org/get_started.html)
- [cmake](https://cmake.org)
- [ninja](https://ninja-build.org)

## Initial build

```shell
cmake -Bbuild -DJUCE_BUILD_EXTRAS=ON -DJUCE_BUILD_EXAMPLES=OFF -DCMAKE_BUILD_TYPE:STRING=Debug -G Ninja
```

```shell
cmake --build build
```

## Build AudioPluginHost to test the plugin

```shell
cmake.exe --build build --config Debug --target AudioPluginHost
```

## Run Unit-Tests

This project uses [Catch2](https://github.com/catchorg/Catch2) for unit testing and [CTest](https://cmake.org/cmake/help/latest/manual/ctest.1.html) for test execution.

In case needed, the unit tests can be built with the following command.

```shell
cmake.exe --build build --config Debug --target SpecletTests
```

From the root directory, start the unit test with the following command.

```shell
ctest --test-dir build/test
```

## Code Quality Analysis with clang-tidy

This project uses [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) for static code analysis.

### Local clang-tidy analysis

First, ensure clang-tools is installed on your system:

**macOS:**

```shell
brew install llvm
```

**Linux (Ubuntu/Debian):**

```shell
sudo apt-get install clang-tools
```

Then run clang-tidy on all project source files:

```shell
cd build
find ../src -type f \( -name '*.cpp' -o -name '*.h' \) -print0 | xargs -0 clang-tidy -p .
```

Or analyze specific files:

```shell
cd build
clang-tidy -p . ../src/SpecletPluginProcessor.cpp
```

### Continuous Integration

clang-tidy is automatically run on Linux and macOS in the GitHub Actions CI/CD pipeline. Results are displayed in the build logs.

### Create and update package-lock.cmake

As described in [CPM Package-lock](https://github.com/cpm-cmake/CPM.cmake/wiki/Package-lock), `package-lock.cmake` can be created and updated using the following commands:

```shell
cmake -H. -Bbuild
cmake --build build --target cpm-update-package-lock 
```

The advantage of this approach is that there is one distinct file that contains all dependencies and versions. [Renovate](https://github.com/renovatebot/renovate) is used to update those dependencies automatically as configured in [renovate.json](./renovate.json).
