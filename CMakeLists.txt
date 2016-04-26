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

# ##################################################################################
# Project Version
# ##################################################################################

set(VERSION_MAJOR   0   CACHE STRING "0")
set(VERSION_MINOR   1   CACHE STRING "1")
set(VERSION_PATCH   0   CACHE STRING "0")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# should be at top so that subdirectories can take advantage of this
include(compiler.cmake)

# ##############################################################################
# Find third party libraries
# ##############################################################################

# any third-party libraries are to be found here
find_package(Threads)
find_package(GSL)
find_package(LibSndFile)

# ####################
# Find Python
# ####################

find_package(PythonInterp)
find_package(PythonLibs)

# require at least python 2.7
if(PYTHON_VERSION_STRING VERSION_LESS 2.7)
  message(FATAL_ERROR " Python version less than 2.7: \"${PYTHON_VERSION_STRING}\".")
endif()

# check for version mismatch.
if(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND
    AND NOT(PYTHON_VERSION_STRING VERSION_EQUAL PYTHONLIBS_VERSION_STRING))
    message(FATAL_ERROR " Python executable (\"${PYTHON_VERSION_STRING}\") and library (\"${PYTHONLIBS_VERSION_STRING}\") version mismatch. Clean build area and try again.")
endif()

mark_as_advanced(PYTHON_EXECUTABLE)
include_directories(${PYTHON_INCLUDE_PATH})


# ##################################################################################
# External Projects
# ##################################################################################

#include(ExternalProject)

# ##################################################################################
# Subdirectories
# ##################################################################################

#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(common)
add_subdirectory(codec)
add_subdirectory(socket)
add_subdirectory(http)
add_subdirectory(plugins)
add_subdirectory(blocks)

# ##################################################################################
# Applications
# ##################################################################################

add_subdirectory(src)
add_subdirectory(examples/histogram)
add_subdirectory(examples/websocket_demo)

# tests
add_subdirectory(test)

# ##################################################################################
# Inclusions
# Always at the end to take advantage of prior variables in this file.
# Although target dependencies are is further down.
# ##################################################################################

include(doxygen.cmake)

