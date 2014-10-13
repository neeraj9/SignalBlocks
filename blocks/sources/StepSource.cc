// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "StepSource.hh"

using namespace sigblocks;

template <class T>
StepSource<T>::StepSource(
  const TimeTick& startTime, const TimeTick& increment, const T& value)
  : mTime(startTime),
    mIncrement(increment),
    mValue(value)
{
}

template <class T>
void
StepSource<T>::Generate()
{
  this->LeakData(0, mValue, mTime);
  mTime += mIncrement;
  ++mValue;
}

template class StepSource<long>;
template class StepSource<unsigned long>;
template class StepSource<int>;
template class StepSource<unsigned int>;
template class StepSource<char>;
template class StepSource<unsigned char>;
template class StepSource<float>;
template class StepSource<double>;
