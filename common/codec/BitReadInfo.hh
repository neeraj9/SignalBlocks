// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_common_codec_BitReadInfo_hh
#define iblocks_common_codec_BitReadInfo_hh

#include <stdint.h>

namespace iblocks
{
  class BitReadInfo
  {
  public:
    uint8_t* buff;
    int offset;   // offset in buff (byte offset) to
                  // read from
    int bitsRead; // bits read at offset (in one octet)
  };
}

#endif // iblocks_common_codec_BitReadInfo_hh
