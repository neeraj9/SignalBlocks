// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_codec_LsbitToMsbitOperation_hh
#define sigblocks_common_codec_LsbitToMsbitOperation_hh

#include "BitReadInfo.hh"

namespace sigblocks {
    class LsbitToMsbitOperation {
    public:
        static uint32_t ReadBits(BitReadInfo* bri, int numBits);
    };
}
#endif // sigblocks_common_codec_LsbitToMsbitOperation_hh
