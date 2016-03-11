// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_Arcsinh_hh
#define sigblocks_math_Arcsinh_hh

#include "BinaryOperator.hh"

#include <gsl/gsl_math.h>

namespace sigblocks
{
  // computes  \arcsinh(x)
  template <class T>
  class Arcsinh
    : public BinaryOperator<T>
  {
  protected: // BinaryOperator interface
    virtual T Compute(const T& arg1) const
    {
      return gsl_asinh(arg1);
    }

  };
}

#endif // sigblocks_math_Arcsinh_hh
