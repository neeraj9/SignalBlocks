// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_codec_MsbitToLsbitOperation_hh
#define sigblocks_common_codec_MsbitToLsbitOperation_hh

#include "BitReadInfo.hh"

namespace sigblocks
{
  class MsbitToLsbitOperation
  {
  public:
    static uint32_t ReadBits(BitReadInfo* bri, int numBits);
  };
}
#endif // sigblocks_common_codec_MsbitToLsbitOperation_hh
