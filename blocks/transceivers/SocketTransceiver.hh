// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_sink_SocketTransceiver_hh
#define sigblocks_blocks_sink_SocketTransceiver_hh

#include "../../common/Port.hh"
#include "../../common/TimeTick.hh"

#include <stdint.h>
#include <memory>

namespace SocketProgramming {
    class ISocket;
}

namespace sigblocks {
    class SocketTransceiver
            : public Port<1, 1, unsigned char> {
    public:
        // startTime, increment and blockSize are
        // source only parameters
        SocketTransceiver(const TimeTick& startTime,
                          TimeTick& increment,
                          int blockSize);

        SocketTransceiver(const TimeTick& startTime,
                          TimeTick& increment,
                          int blockSize,
                          std::unique_ptr<SocketProgramming::ISocket> pIns);

        void Generate(); // Source only interface
        void SetStreamTransceiver(std::unique_ptr<SocketProgramming::ISocket> pIns);

    public: // Port interface needed to be overridden only by the Sink
        void Process(int sourceIndex, const unsigned char& data, const TimeTick& startTime);

        void Process(
                int sourceIndex, std::unique_ptr<unsigned char[]> data, int len, const TimeTick& startTime);

    private:
        TimeTick mTime; // only for source
        const TimeTick mIncrement; // only for source
        std::unique_ptr<SocketProgramming::ISocket> mpSocket;
        const int mBlockSize; // only for source
        std::unique_ptr<uint8_t[]> mpBuffer; // only for source
        int mBytesRead; // only for source
        int mBufferSize; // only for source
    };
}

#endif // sigblocks_blocks_sink_SocketTransceiver_hh
