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

target_compile_definitions("${wavelet_library_target_name}" PUBLIC _USE_MATH_DEFINES)
install(TARGETS "${wavelet_library_target_name}" DESTINATION lib)
install(FILES ${wavelet_library_headers} DESTINATION include)