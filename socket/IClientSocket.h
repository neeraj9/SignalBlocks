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

#ifndef SIGNALBLOCKS_ICLIENTSOCKET_H
#define SIGNALBLOCKS_ICLIENTSOCKET_H

#include <stdint.h>
#include <sys/socket.h>

namespace SocketProgramming {
    class IClientSocket {
    public:
        virtual ~IClientSocket() {
        }

        virtual bool Bind(int localPort) = 0;

        virtual bool Connect(struct sockaddr* to, socklen_t tolen) = 0;

        virtual int Receive(uint8_t* pBuff,
                            int length,
                            struct sockaddr* from,
                            socklen_t* fromlen) = 0;

        virtual int Send(const uint8_t* pBuff,
                         int length) = 0;

        virtual int Send(const uint8_t* pBuff,
                         int length,
                         struct sockaddr* to,
                         socklen_t tolen) = 0;

        virtual bool IsValid() = 0;
    };
}

#endif // SIGNALBLOCKS_ICLIENTSOCKET_H
