// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_source_ServerSocketSource_hh
#define sigblocks_blocks_source_ServerSocketSource_hh

#include "../../common/Port.hh"
#include "../../common/TimeTick.hh"

#include <memory>
#include <stdint.h>

namespace SocketProgramming
{
  class IServerSocket;
}

namespace sigblocks
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
               std::unique_ptr<SocketProgramming::IServerSocket> pIns,
               int blockSize);

    void Generate();
    void SetStreamSource(std::unique_ptr<SocketProgramming::IServerSocket> pIns);

  private:
    TimeTick mTime;
    const TimeTick mIncrement;
    std::unique_ptr<SocketProgramming::IServerSocket> mpSocket;
    const int mBlockSize;
    std::unique_ptr<uint8_t[]> mpBuffer;
    int mBytesRead;
    int mBufferSize;
  };
}

#endif // sigblocks_blocks_source_ServerSocketSource_hh
