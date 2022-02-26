# This file was added later to the library 
# and is not part of the original library.
# The same license as the original library is applied to this file.
cmake_minimum_required(VERSION 3.15)
project(wave++)

# add headers to the library
FILE(GLOB_RECURSE library_headers "source/*.h")

# target_include_directories(wave++ PUBLIC ${library_headers})

# message(STATUS "wave++ library headers added:")
# foreach(header ${library_headers})
#    message(STATUS "'${header}'")
# endforeach()

# add sources (and headers) to the library
FILE(GLOB_RECURSE library_sources "source/*.cc")

add_library(wave++ STATIC ${library_sources} ${library_headers})

message(STATUS "wave++ library sources added:")
foreach(source ${library_sources})
   message(STATUS "'${source}'")
endforeach()

install(TARGETS wave++ DESTINATION lib)
install(FILES ${library_headers} DESTINATION include)