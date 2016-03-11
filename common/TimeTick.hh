// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_TimeTick_hh
#define sigblocks_common_TimeTick_hh

namespace sigblocks
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

#endif // sigblocks_common_TimeTick_hh
