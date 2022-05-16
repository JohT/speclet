add_library(compiler_options INTERFACE)
add_library(Speclet::CompilerOptions ALIAS compiler_options)

# Sets compile options for the target:
#if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#    #set(CMAKE_VERBOSE_MAKEFILE 1)
#    set_target_properties(compiler_options PROPERTIES COMPILE_DEFINITIONS "Bits64_;UNIX;_BOOL;LINUX;FUNCPROTO;_GNU_SOURCE;LINUX_64;REQUIRE_IOSTREAM")
#endif()

# This file doesn't contain compile options, that are already set by juce::juce_recommended_config_flags:
# https://github.com/juce-framework/JUCE/blob/master/extras/Build/CMake/JUCEHelperTargets.cmake

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang") # Using GNU or Clang compiler ("GNU-style" C++ compiler)
    # -fPIC is needed for position-indipendent code to overcome the following error while linking:
    # "relocation R_X86_64_32S against symbol `stdout@@GLIBC_2.2.5' can not be used when making a shared object; recompile with -fPIC"
    target_compile_options(compiler_options INTERFACE "-fPIC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")  # Using Microsoft Visual Studio C++
    # /Gy: Allows the compiler to package individual functions in the form of packaged functions (COMDATs). [Microsoft Docs]
    #      The main advantage of this is that if you have identical functions the linker can collapse them all down into one actual piece of code ("COMDAT folding"). [StackOverflow]
    # /nologo: Suppresses the display of the copyright banner when the compiler starts up and display of informational messages during compiling. [Microsoft Docs]
    # /permissive-: Specify standards conformance mode to the compiler. Use this option to help you identify and fix conformance issues in your code, to make it both more correct and more portable. [Microsoft Docs]
    target_compile_options(compiler_options INTERFACE "/Gy;/nologo;/permissive-")
endif()

# Sanitizers
# Based on https://github.com/tobanteAudio/juce-6-demo
set(SANITIZERS "")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    # Address Sanatizer
    # list(APPEND SANITIZERS "address") # TODO (JohT) Enable this again when failing unit tests are fixed 
    # Undefined Sanatizer
    list(APPEND SANITIZERS "undefined")

    # Thread Sanatizer (cannot be used with address sanatizer)
    list(APPEND SANITIZERS "thread")
endif()

# The Memory Sanatizer is only supported by Clang for Linux. 
# Since GNU C++ is used for Linux, it will never be used.
#if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#    list(APPEND SANITIZERS "memory")
#endif()

list(JOIN SANITIZERS ";-fsanitize=" LIST_OF_SANITIZERS)
if((LIST_OF_SANITIZERS) AND (NOT "${LIST_OF_SANITIZERS}" STREQUAL ""))
    set(SANITIZERS_COMPILE_OPTIONS "-fsanitize=${LIST_OF_SANITIZERS};-fno-omit-frame-pointer")
    target_compile_options(compiler_options INTERFACE "$<$<CONFIG:Debug>:${SANITIZERS_COMPILE_OPTIONS}>")
    target_link_libraries(compiler_options INTERFACE "$<$<CONFIG:Debug>:${SANITIZERS_COMPILE_OPTIONS}>")
endif()

# CLang compiler remarks for vectorisation
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    set(VECTORIZATION_REMARKS_COMPILE_OPTIONS "-Rpass=loop-vectorize;-Rpass-missed=loop-vectorize;-Rpass-analysis=loop-vectorize;-gline-tables-only;-gcolumn-info")
    target_compile_options(compiler_options INTERFACE "$<$<CONFIG:Debug>:${VECTORIZATION_REMARKS_COMPILE_OPTIONS}>")
endif()

# Displays the chosen target's compile options:
get_target_property(FINAL_COMPILE_OPTIONS compiler_options INTERFACE_COMPILE_OPTIONS)
message(STATUS "Compiler definitions added to target compiler_options for compiler ${CMAKE_CXX_COMPILER_ID} and build type ${CMAKE_BUILD_TYPE}: ${FINAL_COMPILE_OPTIONS}")
