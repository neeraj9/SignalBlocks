# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

cmake_minimum_required(VERSION 3.3)
project(SignalBlocks)

# ##################################################################################
# Misc Settings
# ##################################################################################

# Uncomment to view detailed build commands for debugging build issues.
set(CMAKE_VERBOSE_MAKEFILE on)

# inform cmake to search for non-standard module (ex. FindICU)
# in project root path
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

# Build with gnu c++11, which when disabled uses llvm+clang
# when the gnu c++11 is disabled then llvm+clang+libstdc++ is used unless
# USE_LLVM_WITH_LIBCXX_LIBCXXABI is set (as shown below).
# Note that setting USE_LLVM_WITH_LIBCXX_LIBCXXABI has no effect on
# gnu c++11 build.
#set(USE_LLVM_WITH_LIBCXX_LIBCXXABI on)
set(USE_GNU_CXX11_BUILD ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# PYTHON_INCLUDE_DIR=/usr/include/python2.7
# PYTHON_LIBRARY=/usr/lib/python2.7/config/libpython2.7.so
set(PYTHON_INCLUDE_DIR "/usr/include/python2.7")
#set(PYTHON_LIBRARY, "/usr/lib64/libpython2.7.so")

# ##################################################################################
# Project Version
# ##################################################################################

set(VERSION_MAJOR   0   CACHE STRING "0")
set(VERSION_MINOR   1   CACHE STRING "1")
set(VERSION_PATCH   0   CACHE STRING "0")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# should be at top so that subdirectories can take advantage of this
include(compiler.cmake)

# ##################################################################################
# Find third party libraries
# ##################################################################################

# any third-party libraries are to be found here
find_package(Threads)
find_package(SCTP)
find_package(GSL)
find_package(PythonLibs)


# required to search for Python.h
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${PYTHON_INCLUDE_DIRS}")

# ##################################################################################
# Subdirectories
# ##################################################################################

#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(blocks)
add_subdirectory(plugins)
add_subdirectory(socket)
add_subdirectory(codec)
add_subdirectory(common)

# ##################################################################################
# Applications
# ##################################################################################

add_subdirectory(src)

# ##################################################################################
# Inclusions
# Always at the end to take advantage of prior variables in this file.
# Although target dependencies are is further down.
# ##################################################################################

include(doxygen.cmake)