// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_UDPLITESOCKET_H
#define SIGBLOCKS_UDPLITESOCKET_H

#include "ISocket.h"

#include <stdint.h>
#include <string>

namespace SocketProgramming {
    class UdpLiteSocket
            : public ISocket {
    public:
        // A value of 0 indicates UDP functionality (ie. cover complete packet)
        UdpLiteSocket(const std::string& destIp, int destPort,
                      int sendCoverageLength = 8, int recvCoverageLength = 8);

        virtual ~UdpLiteSocket();

        void SetDestination(const std::string& destIp, int destPort);

    public: // ISocket interface
        bool Bind(int port);

        bool Connect(); // dummy in case of udp
        bool Listen(int backlog); // dummy in case of udp
        int Receive(uint8_t* pBuff, int length);

        int Send(const uint8_t* pBuff, int length);

        bool IsValid() const;

    private:
        int mLocalPort; // in network byte order
        int mSockfd;
        std::string mDestIp;
        int mDestPort; // in network byte order
        int mSendCoverage;
        int mRecvCoverage;
    };
}

#endif // SIGBLOCKS_UDPLITESOCKET_H
