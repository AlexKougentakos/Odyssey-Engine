# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Programming/VisualStudio/Odyssey-CMake Git/build_web/_deps/imgui-src"
  "D:/Programming/VisualStudio/Odyssey-CMake Git/build_web/_deps/imgui-build"
  "D:/Programming/VisualStudio/Odyssey-CMake Git/build_web/_deps/imgui-subbuild/imgui-populate-prefix"
  "D:/Programming/VisualStudio/Odyssey-CMake Git/build_web/_deps/imgui-subbuild/imgui-populate-prefix/tmp"
  "D:/Programming/VisualStudio/Odyssey-CMake Git/build_web/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp"
  "D:/Programming/VisualStudio/Odyssey-CMake Git/build_web/_deps/imgui-subbuild/imgui-populate-prefix/src"
  "D:/Programming/VisualStudio/Odyssey-CMake Git/build_web/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Programming/VisualStudio/Odyssey-CMake Git/build_web/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Programming/VisualStudio/Odyssey-CMake Git/build_web/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
