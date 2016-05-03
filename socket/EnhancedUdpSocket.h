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

#ifndef SIGNALBLOCKS_ENHANCEDUDPSOCKET_H
#define SIGNALBLOCKS_ENHANCEDUDPSOCKET_H

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

#endif // SIGNALBLOCKS_ENHANCEDUDPSOCKET_H
