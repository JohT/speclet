# Test Build Configuration

# Sets the test project name 
# Uses the main project name defined in the main CMakeLists.txt file
set(TEST_PROJECT_NAME "${PROJECT_NAME}Tests")

# Import test dependencies
# Needs CPM which comes from "Environment.cmake" in the root directory of this repository
CPMAddPackage("gh:catchorg/Catch2@3.0.0-preview4")

# Enable testing with Catch2
enable_testing()
add_test("${TEST_PROJECT_NAME}" "${TEST_PROJECT_NAME}")

# find test sources
# GLOB_RECURSE is not recommended but used here for simplicity: https://cmake.org/cmake/help/latest/command/file.html?highlight=CONFIGURE_DEPENDS#filesystem
FILE(GLOB_RECURSE test_sources CONFIGURE_DEPENDS "*.h" "*Test.cpp")

# log found sources when log message level is verbose (use set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE) in CMakeLists.txt)
message(VERBOSE "${TEST_PROJECT_NAME} sources found:")
foreach(source ${test_sources})
   message(VERBOSE "'${source}'")
endforeach()

add_executable("${TEST_PROJECT_NAME}" ${test_sources})

# use the same C++ standard
get_target_property (MAIN_TARGET_CXX_STANDARD ${PROJECT_NAME} CXX_STANDARD)
if (NOT MAIN_TARGET_CXX_STANDARD STREQUAL "MAIN_TARGET_CXX_STANDARD-NOTFOUND")
   set_target_properties ("${TEST_PROJECT_NAME}" PROPERTIES CXX_STANDARD ${MAIN_TARGET_CXX_STANDARD})
   message (VERBOSE "${TEST_PROJECT_NAME}: Language Standard set to ${MAIN_TARGET_CXX_STANDARD}")
endif()

# Use the same include directories as the main target
get_target_property (MAIN_TARGET_INCLUDE_DIRECTORIES ${PROJECT_NAME} INCLUDE_DIRECTORIES)
if (NOT MAIN_TARGET_INCLUDE_DIRECTORIES STREQUAL "MAIN_TARGET_INCLUDE_DIRECTORIES-NOTFOUND")
   target_include_directories ("${TEST_PROJECT_NAME}" PRIVATE ${MAIN_TARGET_INCLUDE_DIRECTORIES})
   message (VERBOSE "${TEST_PROJECT_NAME}: Using include directories from ${PROJECT_NAME}: ${MAIN_TARGET_INCLUDE_DIRECTORIES}")
endif()

# Use the same compile options as the main target
get_target_property (MAIN_TARGET_COMPILE_OPTIONS ${PROJECT_NAME} COMPILE_OPTIONS)
if (NOT MAIN_TARGET_COMPILE_OPTIONS STREQUAL "MAIN_TARGET_COMPILE_OPTIONS-NOTFOUND")
   target_compile_options ("${TEST_PROJECT_NAME}" PRIVATE ${MAIN_TARGET_COMPILE_OPTIONS})
   message (VERBOSE "${TEST_PROJECT_NAME}: Using compile options from ${PROJECT_NAME}: ${MAIN_TARGET_COMPILE_OPTIONS}")
endif()

# Use the same compile definitions as the main target
get_target_property (MAIN_TARGET_COMPILE_DEFINITIONS ${PROJECT_NAME} COMPILE_DEFINITIONS)
if (NOT MAIN_TARGET_COMPILE_DEFINITIONS STREQUAL MAIN_TARGET_COMPILE_DEFINITIONS-NOTFOUND)
   target_compile_definitions ("${TEST_PROJECT_NAME}" PRIVATE ${MAIN_TARGET_COMPILE_DEFINITIONS})
   message (VERBOSE "${TEST_PROJECT_NAME}: Using compiler definitions from ${PROJECT_NAME}: ${MAIN_TARGET_COMPILE_DEFINITIONS}")
endif()

# Use the same link options as the main target and add the verbose output option
set(TEST_PROJECT_LINK_OPTIONS "-v")
get_target_property (MAIN_TARGET_LINK_OPTIONS ${PROJECT_NAME} LINK_OPTIONS)
if (NOT MAIN_TARGET_LINK_OPTIONS STREQUAL MAIN_TARGET_LINK_OPTIONS-NOTFOUND)
   set(TEST_PROJECT_LINK_OPTIONS "${MAIN_TARGET_LINK_OPTIONS}LINKER:,{TEST_PROJECT_LINK_OPTIONS}")
endif()
target_link_options ("${TEST_PROJECT_NAME}" PRIVATE "LINKER:${TEST_PROJECT_LINK_OPTIONS}")
message (VERBOSE "${TEST_PROJECT_NAME}: Using link options including those of ${PROJECT_NAME}: ${TEST_PROJECT_LINK_OPTIONS}")

# Additionally use the same link libraries as the main target
get_target_property (MAIN_TARGET_LINK_LIBRARIES ${PROJECT_NAME} LINK_LIBRARIES)
if (NOT MAIN_TARGET_LINK_LIBRARIES STREQUAL "MAIN_TARGET_LINK_LIBRARIES-NOTFOUND")
   message (VERBOSE "${TEST_PROJECT_NAME}: Using link libraries from ${PROJECT_NAME}: ${MAIN_TARGET_LINK_LIBRARIES}")
elseif()
   message (FATAL_ERROR "${TEST_PROJECT_NAME}: Link Libraries of main target not found.")
endif()

# Link catch with build-in main function and the main project link libraries
target_link_libraries("${TEST_PROJECT_NAME}" PRIVATE Catch2::Catch2WithMain "${PROJECT_NAME}" ${MAIN_TARGET_LINK_LIBRARIES})

# Register tests to CMake's CTest using the integration provided by Catch2:
# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md
include(CTest)
include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
catch_discover_tests("${TEST_PROJECT_NAME}")