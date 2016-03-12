// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_source_SocketSource_hh
#define sigblocks_blocks_source_SocketSource_hh

#include "../../common/Port.h"
#include "../../common/TimeTick.h"

#include <memory>
#include <stdint.h>

namespace SocketProgramming {
    class ISocket;
}

namespace sigblocks {
    class SocketSource
            : public Port<0, 1, unsigned char> {
    public:
        SocketSource(int blockSize);

        SocketSource(std::unique_ptr<SocketProgramming::ISocket> pIns,
                     int blockSize);

        void SetStreamSource(std::unique_ptr<SocketProgramming::ISocket> pIns);

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        TimeTick mLastTick;
        std::unique_ptr<SocketProgramming::ISocket> mpSocket;
        const int mBlockSize;
        std::unique_ptr<uint8_t[]> mpBuffer;
        int mBytesRead;
        int mBufferSize;
    };
}

#endif // sigblocks_blocks_source_SocketSource_hh
