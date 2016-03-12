// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_BASICTYPECONVERTER_H
#define SIGBLOCKS_BASICTYPECONVERTER_H

#include "../../common/MixedPort.h"

namespace sigblocks {
    template<class TFROM, class TTO>
    class BasicTypeConverter
            : public MixedPort<1, 1, TFROM, TTO> {
    protected: // Override MixedPort interfaces for Sink
        virtual void Process(int sourceIndex, const TFROM& data, const TimeTick& startTime);

        virtual void Process(
                int sourceIndex, std::unique_ptr<TFROM[]> data, int len, const TimeTick& startTime);
    };
}

#endif // SIGBLOCKS_BASICTYPECONVERTER_H
