// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_source_IstreamSource_hh
#define sigblocks_blocks_source_IstreamSource_hh

#include "../../common/Port.h"
#include "../../common/TimeTick.h"

#include <memory>

namespace sigblocks {
    template<class T>
    class IstreamSource
            : public Port<0, 1, T> {
    public:
        IstreamSource(int blockSize);

        IstreamSource(int blockSize,
                      std::unique_ptr<std::istream> pIns);

        void SetStreamSource(std::unique_ptr<std::istream> pIns);

        void Loop(bool loopOver);

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        TimeTick mLastTick;
        std::unique_ptr<std::istream> mpIstream;
        bool mLoopOver;
        int mBlockSize;
    };
}

#endif // sigblocks_blocks_source_IstreamSource_hh
