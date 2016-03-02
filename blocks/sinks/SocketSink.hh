// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_sink_SocketSink_hh
#define sigblocks_blocks_sink_SocketSink_hh

#include "../../common/Port.hh"

#include <memory>

namespace SocketProgramming
{
  class ISocket;
}

namespace sigblocks
{
  class SocketSink
    : public Port<1, 0, unsigned char>
  {
  public:
    SocketSink();
    SocketSink(std::unique_ptr<SocketProgramming::ISocket> pIns);
    void SetStreamSink(std::unique_ptr<SocketProgramming::ISocket> pIns);

  public: // Port interface
    void Process(int sourceIndex, const unsigned char& data, const TimeTick& startTime);
    void Process(
      int sourceIndex, MultiPtr<unsigned char> pData, int len, const TimeTick& startTime);

  private:
    std::unique_ptr<SocketProgramming::ISocket> mpSocket;
  };
}

#endif // sigblocks_blocks_sink_SocketSink_hh
