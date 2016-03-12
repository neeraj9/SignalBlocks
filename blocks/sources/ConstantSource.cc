// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "ConstantSource.hh"

using namespace sigblocks;

template<class T>
ConstantSource<T>::ConstantSource(const T& value)
        : mFixedValue(value) {
}

template<class T>
void
ConstantSource<T>::ClockCycle(const TimeTick& timeTick) {
    this->LeakData(0, mFixedValue, timeTick);
}

template
class ConstantSource<long>;

template
class ConstantSource<unsigned long>;

template
class ConstantSource<int>;

template
class ConstantSource<unsigned int>;

template
class ConstantSource<char>;

template
class ConstantSource<unsigned char>;

template
class ConstantSource<float>;

template
class ConstantSource<double>;
