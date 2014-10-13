// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_Sum_hh
#define sigblocks_math_Sum_hh

#include "NOperator.hh"

namespace sigblocks
{
  template <int N, class T>
  class Sum
    : public NOperator<N, T>
  {
  protected: // NOperator interface
    virtual T Compute(const T& arg1) const
    {
      return arg1;
    }

    virtual T Compute(const T& arg1, const T& arg2) const
    {
      return (arg1 + arg2);
    }

    virtual T Compute(const std::list<T>& args) const
    {
      typename std::list<T>::const_iterator iter = args.begin();
      T result = *iter; // at least one is present
      ++iter;
      for ( ; iter != args.end(); ++iter)
      {
        result += (*iter);
      }
      return result;
    }
  };
}

#endif // sigblocks_math_Sum_hh
