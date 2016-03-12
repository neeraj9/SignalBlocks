// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_source_IstreamSource_hh
#define sigblocks_blocks_source_IstreamSource_hh

#include "../../common/Port.hh"
#include "../../common/TimeTick.hh"

#include <memory>

namespace sigblocks {
    template<class T>
    class IstreamSource
            : public Port<0, 1, T> {
    public:
        IstreamSource(const TimeTick& startTime,
                      TimeTick& increment,
                      int blockSize);

        IstreamSource(const TimeTick& startTime,
                      TimeTick& increment,
                      std::unique_ptr<std::istream> pIns,
                      int blockSize);

        void Generate();

        void SetStreamSource(std::unique_ptr<std::istream> pIns);

        void Loop(bool loopOver);

    private:
        TimeTick mTime;
        const TimeTick mIncrement;
        std::unique_ptr<std::istream> mpIstream;
        bool mLoopOver;
        int mBlockSize;
    };
}

#endif // sigblocks_blocks_source_IstreamSource_hh
