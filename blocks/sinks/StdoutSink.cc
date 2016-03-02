// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "StdoutSink.hh"

#include <iostream>
#include <assert.h>

using namespace sigblocks;
using namespace std;

template <class T>
void
StdoutSink<T>::Process(int sourceIndex, const T& data, const TimeTick& startTime)
{
  assert(sourceIndex == 0);
  cout << &dec << startTime.GetValue() << ": " <<
          &hex << data << &dec << endl;
          //&hex << static_cast<int>(data) << &dec << endl;
}

template <class T>
void
StdoutSink<T>::Process(
      int sourceIndex, MultiPtr<T> pData, int len, const TimeTick& startTime)
{
  assert(sourceIndex == 0);
  cout << &dec << startTime.GetValue() << ": " << &hex;
  for (int i = 0; i < len; ++i)
  {
    cout << (pData.get()[i]) << ", ";
    //cout << static_cast<unsigned long>(pData.get()[i]) << ", ";
  }
  cout << &dec << endl;
}

// instantiate usuals
template class StdoutSink<unsigned char>;
template class StdoutSink<short>;
template class StdoutSink<int>;
template class StdoutSink<long>;
template class StdoutSink<float>;
template class StdoutSink<double>;
