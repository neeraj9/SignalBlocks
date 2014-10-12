// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_math_Difference_hh
#define iblocks_math_Difference_hh

#include "NOperator.hh"

namespace iblocks
{
  // implement a(n) - b(n) - c(n) - ...
  template <int N, class T>
  class Difference
    : public NOperator<N, T>
  {
  protected: // NOperator interface
    virtual T Compute(const T& arg1) const
    {
      return arg1;
    }

    virtual T Compute(const T& arg1, const T& arg2) const
    {
      return (arg2 - arg1);
    }

    // args[0], ... args[N] = a(n), b(n), ... 
    // implement args[0] - args[1] - ... args[N-1]
    virtual T Compute(const std::list<T>& args) const
    {
      typename std::list<T>::const_iterator iter = args.begin();
      T result = *iter; // at least one is present
      ++iter;
      for ( ; iter != args.end(); ++iter)
      {
        result = result - (*iter); // XXX saturate if enabled.
      }
      return result;
    }
  };
}

#endif // iblocks_math_Difference_hh
