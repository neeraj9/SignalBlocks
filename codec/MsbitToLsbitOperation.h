// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_MSBITTOLSBITOPERATION_H
#define SIGBLOCKS_MSBITTOLSBITOPERATION_H

#include "BitReadInfo.h"

namespace sigblocks {
    class MsbitToLsbitOperation {
    public:
        static uint32_t ReadBits(BitReadInfo* bri, int numBits);
    };
}
#endif // SIGBLOCKS_MSBITTOLSBITOPERATION_H
