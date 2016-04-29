# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

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
        SoundInfo.h sha1.cpp sha1.h base64.cpp base64.h BlockManager.cpp BlockManager.h BlockTypes.h)

add_library(common STATIC ${COMMON_SOURCES})

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})