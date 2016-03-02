# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

set(COMMON_SOURCES
        TimeTick.hh
        ComplexType.hh
        DataTypes.hh
        Port.cc
        ComplexType.cc
        TimeTick.cc
        Port.hh
        MixedPort.cc
        GenericCopy.hh
        DataTypes.cc
        MultiPtr.hh
        MixedPort.hh
        IPort.hh)

add_library(common STATIC ${COMMON_SOURCES})

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})