// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "Demultiplex.hh"

using namespace iblocks;
using namespace std;

template <int N, class T>
Demultiplex<N,T>::Demultiplex()
  : mNextOutputPort(0)
{
}

template <int N, class T>
Demultiplex<N,T>::~Demultiplex()
{
}

template <int N, class T>
void
Demultiplex<N,T>::Process(int sourceIndex, const T& data, const TimeTick& startTime)
{
  assert(sourceIndex >= 0 || sourceIndex < N); // XXX change to an assertion library.

  this->LeakData(mNextOutputPort, data, startTime);

  mNextOutputPort = (mNextOutputPort + 1) % N;
}

template <int N, class T>
void
Demultiplex<N,T>::Process(
  int sourceIndex, MultiPtr<T> pData, int len, const TimeTick& startTime)
{
  assert(sourceIndex >= 0 || sourceIndex < N); // XXX change to an assertion library.

  // Mutiple inputs are treated as one when passing downstream
  // use some other class to split such a data into individual data
  // items if those need to be demultiplexed and then apply this module.
  this->LeakData(mNextOutputPort, pData, len, startTime);

  mNextOutputPort = (mNextOutputPort + 1) % N;
}

#define INSTANTIATE_TYPES_FOR_N_INPUTS(NUM) \
  template class Demultiplex<NUM, long>; \
  template class Demultiplex<NUM, unsigned long>; \
  template class Demultiplex<NUM, int>; \
  template class Demultiplex<NUM, unsigned int>; \
  template class Demultiplex<NUM, unsigned char>; \
  template class Demultiplex<NUM, float>; \
  template class Demultiplex<NUM, double>;

// XXX allow max of two inputs for now.
INSTANTIATE_TYPES_FOR_N_INPUTS(1)
INSTANTIATE_TYPES_FOR_N_INPUTS(2)
INSTANTIATE_TYPES_FOR_N_INPUTS(3)
