// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_blocks_source_SocketSource_hh
#define iblocks_blocks_source_SocketSource_hh

#include "../../common/MultiPtr.hh"
#include "../../common/Port.hh"
#include "../../common/TimeTick.hh"

#include <memory>
#include <stdint.h>

namespace SocketProgramming
{
  class ISocket;
}

namespace iblocks
{
  class SocketSource
    : public Port<0, 1, unsigned char>
  {
  public:
    SocketSource(const TimeTick& startTime,
               TimeTick& increment,
               int blockSize);
    SocketSource(const TimeTick& startTime,
               TimeTick& increment,
               std::auto_ptr<SocketProgramming::ISocket> pIns,
               int blockSize);

    void Generate();
    void SetStreamSource(std::auto_ptr<SocketProgramming::ISocket> pIns);

  private:
    TimeTick mTime;
    const TimeTick mIncrement;
    std::auto_ptr<SocketProgramming::ISocket> mpSocket;
    const int mBlockSize;
    MultiPtr<uint8_t> mpBuffer;
    int mBytesRead;
    int mBufferSize;
  };
}

#endif // iblocks_blocks_source_SocketSource_hh
