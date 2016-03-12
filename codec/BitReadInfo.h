// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_BITREADINFO_H
#define SIGBLOCKS_BITREADINFO_H

#include <stdint.h>

namespace sigblocks {
    class BitReadInfo {
    public:
        uint8_t* buff;
        int offset;   // offset in buff (byte offset) to
        // read from
        int bitsRead; // bits read at offset (in one octet)
    };
}

#endif // SIGBLOCKS_BITREADINFO_H
