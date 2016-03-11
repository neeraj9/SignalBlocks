// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "TimeTick.hh"

#include <cmath>

using namespace sigblocks;

const float EPSILON_THRESHOLD = 0.000000001; // XXX pick from global config

TimeTick::TimeTick(TimeTickType value)
  : mValue(value)
{
}

TimeTick::TimeTick(const TimeTick& copy)
  : mValue(copy.mValue)
{
}

TimeTickType
TimeTick::GetValue() const
{
  return mValue;
}

// XXX Note that the increment should be done carefully
//     because after a long time the value will overflow
//     and needs to be wrapped around to zero.
const TimeTick&
TimeTick::operator +=(const TimeTick& rhs)
{
  mValue += rhs.mValue;
  return *this;
}

bool
TimeTick::operator !=(const TimeTick& rhs) const
{
  return (mValue != rhs.mValue);
}

bool
TimeTick::operator ==(const TimeTick& rhs) const
{
  return (fabs((mValue - rhs.mValue) < EPSILON_THRESHOLD));
}

// XXX Note that less-than property should be carefully implemented
//     because after a long time the time value will wrap around
//     and then less-than needs to account for that.
// see http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
// for Comparing floating point numbers
bool
TimeTick::operator <(const TimeTick& rhs) const
{
  if (! operator==(rhs))
  {
    return ((rhs.mValue - mValue) > EPSILON_THRESHOLD);
  }
  return false;
}

bool
TimeTick::operator >(const TimeTick& rhs) const
{
  return (! operator<(rhs));
}
