// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_source_RandomSource_hh
#define sigblocks_blocks_source_RandomSource_hh

#include "../../common/Port.h"
#include "../../common/TimeTick.h"

namespace sigblocks {
    class RandomSource
            : public Port<0, 1, int> {
    public:
        RandomSource();

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        TimeTick mLastTick;
        unsigned int mSeed;
    };
}

#endif // sigblocks_blocks_source_RandomSource_hh
