#!/bin/sh
#
# Setup environment for g++ version 8 for complete C++17 support
#
# see https://gcc.gnu.org/projects/cxx-status.html#cxx17
# see https://stackoverflow.com/questions/17275348/how-to-specify-new-gcc-path-for-cmake
#

export CC=/usr/bin/gcc-8
export CXX=/usr/bin/g++-8

cmake $@
