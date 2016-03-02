# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

set(SOCKET_SOURCES
        EnhancedUdpSocket.cc
        EnhancedUdpSocket.hh
        IClientSocket.hh
        IPSocket.cc
        IPSocket.hh
        IServerSocket.hh
        ISocket.hh
        RawIPSocket.cc
        RawIPSocket.hh
        SctpSocket.cc
        SctpSocket.hh
        TcpServerSocket.cc
        TcpServerSocket.hh
        TcpSocket.cc
        TcpSocket.hh
        UdpLiteSocket.cc
        UdpLiteSocket.hh
        UdpSocket.cc
        UdpSocket.hh)

add_library(socket STATIC ${SOCKET_SOURCES})

target_include_directories(socket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})