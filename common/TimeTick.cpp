// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "TimeTick.h"

#include <cmath>
#include <limits>

using namespace sigblocks;

TimeTick::TimeTick()
        : mValue(0) {
}

TimeTick::TimeTick(TimeTickType value)
        : mValue(value) {
}

TimeTick::TimeTick(const TimeTick& copy)
        : mValue(copy.mValue) {
}

TimeTickType
TimeTick::GetValue() const {
    return mValue;
}

bool TimeTick::IsLess(const TimeTick& rhs, TimeTickType threshold) const {
    if (!operator==(rhs)) {
        if (mValue > rhs.mValue) {
            TimeTickType from_max = std::numeric_limits<TimeTickType>::max() - mValue;
            return (from_max <= threshold);
        } else {
            return true;
        }
    }
    return false;
}

bool TimeTick::IsGreater(const TimeTick& rhs, TimeTickType threshold) const {
    return rhs.IsLess(*this, threshold);
}

const TimeTick&
TimeTick::operator+=(const TimeTick& rhs) {
    mValue += rhs.mValue;
    return *this;
}

bool
TimeTick::operator!=(const TimeTick& rhs) const {
    return (mValue != rhs.mValue);
}

bool
TimeTick::operator==(const TimeTick& rhs) const {
    return (mValue == rhs.mValue);
}

bool
TimeTick::operator<(const TimeTick& rhs) const {
    if (!operator==(rhs)) {
        return (mValue < rhs.mValue);
    }
    return false;
}

bool
TimeTick::operator>(const TimeTick& rhs) const {
    if (!operator==(rhs)) {
        return (mValue > rhs.mValue);
    }
    return false;
}
