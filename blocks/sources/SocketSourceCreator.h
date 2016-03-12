// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_source_SocketSourceCreator_hh
#define sigblocks_blocks_source_SocketSourceCreator_hh

#include "SocketSource.h"

#include "../../common/IPort.h"
#include "../../common/TimeTick.h"
#include "../../socket/ISocket.h"

#include <memory>
#include <string>

namespace sigblocks {
    template<class X>
    class SocketSourceCreator {
    public:
        static SocketSource* Create(
                int blockSize,
                const std::string& destIp,
                int destPort,
                int localPort) {
            SocketSource* socket_source =
                    new SocketSource(blockSize);
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

#endif // sigblocks_blocks_source_SocketSourceCreator_hh
