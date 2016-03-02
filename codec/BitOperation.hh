// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_codec_BitOperation_hh
#define sigblocks_common_codec_BitOperation_hh

#include "BitReadInfo.hh"

#include <stdint.h>

namespace sigblocks
{
  templace <typename Operation>
  class BitOperation
  {
  public:
    static unsigned int ReadBits(BitReadInfo* bri, int numBits);
    static int ReadBits(BitReadInfo* bri, int numBits, uint8_t* result);
    static int ReadBytes(BitReadInfo* bri, int numBytes, uint8_t* result);
  };

  // XXX Let client worry about availability of bits in bri
  template <typename Operation>
  unsigned int
  BitOperation<Operation>::ReadBits(BitReadInfo* bri, int numBits)
  {
    if (numBits > 0 && numBits <= 32)
    {
      unsigned int result = Operation::ReadBits(bri, numBits);
      return result;
    }
    else
    {
      return -1;
    }
    return 0;
  }

  // XXX Let client worry about availability of bits in bri
  template <typename Operation>
  int
  BitOperation<Operation>::ReadBits(
    BitReadInfo* bri, int numBits, uint8_t* result)
  {
    if (numBits < 0)
    {
      return -1;
    }
    if (numBits > 32)
    {
      if (bri->bitsRead == 0)
      {
        memcpy(result, bri->buff, numBits / 8);
        numBits -= 8 * (numBits / 8);
        result += (numBits / 8);
      }
      while (numBits > 32)
      {
        unsigned int tmp = Operation::ReadBits(bri, 32);
        memcpy(result, &tmp, 4);
        result += 4;
        numBits -= 32;
      }
      if (numBits > 0)
      {
        unsigned int tmp = Operation::ReadBits(bri, numBits);
        memcpy(result, &tmp, (numBits + 7) / 8);
      }
    }
    else if (numBits > 0 && numBits <= 32)
    {
      unsigned int tmp = Operation::ReadBits(bri, numBits);
      memcpy(result, &tmp, (numBits + 7) / 8);
    }
    return 0;
  }

  // XXX Let client worry about availability of bits in bri
  template <typename Operation>
  int
  ReadBytes(BitReadInfo* bri, int numBytes, uint8_t* result)
  {
    if (numBytes < 0)
    {
      return -1;
    }
    if (bri->bitsRead > 0)
    {
      int num_bits_to_read = numBytes * 8;
      while (num_bits_to_read > 32)
      {
        unsigned int tmp = Operation::ReadBits(bri, 32);
        memcpy(result, &tmp, 4);
        result += 4;
        num_bits_to_read -= 32;
      }
      if (num_bits_to_read > 0)
      {
        unsigned int tmp = Operation::ReadBits(bri, num_bits_to_read);
        memcpy(result, &tmp, (num_bits_to_read + 7) / 8);
      }
    }
    else
    {
      memcpy(result, bri->buff, numBytes);
    }
    return 0;
  }
}
#endif // sigblocks_common_codec_BitOperation_hh
