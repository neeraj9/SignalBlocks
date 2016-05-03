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


# - Try to find libsndfile library and headers
# Once done, this will define
#
#  LIBSNDFILE_FOUND - system has libsndfile
#  LIBSNDFILE_INCLUDE_DIR - the libsndfile include directories
#  LIBSNDFILE_LIBRARIES - link these to use libsndfile


if (LIBSNDFILE_INCLUDE_DIR AND LIBSNDFILE_LIBRARIES)
    set(LIBSNDFILE_FIND_QUIETLY TRUE)
endif (LIBSNDFILE_INCLUDE_DIR AND LIBSNDFILE_LIBRARIES)

# Include dir
find_path(LIBSNDFILE_INCLUDE_DIR
        NAMES sndfile.h
        )

# Library
find_library(LIBSNDFILE_LIBRARY
        NAMES sndfile
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBSNDFILE DEFAULT_MSG LIBSNDFILE_LIBRARY LIBSNDFILE_INCLUDE_DIR)

# If we successfully found the sndfile library then add the library to the
# LIBSNDFILE_LIBRARIES cmake variable otherwise set LIBSNDFILE_LIBRARIES to nothing.
if(LIBSNDFILE_FOUND)
    set(LIBSNDFILE_LIBRARIES ${LIBSNDFILE_LIBRARY})
else(LIBSNDFILE_FOUND)
    set(LIBSNDFILE_LIBRARIES)
endif(LIBSNDFILE_FOUND)


# Lastly make it so that the LIBSNDFILE_LIBRARY and LIBSNDFILE_INCLUDE_DIR variables
# only show up under the advanced options in the cmake applications.
MARK_AS_ADVANCED(LIBSNDFILE_LIBRARY LIBSNDFILE_INCLUDE_DIR)