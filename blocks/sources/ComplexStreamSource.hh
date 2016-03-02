// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_source_ComplexStreamSource_hh
#define sigblocks_blocks_source_ComplexStreamSource_hh

#include "../../common/Port.hh"
#include "../../common/TimeTick.hh"

#include <memory>

namespace sigblocks
{
  template <class T>
  class ComplexStreamSource
    : public Port<0, 1, T>
  {
  public:
    ComplexStreamSource(const TimeTick& startTime,
                  TimeTick& increment,
                  int blockSize);
    ComplexStreamSource(const TimeTick& startTime,
                  TimeTick& increment,
                  std::unique_ptr<std::istream> pIns,
                  int blockSize);

    void Generate();
    void SetStreamSource(std::unique_ptr<std::istream> pIns);
    void Loop(bool loopOver);

  private:
    TimeTick mTime;
    const TimeTick mIncrement;
    std::unique_ptr<std::istream> mpComplexStream;
    bool mLoopOver;
    int mBlockSize;
  };
}

#endif // sigblocks_blocks_source_ComplexStreamSource_hh
