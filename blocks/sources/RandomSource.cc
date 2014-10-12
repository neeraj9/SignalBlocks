// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "RandomSource.hh"

#include <cstdlib>

using namespace iblocks;

RandomSource::RandomSource(const TimeTick& startTime, const TimeTick& increment)
  : mTime(startTime),
    mIncrement(increment),
    mSeed(0)
{
  mSeed = rand();
}

void
RandomSource::Generate()
{
  int random_value = rand_r(&mSeed);
  LeakData(0, random_value, mTime);
  mTime += mIncrement;
}
