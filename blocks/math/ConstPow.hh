// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_ConstPow_hh
#define sigblocks_math_ConstPow_hh

#include "BinaryOperator.hh"

#include <gsl/gsl_math.h>

namespace sigblocks
{
  // computes  x^N, where N is a constant
  template <int N, class T>
  class ConstPow
    : public BinaryOperator<T>
  {
  protected: // BinaryOperator interface
    virtual T Compute(const T& arg1) const
    {
      return gsl_pow_int(arg1, N);
    }

  };

  // Partial template specialization for optimized
  // power function.
  //
  // The c-equivalent functions are not optimized for
  // lower power, so template specialization is employed
  // to take advantage of specific gsl functions.
  //
  // Note to improve efficiency these
  // functions do not check for overflow or underflow conditions.
  //
  // computes  x^N, where N is a constant
#define OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(N) \
  template <##N, class T> \
  class ConstPow \
    : public BinaryOperator<T> \
  { \
  protected: \
    virtual T Compute(const T& arg1) const \
    { \
      return gsl_pow_##N(arg1); \
    } \
  }

  OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(2);
  OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(3);
  OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(4);
  OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(5);
  OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(6);
  OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(7);
  OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(8);
  OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(9);

}

#endif // sigblocks_math_ConstPow_hh
