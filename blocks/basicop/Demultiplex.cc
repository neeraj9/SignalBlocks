// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include <assert.h>
#include "Demultiplex.hh"

using namespace sigblocks;
using namespace std;

template<int N, class T>
Demultiplex<N, T>::Demultiplex()
        : mNextOutputPort(0) {
}

template<int N, class T>
Demultiplex<N, T>::~Demultiplex() {
}

template<int N, class T>
void
Demultiplex<N, T>::Process(int sourceIndex, const T& data, const TimeTick& startTime) {
    assert(sourceIndex >= 0 || sourceIndex < N); // XXX change to an assertion library.

    this->LeakData(mNextOutputPort, data, startTime);

    mNextOutputPort = (mNextOutputPort + 1) % N;
}

template<int N, class T>
void
Demultiplex<N, T>::Process(
        int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
    assert(sourceIndex >= 0 || sourceIndex < N); // XXX change to an assertion library.

    // Mutiple inputs are treated as one when passing downstream
    // use some other class to split such a data into individual data
    // items if those need to be demultiplexed and then apply this module.
    this->LeakData(mNextOutputPort, std::move(data), len, startTime);

    mNextOutputPort = (mNextOutputPort + 1) % N;
}

#define INSTANTIATE_TYPES_FOR_N_INPUTS(NUM) \
  template class sigblocks::Demultiplex<NUM, long>; \
  template class sigblocks::Demultiplex<NUM, unsigned long>; \
  template class sigblocks::Demultiplex<NUM, int>; \
  template class sigblocks::Demultiplex<NUM, unsigned int>; \
  template class sigblocks::Demultiplex<NUM, unsigned char>; \
  template class sigblocks::Demultiplex<NUM, float>; \
  template class sigblocks::Demultiplex<NUM, double>;

// XXX allow max of two inputs for now.
INSTANTIATE_TYPES_FOR_N_INPUTS(1)

INSTANTIATE_TYPES_FOR_N_INPUTS(2)

INSTANTIATE_TYPES_FOR_N_INPUTS(3)
