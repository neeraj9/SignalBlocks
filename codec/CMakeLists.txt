# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

set(CODEC_SOURCES
        BitOperation.h
        BitReadInfo.h
        LsbitToMsbitOperation.cpp
        LsbitToMsbitOperation.h
        MsbitToLsbitOperation.cpp
        MsbitToLsbitOperation.h)

add_library(codec STATIC ${CODEC_SOURCES})

target_include_directories(codec PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})