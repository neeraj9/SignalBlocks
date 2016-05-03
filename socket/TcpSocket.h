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

#ifndef SIGNALBLOCKS_TCPSOCKET_H
#define SIGNALBLOCKS_TCPSOCKET_H

#include "ISocket.h"

#include <stdint.h>
#include <string>

namespace SocketProgramming {
    class TcpSocket
            : public ISocket {
    public:
        TcpSocket(const std::string& destIp, int destPort);

        virtual ~TcpSocket();

        void SetDestination(const std::string& destIp, int destPort);

    public: // ISocket interface
        bool Bind(int port);

        bool Connect();

        bool Listen(int backlog);

        int Receive(uint8_t* pBuff, int length);

        int Send(const uint8_t* pBuff, int length);

        bool IsValid() const;

    private:
        int mLocalPort; // in network byte order
        int mSockfd;
        std::string mDestIp;
        int mDestPort; // in network byte order
        bool mIsConnected;
    };
}

#endif // SIGNALBLOCKS_TCPSOCKET_H
