// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_sink_SocketSinkCreator_hh
#define sigblocks_blocks_sink_SocketSinkCreator_hh

#include "SocketSink.hh"

#include "../../common/IPort.hh"
#include "../../common/TimeTick.hh"
#include "../../socket/ISocket.hh"

#include <memory>
#include <string>

namespace sigblocks {
    template<class X>
    class SocketSinkCreator {
    public:
        static SocketSink* Create(
                const std::string& destIp,
                int destPort) {
            SocketSink* socket_sink = new SocketSink();
            std::unique_ptr<SocketProgramming::ISocket> pIns(new X(destIp, destPort));
            socket_sink->SetStreamSink(pIns);
            return socket_sink;
        }

        static SocketSink* Create(
                const std::string& destIp,
                int destPort,
                int localPort) {
            SocketSink* socket_sink = new SocketSink();
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

#endif // sigblocks_blocks_sink_SocketSinkCreator_hh
