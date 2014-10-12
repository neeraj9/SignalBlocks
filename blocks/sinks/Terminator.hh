// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_blocks_sink_Terminator_hh
#define iblocks_blocks_sink_Terminator_hh

#include "../../common/Port.hh"

namespace iblocks
{
  template <class T>
  class Terminator
    : public Port<1, 0, T>
  {
  public: // Port interface
    void Process(int sourceIndex, const T& data, const TimeTick& startTime)
    {}
    void Process(int sourceIndex, MultiPtr<T> pData, int len, const TimeTick& startTime)
    {}
  };
}
#endif // iblocks_blocks_sink_Terminator_hh
