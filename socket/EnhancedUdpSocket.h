// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_ENHANCEDUDPSOCKET_H
#define SIGBLOCKS_ENHANCEDUDPSOCKET_H

#include "UdpSocket.h"

#include <netinet/in.h>
#include <stdint.h>
#include <string>

namespace SocketProgramming {
    class EnhancedUdpSocket
            : public UdpSocket {
    public:
        EnhancedUdpSocket(const std::string& destIp, int destPort);

    public: // ISocket interface
        /// non-blocking read
        int Receive(uint8_t* pBuff, int length, sockaddr_in* pSrcAddr, sockaddr_in* pDestAddr);

        int Send(const uint8_t* pBuff, int length, const std::string& destIp, int destPort);
    };
}

#endif // SIGBLOCKS_ENHANCEDUDPSOCKET_H
