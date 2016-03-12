// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_TCPSERVERSOCKET_H
#define SIGBLOCKS_TCPSERVERSOCKET_H

#include "IServerSocket.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <stdint.h>
#include <string>

namespace SocketProgramming {
    /**
     * Open a non-blocking tcp server socket, which accepts
     * one connection at a time. This can later be enhanced
     * to accept more than one connection at a time, or rather
     * templatized to accept and read on N connections at
     * a time.
     */
    class TcpServerSocket
            : public IServerSocket {
    public:
        TcpServerSocket();

        virtual ~TcpServerSocket();

    public: // IServerSocket interface
        bool Bind(int port);

        bool Listen(int backlog);

        bool Accept(struct sockaddr* from,
                    socklen_t* fromlen);

        int Receive(uint8_t* pBuff, int length,
                    struct sockaddr* from,
                    socklen_t* fromlen);

        int Send(const uint8_t* pBuff, int length,
                 struct sockaddr* to,
                 socklen_t tolen);

        bool IsValid() const;

        bool IsConnected();

    private:
        int mLocalPort; // in network byte order
        int mListeningSockfd;
        int mDataSockfd;
        struct sockaddr_in mDestAddr;
    };
}

#endif // SIGBLOCKS_TCPSERVERSOCKET_H
