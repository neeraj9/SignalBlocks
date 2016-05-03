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

#ifndef SIGNALBLOCKS_SOCKETSOURCECREATOR_H
#define SIGNALBLOCKS_SOCKETSOURCECREATOR_H

#include "SocketSource.h"

#include "../../common/IPort.h"
#include "../../common/TimeTick.h"
#include "../../socket/ISocket.h"

#include <memory>
#include <string>

namespace signalblocks {
    template<class X>
    class SocketSourceCreator {
    public:
        static SocketSource* Create(
                int blockSize,
                const std::string& destIp,
                int destPort,
                int localPort) {
            SocketSource* socket_source =
                    new SocketSource("socket-source", blockSize);
            std::unique_ptr<SocketProgramming::ISocket> pIns(new X(destIp, destPort));
            if (pIns->Bind(localPort)) {
                if (pIns->Listen(1)) // TODO: Note for TCP of connection oriented
                    // protocols this will stop to get one client
                    // connection and only then do the rest of the tasks.
                    // Think about Lazy initialization here (how about a new thread
                    // while listens)!
                {
                    // save only if successful
                    socket_source->SetStreamSource(std::move(pIns));
                }
                else {
                    // cerr TODO
                }
                return socket_source;
            }
            else {
                // cerr TODO
            }
            return socket_source;
        }
    };
}

#endif // SIGNALBLOCKS_SOCKETSOURCECREATOR_H
