// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_blocks_source_ServerSocketSource_hh
#define iblocks_blocks_source_ServerSocketSource_hh

#include "../../common/MultiPtr.hh"
#include "../../common/Port.hh"
#include "../../common/TimeTick.hh"

#include <memory>
#include <stdint.h>

namespace SocketProgramming
{
  class IServerSocket;
}

namespace iblocks
{
  class ServerSocketSource
    : public Port<0, 1, unsigned char>
  {
  public:
    ServerSocketSource(const TimeTick& startTime,
               TimeTick& increment,
               int blockSize);
    ServerSocketSource(const TimeTick& startTime,
               TimeTick& increment,
               std::auto_ptr<SocketProgramming::IServerSocket> pIns,
               int blockSize);

    void Generate();
    void SetStreamSource(std::auto_ptr<SocketProgramming::IServerSocket> pIns);

  private:
    TimeTick mTime;
    const TimeTick mIncrement;
    std::auto_ptr<SocketProgramming::IServerSocket> mpSocket;
    const int mBlockSize;
    MultiPtr<uint8_t> mpBuffer;
    int mBytesRead;
    int mBufferSize;
  };
}

#endif // iblocks_blocks_source_ServerSocketSource_hh
