# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

set(HTTP_SOURCES
        HttpTcpWebsocketServer.cpp
        HttpTcpWebsocketServer.h)

add_library(http STATIC ${HTTP_SOURCES})

target_include_directories(http PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})