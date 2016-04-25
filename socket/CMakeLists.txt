# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

set(SOCKET_SOURCES
        EnhancedUdpSocket.cpp
        EnhancedUdpSocket.h
        IClientSocket.h
        IPSocket.cpp
        IPSocket.h
        IServerSocket.h
        ISocket.h
        RawIPSocket.cpp
        RawIPSocket.h
        SctpSocket.cpp
        SctpSocket.h
        TcpServerSocket.cpp
        TcpServerSocket.h
        TcpSocket.cpp
        TcpSocket.h
        UdpLiteSocket.cpp
        UdpLiteSocket.h
        UdpSocket.cpp
        UdpSocket.h SocketUtils.cpp SocketUtils.h)

add_library(socket STATIC ${SOCKET_SOURCES})

target_include_directories(socket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})