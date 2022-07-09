# Speclet Commands
This document lists the steps and commands that were executed to set up the project or that can be 
used to build and test the project.

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

## Build AudioPluginHost to test the plugin:

```shell
cmake.exe --build build --config Debug --target AudioPluginHost
```

## Run Unit-Tests

From this directory start the unit test with the following command. 

```shell
ctest --test-dir build/test
```

[CTest](https://cmake.org/cmake/help/latest/manual/ctest.1.html)