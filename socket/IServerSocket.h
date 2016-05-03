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

#ifndef SIGNALBLOCKS_ISERVERSOCKET_H
#define SIGNALBLOCKS_ISERVERSOCKET_H

#include <stdint.h>
#include <sys/socket.h>

namespace SocketProgramming {
    /**
     * Abstract base class for server socket. All the transport
     * layers (ex. UDP, TCP, UDPLite) will derive from
     * it and provide the server side of socket library.
     */
    class IServerSocket {
    public:
        virtual ~IServerSocket() {
        }

        virtual int GetFd() const = 0;

        /// Bind server to a localPort
        /// \return true when successful
        /// \return false when unable to bind
        virtual bool Bind(int localPort) = 0;

        /// Listen with a specified backlog
        virtual bool Listen(int backlog) = 0;

        /// Accept connection from a client.
        /// \return  false on no waiting client
        /// \return true when accepted a new connection.
        virtual bool Accept(struct sockaddr* from,
                            socklen_t* fromlen) = 0;

        /// Receive new data message on connected client.
        /// \return -1 on error (not connected, or no data) or
        ///         >0 on bytes received (return the num bytes read)
        virtual int Receive(uint8_t* pBuff,
                            int length,
                            struct sockaddr* from,
                            socklen_t* fromlen) = 0;

        virtual int Send(const uint8_t* pBuff,
                         int length,
                         struct sockaddr* to,
                         socklen_t tolen) = 0;

        virtual bool IsValid() const = 0;

        virtual bool IsConnected() = 0;
    };
}

#endif // SIGNALBLOCKS_ISERVERSOCKET_H
