// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
// copyright (c) 2010-12 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE.txt for license information
#ifndef iblocks_common_TimeTick_hh
#define iblocks_common_TimeTick_hh

namespace iblocks
{
  typedef float TimeTickType;
  class TimeTick
  {
  public:
    TimeTick(TimeTickType value);
    TimeTick(const TimeTick& copy);
    TimeTickType GetValue() const;

    const TimeTick& operator +=(const TimeTick& rhs);
    bool operator !=(const TimeTick& rhs) const;
    bool operator ==(const TimeTick& rhs) const;
    bool operator <(const TimeTick& rhs) const;
    bool operator >(const TimeTick& rhs) const;

  private:
    TimeTickType mValue;
  };
}

#endif // iblocks_common_TimeTick_hh
