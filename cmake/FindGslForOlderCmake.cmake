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

# - Find GSL
# Find the native GSL includes and library
#
#  GSL_INCLUDES    - where to find gsl/gsl_*.h, etc.
#  GSL_LIBRARIES   - List of libraries when using GSL.
#  GSL_FOUND       - True if GSL found.


if (GSL_INCLUDES)
  # Already in cache, be silent
  set (GSL_FIND_QUIETLY TRUE)
endif (GSL_INCLUDES)

find_path (GSL_INCLUDES gsl/gsl_math.h)

find_library (GSL_LIB NAMES gsl)

set (GSL_LIBRARIES "${GSL_LIB}" "${GSL_CBLAS_LIB}")

# handle the QUIETLY and REQUIRED arguments and set GSL_FOUND to TRUE if
# all listed variables are TRUE
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (GSL DEFAULT_MSG GSL_LIBRARIES GSL_INCLUDES)

mark_as_advanced (GSL_LIB GSL_CBLAS_LIB GSL_INCLUDES)
