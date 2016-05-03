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

#ifndef SIGNALBLOCKS_SOCKETSINKCREATOR_H
#define SIGNALBLOCKS_SOCKETSINKCREATOR_H

#include "SocketSink.h"

#include "../../common/IPort.h"
#include "../../common/TimeTick.h"
#include "../../socket/ISocket.h"

#include <memory>
#include <string>

namespace signalblocks {
    template<class X>
    class SocketSinkCreator {
    public:
        static SocketSink* Create(
                const std::string& destIp,
                int destPort) {
            SocketSink* socket_sink = new SocketSink("socket-sink");
            std::unique_ptr<SocketProgramming::ISocket> pIns(new X(destIp, destPort));
            socket_sink->SetStreamSink(std::move(pIns));
            return socket_sink;
        }

        static SocketSink* Create(
                const std::string& destIp,
                int destPort,
                int localPort) {
            SocketSink* socket_sink = new SocketSink("socket-sink");
            std::unique_ptr<SocketProgramming::ISocket> pIns(new X(destIp, destPort));
            if (pIns->Bind(localPort)) {
                if (pIns->Connect()) {
                    // save only if successful
                    socket_sink->SetStreamSink(std::move(pIns));
                }
            }
            else {
                // cerr TODO
            }
            return socket_sink;
        }
    };
}

#endif // SIGNALBLOCKS_SOCKETSINKCREATOR_H
