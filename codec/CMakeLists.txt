# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

set(CODEC_SOURCES
        BitOperation.hh
        BitReadInfo.hh
        LsbitToMsbitOperation.cc
        LsbitToMsbitOperation.hh
        MsbitToLsbitOperation.cc
        MsbitToLsbitOperation.hh)

add_library(codec STATIC ${CODEC_SOURCES})

target_include_directories(codec PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})