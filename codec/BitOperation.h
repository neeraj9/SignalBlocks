// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// This file is part of SignalBlocks.
//
// SignalBlocks is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SignalBlocks is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SignalBlocks.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SIGNALBLOCKS_BITOPERATION_H
#define SIGNALBLOCKS_BITOPERATION_H

#include "BitReadInfo.h"

#include <cstring>
#include <stdint.h>

namespace signalblocks {
    template<typename Operation>
    class BitOperation {
    public:
        static unsigned int ReadBits(BitReadInfo* bri, int numBits);

        static int ReadBits(BitReadInfo* bri, int numBits, uint8_t* result);

        static int ReadBytes(BitReadInfo* bri, int numBytes, uint8_t* result);
    };

    // XXX Let client worry about availability of bits in bri
    template<typename Operation>
    unsigned int
    BitOperation<Operation>::ReadBits(BitReadInfo* bri, int numBits) {
        if (numBits > 0 && numBits <= 32) {
            unsigned int result = Operation::ReadBits(bri, numBits);
            return result;
        }
        else {
            return -1;
        }
        return 0;
    }

    // XXX Let client worry about availability of bits in bri
    template<typename Operation>
    int
    BitOperation<Operation>::ReadBits(
            BitReadInfo* bri, int numBits, uint8_t* result) {
        if (numBits < 0) {
            return -1;
        }
        if (numBits > 32) {
            if (bri->bitsRead == 0) {
                memcpy(result, bri->buff, numBits / 8);
                numBits -= 8 * (numBits / 8);
                result += (numBits / 8);
            }
            while (numBits > 32) {
                unsigned int tmp = Operation::ReadBits(bri, 32);
                memcpy(result, &tmp, 4);
                result += 4;
                numBits -= 32;
            }
            if (numBits > 0) {
                unsigned int tmp = Operation::ReadBits(bri, numBits);
                memcpy(result, &tmp, (numBits + 7) / 8);
            }
        }
        else if (numBits > 0 && numBits <= 32) {
            unsigned int tmp = Operation::ReadBits(bri, numBits);
            memcpy(result, &tmp, (numBits + 7) / 8);
        }
        return 0;
    }

    // XXX Let client worry about availability of bits in bri
    template<typename Operation>
    int
    ReadBytes(BitReadInfo* bri, int numBytes, uint8_t* result) {
        if (numBytes < 0) {
            return -1;
        }
        if (bri->bitsRead > 0) {
            int num_bits_to_read = numBytes * 8;
            while (num_bits_to_read > 32) {
                unsigned int tmp = Operation::ReadBits(bri, 32);
                memcpy(result, &tmp, 4);
                result += 4;
                num_bits_to_read -= 32;
            }
            if (num_bits_to_read > 0) {
                unsigned int tmp = Operation::ReadBits(bri, num_bits_to_read);
                memcpy(result, &tmp, (num_bits_to_read + 7) / 8);
            }
        }
        else {
            memcpy(result, bri->buff, numBytes);
        }
        return 0;
    }
}
#endif // SIGNALBLOCKS_BITOPERATION_H
