message(STATUS "Environment setup started")

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()

# Set C++ Language Standard to C++20
set(CMAKE_CXX_STANDARD 20)

# Generates a JSON file containing compile info like include paths, defines, etc. for clangd (IDE support)
# Reference: https://stackoverflow.com/questions/39455090/clang-tidy-cant-find-my-header-files
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Download "cpm", the setup-free CMake dependency management 
# https://github.com/cpm-cmake/CPM.cmake
set(CPM_DOWNLOAD_VERSION 0.35.0)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()
include(${CPM_DOWNLOAD_LOCATION})

#Minimum MacOS target, set globally
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment version" FORCE)

option(UniversalBinary "Build universal binary for mac" OFF)

if (UniversalBinary)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
endif()

#static linking in Windows
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

message(STATUS "Environment setup finished")