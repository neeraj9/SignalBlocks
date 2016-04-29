// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_SOCKETSINKCREATOR_H
#define SIGBLOCKS_SOCKETSINKCREATOR_H

#include "SocketSink.h"

#include "../../common/IPort.h"
#include "../../common/TimeTick.h"
#include "../../socket/ISocket.h"

#include <memory>
#include <string>

namespace sigblocks {
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

#endif // SIGBLOCKS_SOCKETSINKCREATOR_H
