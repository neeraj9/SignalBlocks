// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_common_codec_LsbitToMsbitOperation_hh
#define iblocks_common_codec_LsbitToMsbitOperation_hh

#include "BitReadInfo.hh"

namespace iblocks
{
  class LsbitToMsbitOperation
  {
  public:
    static uint32_t ReadBits(BitReadInfo* bri, int numBits);
  };
}
#endif // iblocks_common_codec_LsbitToMsbitOperation_hh
