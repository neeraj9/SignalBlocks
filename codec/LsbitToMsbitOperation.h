// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_LSBITTOMSBITOPERATION_H
#define SIGBLOCKS_LSBITTOMSBITOPERATION_H

#include "BitReadInfo.h"

namespace sigblocks {
    class LsbitToMsbitOperation {
    public:
        static uint32_t ReadBits(BitReadInfo* bri, int numBits);
    };
}
#endif // SIGBLOCKS_LSBITTOMSBITOPERATION_H
