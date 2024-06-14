# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/source/repos/Format/out/build/x64-debug/_deps/std-src"
  "D:/source/repos/Format/out/build/x64-debug/_deps/std-build"
  "D:/source/repos/Format/out/build/x64-debug/_deps/std-subbuild/std-populate-prefix"
  "D:/source/repos/Format/out/build/x64-debug/_deps/std-subbuild/std-populate-prefix/tmp"
  "D:/source/repos/Format/out/build/x64-debug/_deps/std-subbuild/std-populate-prefix/src/std-populate-stamp"
  "D:/source/repos/Format/out/build/x64-debug/_deps/std-subbuild/std-populate-prefix/src"
  "D:/source/repos/Format/out/build/x64-debug/_deps/std-subbuild/std-populate-prefix/src/std-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/source/repos/Format/out/build/x64-debug/_deps/std-subbuild/std-populate-prefix/src/std-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/source/repos/Format/out/build/x64-debug/_deps/std-subbuild/std-populate-prefix/src/std-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
