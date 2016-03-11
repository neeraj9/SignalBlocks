// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_Sin_hh
#define sigblocks_math_Sin_hh

#include "BinaryOperator.hh"

#include <cmath>

namespace sigblocks
{
  // computes  \sin(x)
  template <class T>
  class Sin
    : public BinaryOperator<T>
  {
  protected: // BinaryOperator interface
    virtual T Compute(const T& arg1) const
    {
      return sin(arg1);
    }

  };
}

#endif // sigblocks_math_Sin_hh
