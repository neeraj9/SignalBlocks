// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_NOperator_hh
#define sigblocks_math_NOperator_hh

#include "../../common/Port.hh"

#include <list>

namespace sigblocks
{
  template <int N, class T>
  class NOperator
    : public Port<N,1,T>
  {
  public:
    NOperator(); // need to set mIsVectorEnabled to false
    virtual ~NOperator(); // Need for vector buffer

  protected:
    // scalar operations
    virtual T Compute(const T& arg1) const = 0; // for N=1
    virtual T Compute(const T& arg1,
                      const T& arg2) const = 0; // for N=2
    virtual T Compute(const std::list<T>& args) const = 0; // Generic

#if 0 // XXX
    // vector operations
    //
    // Note: the input is not const because the algo can do in-place
    //       operation, in which case input will loose the ownership of the
    //       data to output.
    // Note: Client should not pre-allocate memory for the output.
    virtual void Compute(std::unique_ptr<T[]>& input,
                         int len,
                         std::unique_ptr<T[]>& output) const = 0;

    // Note: Either intput1 or input2 memory can be used by the output.
    // Note: If size of intput1 and input2 are different then the smaller
    //       one should be zero padded (virtually and no new realloc is done), so
    //       for in-place algo the bigger intput's storage is passed to the
    //       output.
    // Note: Zero pading needs to be taken care by the algo and *not* here.
    //       This is to avoid the realloc of the memory.
    // Note: Special case - if either one of input1, intput2 is null then
    //       it implies that its not present, so the algo needs to take care
    //       of that scenario as well by asumming all zeros.
    virtual void Compute(std::unique_ptr<T[]>& input1, int len1,
                         std::unique_ptr<T[]>& input2, int len2,
                         std::unique_ptr<T[]>& output) const = 0;

    // Generic array based interface.
    // Note: The list stores pointer instead of value to save ctor/dtor of
    //       the MultiPtr class.
    // Note: The logic of in-place algorithm and zero padding is the same as
    //       for 2 inputs.
    // Note: Special case - if either one of input1, ... intputN is null then
    //       it implies that its not present, so the algo needs to take care
    //       of that scenario as well by asumming all zeros.
    virtual void Compute(std::list<std::unique_ptr<T[]>*>& inputs,
                         std::list<int>& len,
                         std::unique_ptr<T[]>& output) const = 0;
#endif

  protected: // Port interface
    virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime);
    virtual void Process(
      int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime);

  private:
    bool mIsVectorEnabled; // To start with this can be enabled
                           // on even a single vector Process call.
                           // Henceforth the scapar Process should
                           // look for vector buffer and not assume
                           // only scalar inputs.

    // scalar input buffer
    std::list<T> mDataPort[N];
    std::list<TimeTick> mDataPortTime[N];

    // vector input buffer
    // Note: There can be multiple cases with
    //       vector and scalar operatios.
    //          * An input port can send scalar and
    //            vector anytime during the lifetime.
    //            It should not be but can happen.
    //          * All the input ports need not be of
    //            vector or scalar. A mix of scalar and
    //            vector ports is a possibility.
    std::list<std::unique_ptr<T[]>*> mDataPortVector[N];
    std::list<int> mDataPortVectorLength[N];
    std::list<TimeTick> mDataPortVectorTime[N];
  };
}

#endif // sigblocks_math_NOperator_hh
