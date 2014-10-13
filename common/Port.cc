// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
// copyright (c) 2010-12 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE.txt for license information
#include "Port.hh"

#include <iostream>
using namespace std;

using namespace sigblocks;

template <int N, int M, class T>
Port<N,M,T>::Port()
  : mMutex(),
    mpSource(),
    mpSink()
{
}

template <int N, int M, class T>
Port<N,M,T>::~Port()
{
  for (int i = 0; i < M; ++i)
  {
    if (mpSink[i])
    {
      mpSink[i]->DisconnectSource(this);
    }
  }
}

template <int N, int M, class T>
void
Port<N,M,T>::SetSource(IPort<T>* peer, int index)
{
  BoostPort::Lock lock(mMutex); (void)lock;
  assert(index < N); // XXX replace assert with better error handling
  mpSource[index] = peer;
}

template <int N, int M, class T>
void
Port<N,M,T>::SetSink(BoostPort::SharedPtr<IPort<T> >& peer, int index)
{
  BoostPort::Lock lock(mMutex); (void)lock;
  assert(index < M);
  mpSink[index] = peer;
}

template <int N, int M, class T>
void
Port<N,M,T>::DisconnectSource(IPort<T>* peer)
{
  for (int i = 0; i < N; ++i)
  {
    if (mpSource[i] == peer)
    {
      mpSource[i] = 0;
    }
  }
}

