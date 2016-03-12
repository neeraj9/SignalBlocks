# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

set(COMMON_SOURCES
        TimeTick.h
        ComplexType.h
        DataTypes.h
        Port.cpp
        ComplexType.cpp
        TimeTick.cpp
        Port.h
        MixedPort.cpp
        GenericCopy.h
        DataTypes.cpp
        MixedPort.h
        IPort.h)

add_library(common STATIC ${COMMON_SOURCES})

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})