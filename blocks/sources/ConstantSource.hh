// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_source_ConstantSource_hh
#define sigblocks_blocks_source_ConstantSource_hh

#include "../../common/Port.hh"
#include "../../common/TimeTick.hh"

namespace sigblocks
{
  template <class T>
  class ConstantSource
    : public Port<0, 1, T>
  {
  public:
    ConstantSource(
      const TimeTick& startTime, const TimeTick& increment, const T& value);
    void Generate();

  private:
    TimeTick mTime;
    const TimeTick mIncrement;
    const T mFixedValue;
  };
}

#endif // sigblocks_blocks_source_ConstantSource_hh
