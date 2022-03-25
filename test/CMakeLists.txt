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

# Compile test project with C++20 Standard
target_compile_features("${TEST_PROJECT_NAME}" PRIVATE cxx_std_20)
message(VERBOSE "C++ Language Standard set to C++20 for Target ${TEST_PROJECT_NAME}")

# Our test executable also wants to know about our plugin code...
target_include_directories("${TEST_PROJECT_NAME}" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries("${TEST_PROJECT_NAME}" PRIVATE Catch2::Catch2WithMain "${PROJECT_NAME}" ${MAIN_LINK_LIBRARIES})

# Load and use the .cmake file provided by Catch2
# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md
# We have to manually provide the source directory here for now
include(CTest)
include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
catch_discover_tests("${TEST_PROJECT_NAME}")

#set(CMAKE_MESSAGE_LOG_LEVEL DEBUG)
#add_test("${TEST_PROJECT_NAME}" "${TEST_PROJECT_NAME}")