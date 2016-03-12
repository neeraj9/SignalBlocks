// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_RANDOMSOURCE_H
#define SIGBLOCKS_RANDOMSOURCE_H

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

#endif // SIGBLOCKS_RANDOMSOURCE_H
