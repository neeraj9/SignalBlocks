// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_source_StepSource_hh
#define sigblocks_blocks_source_StepSource_hh

#include "../../common/Port.hh"
#include "../../common/TimeTick.hh"

namespace sigblocks
{
  template <class T>
  class StepSource
    : public Port<0, 1, T>
  {
  public:
    StepSource(
      const TimeTick& startTime, const TimeTick& increment, const T& value);
    void Generate();

  private:
    TimeTick mTime;
    const TimeTick mIncrement;
    T mValue;
  };
}

#endif // sigblocks_blocks_source_StepSource_hh
