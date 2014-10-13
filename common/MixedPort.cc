// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
// copyright (c) 2010-12 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE.txt for license information
#include "MixedPort.hh"

#include <iostream>
using namespace std;

using namespace sigblocks;

template <int N, int M, class TN, class TM>
MixedPort<N,M,TN,TM>::MixedPort()
  : mMutex(),
    mpSource(),
    mpSink()
{
}

template <int N, int M, class TN, class TM>
MixedPort<N,M,TN,TM>::~MixedPort()
{
  for (int i = 0; i < M; ++i)
  {
    if (mpSink[i])
    {
      mpSink[i]->DisconnectSource(this);
    }
  }
}

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::SetSource(IPort<TN>* peer, int index)
{
  BoostPort::Lock lock(mMutex); (void)lock;
  assert(index < N); // XXX replace assert with better error handling
  mpSource[index] = peer;
}

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::SetSink(BoostPort::SharedPtr<IPort<TM> >& peer, int index)
{
  BoostPort::Lock lock(mMutex); (void)lock;
  assert(index < M);
  mpSink[index] = peer;
}

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::DisconnectSource(IPort<TN>* peer)
{
  for (int i = 0; i < N; ++i)
  {
    if (mpSource[i] == peer)
    {
      mpSource[i] = 0;
    }
  }
}

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::ConsumeData(const IPort<TN> * pSender,
                                  const TN& data,
                                  const TimeTick& startTime)
{
  for (int i = 0; i < N; ++i)
  {
    if (mpSource[i] == pSender)
    {
      Process(i, data, startTime);
      break;
    }
  }
}

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::ConsumeData(const IPort<TN> * pSender,
                                  MultiPtr<TN> pData,
                                  int len,
                                  const TimeTick& startTime)
{
  for (int i = 0; i < N; ++i)
  {
    if (mpSource[i] == pSender)
    {
      Process(i, pData, len, startTime);
      break;
    }
  }
}

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::LeakData(int index,
                               const TM& data,
                               const TimeTick& startTime)
{
  assert(index < M);
  assert(0 <= index);
  if (mpSink[index])
  {
    mpSink[index]->ConsumeData(this, data, startTime);
  }
}

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::LeakData(int index,
                               MultiPtr<TM> pData,
                               int len,
                               const TimeTick& startTime)
{
  assert(index < M);
  assert(0 <= index);
  if (mpSink[index])
  {
    mpSink[index]->ConsumeData(this, pData, len, startTime);
  }
}

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::Process(int sourceIndex,
                              const TN& data,
                              const TimeTick& startTime)
{
  // silently drop
}

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::Process(int sourceIndex,
                              MultiPtr<TN> pData,
                              int len,
                              const TimeTick& startTime)
{
  // silently drop
}

//
// interface which are intentionally private to disable use
//

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::SetSource(IPort<TM>* peer, int index)
{
}

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::SetSink(BoostPort::SharedPtr<IPort<TN> >& peer, int index)
{
}

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::DisconnectSource(IPort<TM>* peer)
{
}

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::ConsumeData(const IPort<TM> * pSender,
                                  const TM& data,
                                  const TimeTick& startTime)
{
}

template <int N, int M, class TN, class TM>
void
MixedPort<N,M,TN,TM>::ConsumeData(const IPort<TM> * pSender,
                                  MultiPtr<TM> pData,
                                  int len,
                                  const TimeTick& startTime)
{
}

////////////////////////////////////////////////////////////

#define INSTANTIATE_TEMPLATE(fromtype, totype) \
  template class MixedPort<1, 1, fromtype, totype>;

// Instantiate so that they can be used elsewhere
//INSTANTIATE_TEMPLATE(char, short)
//INSTANTIATE_TEMPLATE(short, int)
//INSTANTIATE_TEMPLATE(int, long)
//INSTANTIATE_TEMPLATE(unsigned char, unsigned short)
//INSTANTIATE_TEMPLATE(unsigned short, unsigned int)
//INSTANTIATE_TEMPLATE(unsigned int, unsigned long)
INSTANTIATE_TEMPLATE(int, float)
INSTANTIATE_TEMPLATE(int, double)
//INSTANTIATE_TEMPLATE(float, double)
