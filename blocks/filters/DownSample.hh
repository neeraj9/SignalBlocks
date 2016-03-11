// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_DownSample_hh
#define sigblocks_math_DownSample_hh

#include "../../common/Port.hh"

#include <list>

namespace sigblocks
{
  template <int N, class T>
  class DownSample
    : public Port<1,1,T>
  {
  public:
    DownSample(int factor)
      : mDownSampleFactor(factor),
        mCurrentCount(0)
    {
      assert(mDownSampleFactor > 0); // XXX replace with some assertion library
    }

  protected: // Port interface
    virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime)
    {
      assert(sourceIndex == 0); // XXX
      if (mCurrentCount == 0)
      {
        LeakData(0, data, startTime);
      }
      mCurrentCount = (mCurrentCount + 1) % mDownSampleFactor;
    }

    virtual void Process(
      int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime)
    {
      assert(sourceIndex == 0); // XXX
      if (len < 0)
      {
        return;
      }
      int rem_len = len;
      int count = 0;
      if (mCurrentCount == 0)
      {
        LeakData(0, data.get()[0], startTime);
        mCurrentCount = (mCurrentCount + 1) % mDownSampleFactor;
        --rem_len;
        ++count;
      }
      const int max_count = ((mCurrentCount + rem_len) / mDownSampleFactor);
      for (; count < max_count; ++count)
      {
        LeakData(0, data.get()[count * mDownSampleFactor], startTime);
      }
      mCurrentCount = (mCurrentCount + rem_len) % mDownSampleFactor;
    }

  private:
    int mDownSampleFactor;
    int mCurrentCount;
  };
}

#endif // sigblocks_math_DownSample_hh
