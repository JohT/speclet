# CPM Package Lock
# This file should be committed to version control

# JUCE
CPMDeclarePackage(JUCE
  GIT_TAG 7.0.0
  GITHUB_REPOSITORY juce-framework/JUCE
  EXCLUDE_FROM_ALL YES
)
# fftw (download links need to be updated manually on version updates)
CPMDeclarePackage(fftw
  URL https://www.fftw.org/fftw-3.3.10.tar.gz URL_HASH MD5=8ccbf6a5ea78a16dbc3e1306e234cc5c EXCLUDE_FROM_ALL YES
)
# span (unversioned)(commits need to be updated manually on version updates)
CPMDeclarePackage(span
 GIT_TAG 117fbada0f888e1535e3db20c7c9ddd86db129e2
 GITHUB_REPOSITORY tcbrindle/span
 EXCLUDE_FROM_ALL YES
)

# Catch2
CPMDeclarePackage(Catch2
  VERSION 3.0.1
  GITHUB_REPOSITORY catchorg/Catch2
  EXCLUDE_FROM_ALL YES
)
