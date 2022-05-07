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
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Using GNU or Clang compiler
    set(GNUCLANG_COMPILE_OPTIONS "-m64;-fPIC")
    set(GNUCLANG_COMPILE_DEBUG_OPTIONS "${GNUCLANG_COMPILE_OPTIONS};-g;-O0")
    set(GNUCLANG_COMPILE_RELEASE_OPTIONS "${GNUCLANG_COMPILE_OPTIONS};-O3")
    target_compile_options("${wavelet_library_target_name}" PRIVATE "$<$<CONFIG:Debug>:${GNUCLANG_COMPILE_DEBUG_OPTIONS}>")
    target_compile_options("${wavelet_library_target_name}" PRIVATE "$<$<CONFIG:Release>:${GNUCLANG_COMPILE_RELEASE_OPTIONS}>")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Using Visual Studio C++
    set(MSVC_COMPILE_OPTIONS "/MP;/W3;/w34710;/Gy;/Zc:wchar_t;/nologo;/std:c++0x /EHsc")
    set(MSVC_COMPILE_DEBUG_OPTIONS "${MSVC_COMPILE_OPTIONS} /ZI /Od")
    set(MSVC_COMPILE_RELEASE_OPTIONS "${MSVC_COMPILE_OPTIONS} /O2")
    target_compile_options("${wavelet_library_target_name}" PRIVATE "$<$<CONFIG:Debug>:${MSVC_COMPILE_DEBUG_OPTIONS}>")
    target_compile_options("${wavelet_library_target_name}" PRIVATE "$<$<CONFIG:Release>:${MSVC_COMPILE_RELEASE_OPTIONS}>")
endif()

# Displays the chosen target's compile options
get_target_property(WAVEPP_TARGET_COMPILE_OPTIONS "${wavelet_library_target_name}" COMPILE_OPTIONS)
message(STATUS "Compiler definitions added to target ${wavelet_library_target_name} for compiler ${CMAKE_CXX_COMPILER_ID} and build type ${CMAKE_BUILD_TYPE}: ${WAVEPP_TARGET_COMPILE_OPTIONS}")

target_compile_definitions("${wavelet_library_target_name}" PUBLIC _USE_MATH_DEFINES)
install(TARGETS "${wavelet_library_target_name}" DESTINATION lib)
install(FILES ${wavelet_library_headers} DESTINATION include)