template <int N, int M, class T>
void
Port<N,M,T>::ConsumeData(const IPort<T> * pSender,
                         const T& data,
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

template <int N, int M, class T>
void
Port<N,M,T>::ConsumeData(const IPort<T> * pSender,
                         MultiPtr<T> pData,
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

template <int N, int M, class T>
void
Port<N,M,T>::LeakData(int index,
                      const T& data,
                      const TimeTick& startTime)
{
  assert(index < M);
  assert(0 <= index);
  if (mpSink[index])
  {
    mpSink[index]->ConsumeData(this, data, startTime);
  }
}

template <int N, int M, class T>
void
Port<N,M,T>::LeakData(int index,
                      MultiPtr<T> pData,
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

template <int N, int M, class T>
void
Port<N,M,T>::Process(int sourceIndex,
                     const T& data,
                     const TimeTick& startTime)
{
  // silently drop
}

template <int N, int M, class T>
void
Port<N,M,T>::Process(int sourceIndex,
                     MultiPtr<T> pData,
                     int len,
                     const TimeTick& startTime)
{
  // silently drop
}

////////////////////////////////////////////////////////////
/// Partial Template Specialization for information source
////////////////////////////////////////////////////////////

template <int M, class T>
Port<0,M,T>::Port()
  : mMutex(),
    mpSink()
{
}

template <int M, class T>
Port<0,M,T>::~Port()
{
  for (int i = 0; i < M; ++i)
  {
    if (mpSink[i])
    {
      mpSink[i]->DisconnectSource(this);
    }
  }
}

template <int M, class T>
void
Port<0,M,T>::SetSource(IPort<T>* peer, int index)
{
}

template <int M, class T>
void
Port<0,M,T>::SetSink(BoostPort::SharedPtr<IPort<T> >& peer, int index)
{
  BoostPort::Lock lock(mMutex); (void)lock;
  assert(index < M);
  mpSink[index] = peer;
}

template <int M, class T>
void
Port<0,M,T>::DisconnectSource(IPort<T>* peer)
{
}

template <int M, class T>
void
Port<0,M,T>::ConsumeData(const IPort<T> * pSender,
                         const T& data,
                         const TimeTick& startTime)
{
}

template <int M, class T>
void
Port<0,M,T>::ConsumeData(const IPort<T> * pSender,
                         MultiPtr<T> pData,
                         int len,
                         const TimeTick& startTime)
{
}

template <int M, class T>
void
Port<0,M,T>::LeakData(int index,
                      const T& data,
                      const TimeTick& startTime)
{
  assert(index < M);
  assert(0 <= index);
  if (mpSink[index])
  {
    mpSink[index]->ConsumeData(this, data, startTime);
  }
}

template <int M, class T>
void
Port<0,M,T>::LeakData(int index,
                      MultiPtr<T> pData,
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

////////////////////////////////////////////////////////////
/// Partial Template Specialization for information sink
////////////////////////////////////////////////////////////

template <int N,class T>
Port<N,0,T>::Port()
  : mMutex(),
    mpSource()
{
}

template <int N, class T>
Port<N,0,T>::~Port()
{
}

template <int N,class T>
void
Port<N,0,T>::SetSource(IPort<T>* peer, int index)
{
  BoostPort::Lock lock(mMutex); (void)lock;
  assert(index < N); // XXX replace assert with better error handling
  mpSource[index] = peer;
}

template <int N,class T>
void
Port<N,0,T>::SetSink(BoostPort::SharedPtr<IPort<T> >& peer, int index)
{
}

template <int N, class T>
void
Port<N,0,T>::DisconnectSource(IPort<T>* peer)
{
  for (int i = 0; i < N; ++i)
  {
    if (mpSource[i] == peer)
    {
      mpSource[i] = 0;
    }
  }
}

template <int N,class T>
void
Port<N,0,T>::ConsumeData(const IPort<T> * pSender,
                         const T& data,
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

template <int N,class T>
void
Port<N,0,T>::ConsumeData(const IPort<T> * pSender,
                         MultiPtr<T> pData,
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

template <int N,class T>
void
Port<N,0,T>::Process(int sourceIndex,
                     const T& data,
                     const TimeTick& startTime)
{
  // silently drop
}

template <int N,class T>
void
Port<N,0,T>::Process(int sourceIndex,
                     MultiPtr<T> pData,
                     int len,
                     const TimeTick& startTime)
{
  // silently drop
}


////////////////////////////////////////////////////////////
/// Partial Template Specialization for null port
////////////////////////////////////////////////////////////

template <class T>
Port<0,0,T>::~Port()
{
}

template <class T>
void
Port<0,0,T>::SetSource(IPort<T>* peer, int index)
{
}

template <class T>
void
Port<0,0,T>::SetSink(BoostPort::SharedPtr<IPort<T> >& peer, int index)
{
}

template <class T>
void
Port<0,0,T>::DisconnectSource(IPort<T>* peer)
{
}

template <class T>
void
Port<0,0,T>::ConsumeData(const IPort<T> * pSender,
                         const T& data,
                         const TimeTick& startTime)
{
}

template <class T>
void
Port<0,0,T>::ConsumeData(const IPort<T> * pSender,
                         MultiPtr<T> pData,
                         int len,
                         const TimeTick& startTime)
{
}

////////////////////////////////////////////////////////////


#define INSTANTIATE_TEMPLATE_N_M(n, m, type) \
  template class Port<n, m, type>;

#define INSTANTIATE_TEMPLATE_N(n, type) \
  INSTANTIATE_TEMPLATE_N_M(n, 0, type) \
  INSTANTIATE_TEMPLATE_N_M(n, 1, type) \
  INSTANTIATE_TEMPLATE_N_M(n, 2, type) \

#define INSTANTIATE_TEMPLATE(type) \
  INSTANTIATE_TEMPLATE_N(0, type) \
  INSTANTIATE_TEMPLATE_N(1, type) \
  INSTANTIATE_TEMPLATE_N(2, type) \
  INSTANTIATE_TEMPLATE_N(3, type) \

// Instantiate so that they can be used elsewhere
INSTANTIATE_TEMPLATE(char)
INSTANTIATE_TEMPLATE(short)
INSTANTIATE_TEMPLATE(int)
INSTANTIATE_TEMPLATE(long)
INSTANTIATE_TEMPLATE(unsigned char)
INSTANTIATE_TEMPLATE(unsigned short)
INSTANTIATE_TEMPLATE(unsigned int)
INSTANTIATE_TEMPLATE(unsigned long)
INSTANTIATE_TEMPLATE(float)
INSTANTIATE_TEMPLATE(double)
