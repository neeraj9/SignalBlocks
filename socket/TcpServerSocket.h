// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// This file is part of SignalBlocks.
//
// SignalBlocks is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SignalBlocks is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SignalBlocks.  If not, see <http://www.gnu.org/licenses/>.
//

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
