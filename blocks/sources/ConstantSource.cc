// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "ConstantSource.hh"

using namespace sigblocks;

template <class T>
ConstantSource<T>::ConstantSource(
  const TimeTick& startTime, const TimeTick& increment, const T& value)
  : mTime(startTime),
    mIncrement(increment),
    mFixedValue(value)
{
}

template <class T>
void
ConstantSource<T>::Generate()
{
  this->LeakData(0, mFixedValue, mTime);
  mTime += mIncrement;
}

template class ConstantSource<long>;
template class ConstantSource<unsigned long>;
template class ConstantSource<int>;
template class ConstantSource<unsigned int>;
template class ConstantSource<char>;
template class ConstantSource<unsigned char>;
template class ConstantSource<float>;
template class ConstantSource<double>;
