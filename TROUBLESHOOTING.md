# Troubleshooting Guide

Common issues and their resolutions when building the project. If you encounter a problem not listed here, please consider contributing a solution.

## C++ standard headers not found on macOS

### Symptom

- CMake/clang failed with "fatal error: 'algorithm' file not found".

### Cause

- The Command Line Tools include layout was inconsistent: 
  `/Library/Developer/CommandLineTools/usr/include/c++/v1` contained remnants
  while the SDK symlink pointed to a newer SDK, so the compiler couldn't find
  the complete C++ headers.

### Resolution (what I did)

1. Updated `cmake/Environment.cmake` to set `CMAKE_OSX_DEPLOYMENT_TARGET` to 11.0.
2. Quick fix (applied): symlinked SDK headers into the expected location:

   sudo rm -rf /Library/Developer/CommandLineTools/usr/include/c++
   sudo ln -s /Library/Developer/CommandLineTools/SDKs/MacOSX15.4.sdk/usr/include/c++ /Library/Developer/CommandLineTools/usr/include/c++

3. Reconfigured and rebuilt:

   rm -rf build
   cmake -Bbuild -DJUCE_BUILD_EXTRAS=ON -DJUCE_BUILD_EXAMPLES=OFF -DCMAKE_BUILD_TYPE:STRING=Debug -G Ninja
   cmake --build build

### Recommended proper fix

- Reinstall Command Line Tools:

   sudo rm -rf /Library/Developer/CommandLineTools
   xcode-select --install

### Notes

- If you have full Xcode installed, switch developer directory and accept the license instead.
