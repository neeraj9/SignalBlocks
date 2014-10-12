// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_math_Serialize_hh
#define iblocks_math_Serialize_hh

#include "../../common/Port.hh"

namespace iblocks
{
  template <class T>
  class Serialize
    : public Port<1,1,T>
  {
  protected: // Port interface
    virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime)
    {
      LeakData(0, data, startTime);
    }

    virtual void Process(
      int sourceIndex, MultiPtr<T> pData, int len, const TimeTick& startTime)
    {
      assert(sourceIndex == 0); // XXX change to an assertion library.
      for (int i = 0; i < len; ++i)
      {
        LeakData(0, pData.get()[i], startTime);
      }
    }
  };
}

#endif // iblocks_math_Serialize_hh
