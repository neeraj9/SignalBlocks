// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "StepSource.h"

using namespace sigblocks;

template<class T>
StepSource<T>::StepSource(const T& value)
        : mLastTick(),
          mValue(value) {
}

template<class T>
void
StepSource<T>::ClockCycle(const TimeTick& timeTick) {
    if (mLastTick == timeTick) {
        return;  // already processed the event
    }
    mLastTick = timeTick;
    this->LeakData(0, mValue, timeTick);
    ++mValue;
}

template
class StepSource<long>;

template
class StepSource<unsigned long>;

template
class StepSource<int>;

template
class StepSource<unsigned int>;

template
class StepSource<char>;

template
class StepSource<unsigned char>;

template
class StepSource<float>;

template
class StepSource<double>;
