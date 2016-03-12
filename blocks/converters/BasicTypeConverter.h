// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_converters_BasicTypeConverter_hh
#define sigblocks_converters_BasicTypeConverter_hh

#include "../../common/MixedPort.hh"

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

#endif // sigblocks_converters_BasicTypeConverter_hh
