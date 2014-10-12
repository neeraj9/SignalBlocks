// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_common_codec_MsbitToLsbitOperation_hh
#define iblocks_common_codec_MsbitToLsbitOperation_hh

#include "BitReadInfo.hh"

namespace iblocks
{
  class MsbitToLsbitOperation
  {
  public:
    static uint32_t ReadBits(BitReadInfo* bri, int numBits);
  };
}
#endif // iblocks_common_codec_MsbitToLsbitOperation_hh
