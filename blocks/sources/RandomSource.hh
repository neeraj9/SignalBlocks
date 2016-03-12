// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_source_RandomSource_hh
#define sigblocks_blocks_source_RandomSource_hh

#include "../../common/Port.hh"
#include "../../common/TimeTick.hh"

namespace sigblocks {
    class RandomSource
            : public Port<0, 1, int> {
    public:
        RandomSource(const TimeTick& startTime, const TimeTick& increment);

        void Generate();

    private:
        TimeTick mTime;
        const TimeTick mIncrement;
        unsigned int mSeed;
    };
}

#endif // sigblocks_blocks_source_RandomSource_hh
