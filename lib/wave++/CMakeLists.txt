# This file was added later to the library 
# and is not part of the original library.
# The same license as the original library is applied to this file.
cmake_minimum_required(VERSION 3.15)
set(wavelet_library_target_name "wave++")
project("${wavelet_library_target_name}")

# add headers to the library
# GLOB_RECURSE is not recommended but used here for simplicity: https://cmake.org/cmake/help/latest/command/file.html?highlight=CONFIGURE_DEPENDS#filesystem
FILE(GLOB_RECURSE wavelet_library_headers CONFIGURE_DEPENDS "source/*.h")

message(VERBOSE "${wavelet_library_target_name} library headers added:")
foreach(header ${wavelet_library_headers})
   message(VERBOSE "'${header}'")
endforeach()

# add sources (and headers) to the library
# GLOB_RECURSE is not recommended but used here for simplicity: https://cmake.org/cmake/help/latest/command/file.html?highlight=CONFIGURE_DEPENDS#filesystem
FILE(GLOB_RECURSE library_sources CONFIGURE_DEPENDS "source/*.cc")

add_library("${wavelet_library_target_name}" STATIC ${library_sources} ${wavelet_library_headers})

message(VERBOSE "${wavelet_library_target_name} library sources added:")
foreach(source ${library_sources})
   message(VERBOSE "'${source}'")
endforeach()

# Sets compile options for the target
#f (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#    #set(CMAKE_VERBOSE_MAKEFILE 1)
#    set_target_properties("${wavelet_library_target_name}" PROPERTIES COMPILE_DEFINITIONS "Bits64_;UNIX;_BOOL;LINUX;FUNCPROTO;_GNU_SOURCE;LINUX_64;REQUIRE_IOSTREAM")
#endif()
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang") # Using GNU or Clang compiler
    # -fPIC: is needed for position-indipendent code to overcome the following error while linking:
    # "relocation R_X86_64_32S against symbol `stdout@@GLIBC_2.2.5' can not be used when making a shared object; recompile with -fPIC"
    target_compile_options("${wavelet_library_target_name}" PRIVATE "-fPIC")

    # -g (Debug Mode): Generate debug information. [https://clang.llvm.org/docs/CommandGuide/clang.html]
    # -O0 (Debug Mode): no optimization [https://clang.llvm.org/docs/CommandGuide/clang.html]
    target_compile_options("${wavelet_library_target_name}" PRIVATE "$<$<CONFIG:Debug>:-g;-O0>")
   
    # -O3 (Release Mode): enables most optimizations [https://clang.llvm.org/docs/CommandGuide/clang.html]
    target_compile_options("${wavelet_library_target_name}" PRIVATE "$<$<CONFIG:Release>:-O3>")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") # Using Visual Studio C++
   # /Gy: Allows the compiler to package individual functions in the form of packaged functions (COMDATs). [Microsoft Docs]
   #      The main advantage of this is that if you have identical functions the linker can collapse them all down into one actual piece of code ("COMDAT folding"). [StackOverflow]
   # /nologo: Suppresses the display of the copyright banner when the compiler starts up and display of informational messages during compiling. [Microsoft Docs]
   # /permissive-: Specify standards conformance mode to the compiler. Use this option to help you identify and fix conformance issues in your code, to make it both more correct and more portable. [Microsoft Docs]
   target_compile_options("${wavelet_library_target_name}" PRIVATE "/MP;/Gy;/nologo;/EHsc")

   # /Od (Debug Mode): Turns off all optimizations in the program and speeds compilation. [Microsoft Docs] 
   target_compile_options("${wavelet_library_target_name}" PRIVATE "$<$<CONFIG:Debug>:/Od>")
   
   # /O2 (Release Mode): Optimization option to maximize speed. [Microsoft Docs] 
   target_compile_options("${wavelet_library_target_name}" PRIVATE "$<$<CONFIG:Release>:/O2>")
endif()

# Displays the chosen target's compile options
get_target_property(WAVEPP_TARGET_COMPILE_OPTIONS "${wavelet_library_target_name}" COMPILE_OPTIONS)
message(STATUS "Compiler definitions added to target ${wavelet_library_target_name} for compiler ${CMAKE_CXX_COMPILER_ID} and build type ${CMAKE_BUILD_TYPE}: ${WAVEPP_TARGET_COMPILE_OPTIONS}")

target_compile_definitions("${wavelet_library_target_name}" PUBLIC _USE_MATH_DEFINES)
install(TARGETS "${wavelet_library_target_name}" DESTINATION lib)
install(FILES ${wavelet_library_headers} DESTINATION include)