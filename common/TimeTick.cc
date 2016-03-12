// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "TimeTick.hh"

#include <cmath>

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
    return (!operator<(rhs));
}
