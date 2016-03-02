// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include <assert.h>
#include "Multiplex.hh"

using namespace sigblocks;
using namespace std;

template <int N, class T>
Multiplex<N,T>::Multiplex()
{
}

template <int N, class T>
Multiplex<N,T>::~Multiplex()
{
}

template <int N, class T>
void
Multiplex<N,T>::Process(int sourceIndex, const T& data, const TimeTick& startTime)
{
  assert(sourceIndex >= 0 || sourceIndex <= N); // XXX change to an assertion library.
  mDataPort[sourceIndex].push_back(data);
  mDataPortTime[sourceIndex].push_back(startTime);

  size_t min_elements = mDataPort[0].size();
  for (int i = 1; i < N; ++i)
  {
    const size_t new_min_elements = mDataPort[i].size();
    if (new_min_elements < min_elements)
    {
      min_elements = new_min_elements;
    }
  }

  if (min_elements <= 0)
  {
    return;
  }

  TimeTick min_time = mDataPortTime[0].front();
  for (int i = 1; i < N; ++i)
  {
    // We are sure that min_elements > 0 so no need to check for
    // the size of list before getting its front value.
    const TimeTick new_min_time = mDataPortTime[i].front();
    if (new_min_time < min_time) // operator< is better than operator>
                                 // see TimeTick for details
    {
      min_time = new_min_time;
    }
  }

  // assert(min_elements > 0);
  std::list<T> arguments;
  for (int i = 0; i < N; ++i)
  {
    if (mDataPortTime[i].front() == min_time)
    {
      arguments.push_back(mDataPort[i].front());
      mDataPort[i].pop_front();
      mDataPortTime[i].pop_front();
    }
    else
    {
      arguments.push_back(0); // insert zero for unmatched-rate (XXX)
      // unmatched rate is a big topic and should be carefully thought
      // and documented. XXX
    }
  }
  // XXX think about one by one multiplexing instead
  // of waiting for all lines to be available before sending
  // downstream multiplexing them in order
  for (typename std::list<T>::iterator iter = arguments.begin();
       iter != arguments.end(); ++iter)
  {
    this->LeakData(0, *iter, min_time);
  }
}

template <int N, class T>
void
Multiplex<N,T>::Process(
  int sourceIndex, MultiPtr<T> pData, int len, const TimeTick& startTime)
{
  assert(0); // FIXME
}

// have some template specialization for optimization
// for lower value of N.
#define N1_SPECIALIZE_SCALAR_PROCESS(T) \
template <> \
void \
Multiplex<1,T>::Process( \
  int sourceIndex, const T& data, const TimeTick& startTime) \
{ \
  assert(sourceIndex == 0); \
  this->LeakData(0, data, startTime); \
}

#define N2_SPECIALIZE_SCALAR_PROCESS(T) \
template <> \
void \
Multiplex<2,T>::Process( \
  int sourceIndex, const T& data, const TimeTick& startTime) \
{ \
  assert(sourceIndex == 0 || sourceIndex == 1); \
  mDataPort[sourceIndex].push_back(data); \
  mDataPortTime[sourceIndex].push_back(startTime); \
  size_t min_elements = mDataPort[0].size(); \
  if (min_elements > mDataPort[1].size()) \
  { \
    min_elements = mDataPort[1].size(); \
  } \
  if (min_elements <= 0) \
  { \
    return; \
  } \
  T arg1 = mDataPort[0].front(); \
  T arg2 = mDataPort[1].front(); \
  TimeTick min_time = mDataPortTime[0].front(); \
  if (mDataPortTime[1].front() < min_time) \
  { \
    arg1 = 0; /* insert zero when rate do not match */ \
    mDataPort[1].pop_front(); \
    mDataPortTime[1].pop_front(); \
  } \
  else if (min_time < mDataPortTime[1].front()) \
  { \
    arg2 = 0; /* insert zero when rate do not match */ \
    mDataPort[0].pop_front(); \
    mDataPortTime[0].pop_front(); \
  } \
  else /* both are equal */ \
  { \
    mDataPort[0].pop_front(); \
    mDataPortTime[0].pop_front(); \
    mDataPort[1].pop_front(); \
    mDataPortTime[1].pop_front(); \
  } \
  this->LeakData(0, arg1, min_time); \
  this->LeakData(0, arg2, min_time); \
}

#define SPECIALIZE_SCALAR_PROCESS(Type) \
  N1_SPECIALIZE_SCALAR_PROCESS(Type) \
  N2_SPECIALIZE_SCALAR_PROCESS(Type)

// This is somehow required by c++ otherwise
// the compilers complain for specialization
// and the definition in different namespaces.
namespace sigblocks
{
  SPECIALIZE_SCALAR_PROCESS(long)
  SPECIALIZE_SCALAR_PROCESS(unsigned long)
  SPECIALIZE_SCALAR_PROCESS(int)
  SPECIALIZE_SCALAR_PROCESS(unsigned int)
  SPECIALIZE_SCALAR_PROCESS(unsigned char)
  SPECIALIZE_SCALAR_PROCESS(float)
  SPECIALIZE_SCALAR_PROCESS(double)
}

#define INSTANTIATE_TYPES_FOR_N_INPUTS(NUM) \
  template class sigblocks::Multiplex<NUM, long>; \
  template class sigblocks::Multiplex<NUM, unsigned long>; \
  template class sigblocks::Multiplex<NUM, int>; \
  template class sigblocks::Multiplex<NUM, unsigned int>; \
  template class sigblocks::Multiplex<NUM, unsigned char>; \
  template class sigblocks::Multiplex<NUM, float>; \
  template class sigblocks::Multiplex<NUM, double>;

// XXX allow max of two inputs for now.
INSTANTIATE_TYPES_FOR_N_INPUTS(1)
INSTANTIATE_TYPES_FOR_N_INPUTS(2)
INSTANTIATE_TYPES_FOR_N_INPUTS(3)
