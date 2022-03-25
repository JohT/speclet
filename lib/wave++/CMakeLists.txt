# This file was added later to the library 
# and is not part of the original library.
# The same license as the original library is applied to this file.
cmake_minimum_required(VERSION 3.15)
project(wave++)

# add headers to the library
# GLOB_RECURSE is not recommended but used here for simplicity: https://cmake.org/cmake/help/latest/command/file.html?highlight=CONFIGURE_DEPENDS#filesystem
FILE(GLOB_RECURSE library_headers CONFIGURE_DEPENDS "source/*.h")

message(VERBOSE "wave++ library headers added:")
foreach(header ${library_headers})
   message(VERBOSE "'${header}'")
endforeach()

# add sources (and headers) to the library
# GLOB_RECURSE is not recommended but used here for simplicity: https://cmake.org/cmake/help/latest/command/file.html?highlight=CONFIGURE_DEPENDS#filesystem
FILE(GLOB_RECURSE library_sources CONFIGURE_DEPENDS "source/*.cc")

add_library(wave++ STATIC ${library_sources} ${library_headers})

message(VERBOSE "wave++ library sources added:")
foreach(source ${library_sources})
   message(VERBOSE "'${source}'")
endforeach()

install(TARGETS wave++ DESTINATION lib)
install(FILES ${library_headers} DESTINATION include)