// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_Arctanh_hh
#define sigblocks_math_Arctanh_hh

#include "BinaryOperator.hh"

#include <gsl/gsl_math.h>

namespace sigblocks
{
  // computes  \arctanh(x)
  template <class T>
  class Arctanh
    : public BinaryOperator<T>
  {
  protected: // BinaryOperator interface
    virtual T Compute(const T& arg1) const
    {
      return gsl_atanh(arg1);
    }

  };
}

#endif // sigblocks_math_Arctanh_hh
