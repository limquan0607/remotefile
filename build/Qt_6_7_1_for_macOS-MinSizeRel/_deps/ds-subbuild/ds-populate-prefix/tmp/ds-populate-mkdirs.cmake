# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/quanvu/test2/build/Qt_6_7_1_for_macOS-MinSizeRel/_deps/ds-src"
  "/Users/quanvu/test2/build/Qt_6_7_1_for_macOS-MinSizeRel/_deps/ds-build"
  "/Users/quanvu/test2/build/Qt_6_7_1_for_macOS-MinSizeRel/_deps/ds-subbuild/ds-populate-prefix"
  "/Users/quanvu/test2/build/Qt_6_7_1_for_macOS-MinSizeRel/_deps/ds-subbuild/ds-populate-prefix/tmp"
  "/Users/quanvu/test2/build/Qt_6_7_1_for_macOS-MinSizeRel/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp"
  "/Users/quanvu/test2/build/Qt_6_7_1_for_macOS-MinSizeRel/_deps/ds-subbuild/ds-populate-prefix/src"
  "/Users/quanvu/test2/build/Qt_6_7_1_for_macOS-MinSizeRel/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/quanvu/test2/build/Qt_6_7_1_for_macOS-MinSizeRel/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/quanvu/test2/build/Qt_6_7_1_for_macOS-MinSizeRel/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()