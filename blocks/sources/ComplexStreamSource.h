// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_source_ComplexStreamSource_hh
#define sigblocks_blocks_source_ComplexStreamSource_hh

#include "../../common/Port.h"
#include "../../common/TimeTick.h"

#include <memory>

namespace sigblocks {
    template<class T>
    class ComplexStreamSource
            : public Port<0, 1, T> {
    public:
        ComplexStreamSource(int blockSize);

        ComplexStreamSource(std::unique_ptr<std::istream> pIns,
                            int blockSize);

        void SetStreamSource(std::unique_ptr<std::istream> pIns);

        void Loop(bool loopOver);

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        TimeTick mLastTick;
        std::unique_ptr<std::istream> mpComplexStream;
        bool mLoopOver;
        int mBlockSize;
    };
}

#endif // sigblocks_blocks_source_ComplexStreamSource_hh
