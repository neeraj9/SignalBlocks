// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_transceiver_SocketTransceiverCreator_hh
#define sigblocks_blocks_transceiver_SocketTransceiverCreator_hh

#include "SocketTransceiver.hh"

#include "../../common/IPort.hh"
#include "../../common/TimeTick.hh"
#include "../../socket/ISocket.hh"

#include <memory>
#include <string>

namespace sigblocks {
    template<class X>
    class SocketTransceiverCreator {
    public:
        static SocketTransceiver* Create(
                int blockSize,
                const std::string& destIp,
                int destPort,
                int localPort) {
            SocketTransceiver* socket_transceiver =
                    new SocketTransceiver(blockSize);
            std::unique_ptr<SocketProgramming::ISocket> pIns(new X(destIp, destPort));
            if (pIns->Bind(localPort)) {
                if (pIns->Connect()) // XXX We could have acted as server as well, but
                    // for now act as a client.
                {
                    // save only if successful
                    socket_transceiver->SetStreamTransceiver(pIns);
                }
            }
            else {
                // cerr TODO
            }
            return socket_transceiver;
        }
    };
}

#endif // sigblocks_blocks_transceiver_SocketTransceiverCreator_hh
