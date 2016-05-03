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

# ##################################################################################
# Common Source Files
# ##################################################################################

set(SOURCE_FILES
        )

# ##################################################################################
# main sample application
# ##################################################################################

add_executable(websocket_demo websocket_demo.cpp ${SOURCE_FILES})

target_link_libraries(websocket_demo blocks)
target_link_libraries(websocket_demo plugins)
target_link_libraries(websocket_demo http)
target_link_libraries(websocket_demo socket)
target_link_libraries(websocket_demo codec)
target_link_libraries(websocket_demo common)

target_link_libraries(websocket_demo ${LIBSNDFILE_LIBRARIES} ${PYTHON_LIBRARIES} ${GSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# define more applications if exists
