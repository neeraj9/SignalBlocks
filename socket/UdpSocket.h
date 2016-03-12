// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef UdpSocket_hh
#define UdpSocket_hh

#include "ISocket.h"

#include <stdint.h>
#include <string>

namespace SocketProgramming {
    class UdpSocket
            : public ISocket {
    public:
        UdpSocket(const std::string& destIp, int destPort);

        virtual ~UdpSocket();

        void SetDestination(const std::string& destIp, int destPort);

    public: // ISocket interface
        bool Bind(int port);

        bool Connect(); // dummy in case of udp
        bool Listen(int backlog); // dummy in case of udp

        /// non-blocking read
        int Receive(uint8_t* pBuff, int length);

        /// non-blocking write
        int Send(const uint8_t* pBuff, int length);

        /// Is the connection still open? Its true always for a udp socket.
        bool IsValid() const;

    protected:
        int mLocalPort; // in network byte order
        int mSockfd;
        std::string mDestIp;
        int mDestPort; // in network byte order
    };
}

#endif // UdpSocket_hh
