# Example Audio Plugin CMakeLists.txt

# To get started on a new plugin, copy this entire folder (containing this file and C++ sources) to
# a convenient location, and then start making modifications.

# The first line of any CMake project should be a call to `cmake_minimum_required`, which checks
# that the installed CMake will be able to understand the following CMakeLists, and ensures that
# CMake's behaviour is compatible with the named version. This is a standard CMake command, so more
# information can be found in the CMake docs.

cmake_minimum_required(VERSION 3.15)

# The top-level CMakeLists.txt file for a project must contain a literal, direct call to the
# `project()` command. `project()` sets up some helpful variables that describe source/binary
# directories, and the current project version. This is a standard CMake command.

# Name of the project and the plugin. Avoid spaces and special characters.
set(PROJECT_NAME "Speclet")
set(CURRENT_VERSION "1.0.0")

# Set message log level to VERBOSE to see more details about what CMake is doing.
#set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)

# For simplilicity, the name of the project is also the name of the targe.
project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})

include(Environment.cmake)

# Import dependencies
#CPMAddPackage("https://math.ryerson.ca/~lkolasa/xxx.tar.gz") # Not online anymore (2022). Now directly included in this repository.
CPMAddPackage("gh:juce-framework/JUCE#6.1.6")
CPMAddPackage("https://www.fftw.org/fftw-3.3.10.tar.gz#MD5=8ccbf6a5ea78a16dbc3e1306e234cc5c")
CPMAddPackage("gh:dmoulding/vld#v1.9h")
# CPMAddPackage("gh:Neargye/magic_enum#v0.7.3") # simplifies enum handling, not yet needed

# If you've installed JUCE somehow (via a package manager, or directly using the CMake install
# target), you'll need to tell this project that it depends on the installed copy of JUCE. If you've
# included JUCE directly in your source tree (perhaps as a submodule), you'll need to tell CMake to
# include that subdirectory as part of the build.

# find_package(JUCE CONFIG REQUIRED)        # If you've installed JUCE to your system
# or
# add_subdirectory(${JUCE_SOURCE_DIR})      # If you've put JUCE in a subdirectory called JUCE

# If you are building a VST2 or AAX plugin, CMake needs to be told where to find these SDKs on your
# system. This setup should be done before calling `juce_add_plugin`.

# juce_set_vst2_sdk_path(...)
# juce_set_aax_sdk_path(...)

# `juce_add_plugin` adds a static library target with the name passed as the first argument
# (Speclet here). This target is a normal CMake target, but has a lot of extra properties set
# up by default. As well as this shared code static library, this function adds targets for each of
# the formats specified by the FORMATS arguments. This function accepts many optional arguments.
# Check the readme at `docs/CMake API.md` in the JUCE repo for the full list.

juce_add_plugin("${PROJECT_NAME}"
    PRODUCT_NAME "${PROJECT_NAME}"              # The name of the final executable, which can differ from the target name
    COMPANY_NAME "Johannes Troppacher"          # Specify the name of the plugin's author
    # VERSION 0.9.0                             # Set this if the plugin version is different to the project version
    FORMATS VST3 AU AUv3 Standalone             # The formats to build. Other valid formats are: AAX Unity VST AU AUv3

    PLUGIN_MANUFACTURER_CODE JohT               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE SpcJ                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    IS_SYNTH FALSE                              # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE                      # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE                     # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                        # Is this plugin a MIDI effect?
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE           # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
)

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

juce_generate_juce_header("${PROJECT_NAME}")

# Compile Project with C++20 Standard
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_20)
message(VERBOSE "C++ Language Standard set to C++20 for Target ${PROJECT_NAME}")

# Get all source files in the src directory and store them in the project_sources variable.
# GLOB_RECURSE is not recommended but used here for simplicity: https://cmake.org/cmake/help/latest/command/file.html?highlight=CONFIGURE_DEPENDS#filesystem
FILE(GLOB_RECURSE project_sources CONFIGURE_DEPENDS "src/*.cpp")
message(VERBOSE "Sources in ${CMAKE_CURRENT_SOURCE_DIR}/src:")
foreach(source ${project_sources})
   message(VERBOSE "'${source}'")
endforeach()

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.
target_sources("${PROJECT_NAME}" PRIVATE ${project_sources})

# Import fftw3 api headers like fftw3.h:
if (fftw_ADDED)
    include_directories("${PROJECT_NAME}" PRIVATE "${fftw_SOURCE_DIR}/api")	 
    message(VERBOSE "FFTW api (headers) directory included for all targets: ${fftw_SOURCE_DIR}/api")
    # Get all include directories for the target.
    get_target_property(PROJECT_INCLUDES "${PROJECT_NAME}" INCLUDE_DIRECTORIES)
    foreach(dir ${PROJECT_INCLUDES})
        message(VERBOSE "Project_include_dir='${dir}'")
    endforeach()
endif()

# Add libraries to the target, that needed to be included inside the repository:
add_subdirectory(lib/wave++)

# `target_compile_definitions` adds some preprocessor definitions to our target. In a Projucer
# project, these might be passed in the 'Preprocessor Definitions' field. JUCE modules also make use
# of compile definitions to switch certain features on/off, so if there's a particular feature you
# need that's not on by default, check the module header for the correct flag to set here. These
# definitions will be visible both to your code, and also the JUCE module code, so for new
# definitions, pick unique names that are unlikely to collide! This is a standard CMake command.
target_compile_definitions("${PROJECT_NAME}"
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_DISPLAY_SPLASH_SCREEN=0 # Disable the splash screen on startup under GPL license
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
        LOG_PERFORMANCE=1
        )

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

# juce_add_binary_data(AudioPluginData SOURCES ...)

# We'll need to link to these from our plugin as well as our tests
set(MAIN_LINK_LIBRARIES
    juce::juce_core
    juce::juce_dsp
    juce::juce_audio_utils
    juce::juce_gui_basics
    fftw3
    wave++
)

# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_audio_utils` module. Inter-module
# dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        ${MAIN_LINK_LIBRARIES}
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# Add tests
add_subdirectory(test)