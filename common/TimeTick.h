// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_TIMETICK_H
#define SIGBLOCKS_TIMETICK_H

#include <stdint.h>

namespace sigblocks {
    typedef uint64_t TimeTickType;

    class TimeTick {
    public:

        /// minimum increment value
        static const TimeTickType MIN_INCREMENT = 1;

        TimeTick();
        TimeTick(TimeTickType value);

        TimeTick(const TimeTick& copy);

        TimeTickType GetValue() const;

        const TimeTick& operator+=(const TimeTick& rhs);

        bool operator!=(const TimeTick& rhs) const;

        bool operator==(const TimeTick& rhs) const;

        bool operator<(const TimeTick& rhs) const;

        bool operator>(const TimeTick& rhs) const;

    private:
        TimeTickType mValue;
    };
}

#endif // SIGBLOCKS_TIMETICK_H
