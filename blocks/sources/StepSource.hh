// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_source_StepSource_hh
#define sigblocks_blocks_source_StepSource_hh

#include "../../common/Port.hh"
#include "../../common/TimeTick.hh"

namespace sigblocks {
    template<class T>
    class StepSource
            : public Port<0, 1, T> {
    public:
        StepSource(const T& value);

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        TimeTick mLastTick;
        T mValue;
    };
}

#endif // sigblocks_blocks_source_StepSource_hh
