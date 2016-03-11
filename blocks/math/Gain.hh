// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_Gain_hh
#define sigblocks_math_Gain_hh

#include "../../common/Port.hh"

namespace sigblocks
{
  template <class T>
  class Gain
    : public Port<1,1,T>
  {
  public:
    Gain(const T& scale)
      : mScale(scale)
    {
    }

  protected: // Port interface
    virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime)
    {
      this->LeakData(0, data * mScale, startTime);
    }

    virtual void Process(
      int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime)
    {
      for (int i = 0; i < len; ++i)
      {
        data.get()[i] *= mScale;
      }
      this->LeakData(0, std::move(data), len, startTime);
    }

  private:
    const T mScale;
  };
}

#endif // sigblocks_math_Gain_hh
