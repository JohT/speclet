# CPM Package Lock
# This file should be committed to version control

# JUCE
CPMDeclarePackage(JUCE
  GIT_TAG 8.0.9
  GITHUB_REPOSITORY juce-framework/JUCE
  EXCLUDE_FROM_ALL YES
)
# fftw (download links need to be updated manually on version updates)
CPMDeclarePackage(fftw
  URL https://www.fftw.org/fftw-3.3.10.tar.gz
  URL_HASH MD5=8ccbf6a5ea78a16dbc3e1306e234cc5c
  EXCLUDE_FROM_ALL YES
  PATCHES "fftw-update-min-cmake.patch"
)
# span (unversioned)(commits need to be updated manually on version updates)
CPMDeclarePackage(span
 GIT_TAG 836dc6a0efd9849cb194e88e4aa2387436bb079b
 GITHUB_REPOSITORY tcbrindle/span
 EXCLUDE_FROM_ALL YES
)

# Catch2
CPMDeclarePackage(Catch2
  VERSION 3.10.0
  GITHUB_REPOSITORY catchorg/Catch2
  EXCLUDE_FROM_ALL YES
)
