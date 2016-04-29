// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_SERVERSOCKETSOURCE_H
#define SIGBLOCKS_SERVERSOCKETSOURCE_H

#include "../../common/Port.h"
#include "../../common/TimeTick.h"

#include <memory>
#include <stdint.h>

namespace SocketProgramming {
    class IServerSocket;
}

namespace sigblocks {
    class ServerSocketSource
            : public Port<0, 1, unsigned char> {
    public:
        ServerSocketSource(std::string name, int blockSize);

        ServerSocketSource(std::string name,
                           std::unique_ptr<SocketProgramming::IServerSocket> pIns,
                           int blockSize);

        void SetStreamSource(std::unique_ptr<SocketProgramming::IServerSocket> pIns);

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        TimeTick mLastTick;
        std::unique_ptr<SocketProgramming::IServerSocket> mpSocket;
        const int mBlockSize;
        std::unique_ptr<uint8_t[]> mpBuffer;
        int mBytesRead;
        int mBufferSize;
    };
}

#endif // SIGBLOCKS_SERVERSOCKETSOURCE_H
