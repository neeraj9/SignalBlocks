// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_sink_StdoutSink_hh
#define sigblocks_blocks_sink_StdoutSink_hh

#include "../../common/Port.hh"

namespace sigblocks
{
  template <class T>
  class StdoutSink
    : public Port<1, 0, T>
  {
  public: // Port interface
    void Process(int sourceIndex, const T& data, const TimeTick& startTime);
    void Process(
      int sourceIndex, MultiPtr<T> pData, int len, const TimeTick& startTime);
  };
}

#endif // sigblocks_blocks_sink_StdoutSink_hh
