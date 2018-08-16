# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# This file is part of SignalBlocks.
#
# SignalBlocks is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SignalBlocks is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with SignalBlocks.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.8)
project(SignalBlocks)

# ##################################################################################
# Misc Settings
# ##################################################################################

# Uncomment to view detailed build commands for debugging build issues.
set(CMAKE_VERBOSE_MAKEFILE on)

option(COVERALLS "Turn on coveralls support" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)

# inform cmake to search for non-standard module (ex. FindICU)
# in project root path
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Build with gnu c++17, which when disabled uses llvm+clang
# when the gnu c++17 is disabled then llvm+clang+libstdc++ is used unless
# USE_LLVM_WITH_LIBCXX_LIBCXXABI is set (as shown below).
# Note that setting USE_LLVM_WITH_LIBCXX_LIBCXXABI has no effect on
# gnu c++17 build.
#set(USE_LLVM_WITH_LIBCXX_LIBCXXABI on)
set(USE_GNU_CXX17_BUILD ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if(NOT CMAKE_COMPILER_IS_GNUCXX)
  message("Since the compiler is non GNU CXX so disable coveralls upload")
  set(COVERALLS_UPLOAD OFF)
endif()

if (COVERALLS)
	include(Coveralls)
	coveralls_turn_on_coverage()
endif()

# include before adding the test subfolder where tests
# are added. Unless you do that ctest will not be integrated correctly.
# Doing the include is enough and you no longer need to do
# enable_testing() here.
#include(CTest)
enable_testing()

# ##################################################################################
# Project Version
# ##################################################################################

set(VERSION_MAJOR   0   CACHE STRING "0")
set(VERSION_MINOR   1   CACHE STRING "1")
set(VERSION_PATCH   0   CACHE STRING "0")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# should be at top so that subdirectories can take advantage of this
include(compiler)

# ##############################################################################
# Find third party libraries
# ##############################################################################

# enable cmake scripts to use pkg_check_modules
include(FindPkgConfig)

# any third-party libraries are to be found here
find_package(Threads)

if(CMAKE_MAJOR_VERSION VERSION_LESS 3.0)
  find_package(GslForOlderCmake)
else()
  find_package(GSL)
endif()

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

if (NOT DISABLE_PYTHON_BIN_AND_LIBCHECK)
    # check for version mismatch.
    if(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND
        AND NOT(PYTHON_VERSION_STRING VERSION_EQUAL PYTHONLIBS_VERSION_STRING))
        message(FATAL_ERROR " Python executable (\"${PYTHON_VERSION_STRING}\") and library (\"${PYTHONLIBS_VERSION_STRING}\") version mismatch. Clean build area and try again.")
    endif()
endif(NOT DISABLE_PYTHON_BIN_AND_LIBCHECK)

mark_as_advanced(PYTHON_EXECUTABLE)
include_directories(${PYTHON_INCLUDE_PATH})

# Find RTLSDR
find_package(LibRTLSDR)

# ##################################################################################
# For Test Coverage
# ##################################################################################

find_program( GCOV_PATH gcov )

if(NOT GCOV_PATH)
	message(FATAL_ERROR "gcov not found! Aborting...")
endif() # not GCOV_PATH

if(NOT CMAKE_COMPILER_IS_GNUCXX)
	# Clang version 3.0.0 and greater now supports gcov as well.
	message(WARNING "Compiler is not GNU gcc! Clang Version 3.0.0 and greater supports gcov as well, but older versions don't.")

	if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		message(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
	endif()
endif() # NOT CMAKE_COMPILER_IS_GNUCXX


if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Coverage")
  # for debug build only
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage" )
endif() # NOT CMAKE_BUILD_TYPE STREQUAL "Debug"


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

if (COVERALLS)
  # without absolute path older cmake (prior to 3.3 experience some issue)
  # so have relative path instead
  FILE(GLOB SOURCE_FILES RELATIVE ${PROJECT_SOURCE_DIR} */*.cpp */*.h */*/*.cpp */*/*.h)
  foreach(item ${SOURCE_FILES})
    if(${item} MATCHES "test/.*")
      list(REMOVE_ITEM SOURCE_FILES ${item})
    endif()
  endforeach()
  message("Coverage source files: ${SOURCE_FILES}")
  coveralls_setup(
        "${SOURCE_FILES}"
        ${COVERALLS_UPLOAD}
        "${PROJECT_SOURCE_DIR}/cmake")
endif()


# ##################################################################################
# Inclusions
# Always at the end to take advantage of prior variables in this file.
# Although target dependencies are is further down.
# ##################################################################################

include(doxygen)

