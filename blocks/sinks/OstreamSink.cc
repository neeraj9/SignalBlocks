// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "OstreamSink.hh"

#include "../../common/MultiPtr.hh"

#include <fstream>
#include <iostream>

using namespace sigblocks;
using namespace std;

template <class T>
OstreamSink<T>::OstreamSink()
  : mpOstream(0)
{
}

template <class T>
OstreamSink<T>::OstreamSink(std::auto_ptr<std::ostream> pOuts)
  : mpOstream(pOuts)
{
}

template <class T>
void
OstreamSink<T>::SetStreamSink(std::auto_ptr<std::ostream> pOuts)
{
  mpOstream = pOuts;
}

template <class T>
void
OstreamSink<T>::Process(int sourceIndex, const T& data, const TimeTick& startTime)
{
  mpOstream->write(reinterpret_cast<const char*>(&data), sizeof(T));
}
  
template <class T>
void
OstreamSink<T>::Process(
  int sourceIndex, MultiPtr<T> pData, int len, const TimeTick& startTime)
{
  mpOstream->write(reinterpret_cast<const char*>(pData.get()), len * sizeof(T));
}

template class OstreamSink<unsigned char>;
template class OstreamSink<char>;
template class OstreamSink<short>;
template class OstreamSink<int>;
template class OstreamSink<long>;
