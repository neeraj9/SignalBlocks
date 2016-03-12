// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "MsbitToLsbitOperation.hh"

#include "BitReadInfo.hh"

#include <stdint.h>

using namespace sigblocks;

namespace {
    /* read order is msbit to lsbit */
    static uint8_t
    ReadPartialBitsMsbitToLsbit(BitReadInfo* bri, int* numBits) {
        uint8_t result = 0;
        uint8_t mask = 0;
        int bits_available = 0;
        int bits_to_read = 0;

        bits_available = 8 - bri->bitsRead;
        if (bits_available >= *numBits) {
            bits_to_read = *numBits;
            *numBits = 0;
        }
        else {
            bits_to_read = bits_available;
            (*numBits) -= bits_to_read;
        }
        mask = (0xff >> bri->bitsRead);
        result = bri->buff[bri->offset];
        result = (result & mask);
        result = result >> (8 - bits_to_read);

        if (bits_to_read < bits_available) {
            bri->bitsRead += bits_to_read;
        }
        else /* read only bits which are available */
        {
            bri->bitsRead = 0;
            bri->offset++;
        }
        return result;
    }
}

/* read order is msbit to lsbit */
uint32_t
MsbitToLsbitOperation::ReadBits(BitReadInfo* bri, int numBits) {
    uint32_t result = 0;

    if (bri->bitsRead > 0) {
        result = ReadPartialBitsMsbitToLsbit(bri, &numBits);
        if (numBits == 0) {
            return result;
        }
        else {
            result = result << numBits; /* make space for more bits */
        }
    }

    switch (numBits / 8) {
        case 4:
            result = result | (bri->buff[bri->offset] << 24);
            bri->offset++;
            /* fall through */
        case 3:
            result = result | (bri->buff[bri->offset] << 16);
            bri->offset++;
            /* fall through */
        case 2:
            result = result | (bri->buff[bri->offset] << 8);
            bri->offset++;
            /* fall through */
        case 1:
            result = result | bri->buff[bri->offset];
            bri->offset++;
            break;
        case 0:
            break;
    }
    numBits = numBits % 8;
    if (numBits > 0) {
        result = result | ReadPartialBitsMsbitToLsbit(bri, &numBits);
    }
    return result;
}
