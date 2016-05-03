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

set(COMMON_SOURCES
        TimeTick.h
        ComplexType.h
        DataTypes.h
        ComplexType.cpp
        TimeTick.cpp
        Port.h
        GenericCopy.h
        DataTypes.cpp
        MixedPort.h
        IPort.h
        CsvParser.cpp
        CsvParser.h
        PortType.h
        BlockRunner.cpp
        BlockRunner.h
        MemInputStream.h
        MemInputStream.cpp
        SoundInfo.cpp
        SoundInfo.h sha1.cpp sha1.h base64.cpp base64.h BlockManager.cpp BlockManager.h BlockTypes.h CallbackTypes.h)

add_library(common STATIC ${COMMON_SOURCES})

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
