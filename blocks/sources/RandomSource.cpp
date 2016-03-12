// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "RandomSource.hh"

#include <cstdlib>

using namespace sigblocks;

RandomSource::RandomSource()
        : mLastTick(),
          mSeed(rand()) {
}

void
RandomSource::ClockCycle(const TimeTick& timeTick) {
    if (mLastTick == timeTick) {
        return;  // already processed the event
    }
    mLastTick = timeTick;
    int random_value = rand_r(&mSeed);
    LeakData(0, random_value, timeTick);
}
