// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_BinaryOperator_hh
#define sigblocks_math_BinaryOperator_hh

#include "../../common/Port.hh"

#include <list>

namespace sigblocks
{
  template <class T>
  class BinaryOperator
    : public Port<2,1,T>
  {
  protected:
    virtual T Compute(const T& arg1, const T& arg2) const = 0;

  protected: // Port interface
    virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime);
    virtual void Process(
      int sourceIndex, MultiPtr<T> pData, int len, const TimeTick& startTime);
  private:
    std::list<T> mDataPort[2];
    std::list<TimeTick> mDataPortTime[2];
  };
}

#endif // sigblocks_math_BinaryOperator_hh
