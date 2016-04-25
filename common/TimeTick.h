// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_TIMETICK_H
#define SIGBLOCKS_TIMETICK_H

#include <stdint.h>
#include <limits>

namespace sigblocks {
    typedef uint64_t TimeTickType;

    class TimeTick {
    public:

        /// minimum increment value
        static const TimeTickType MIN_INCREMENT = 1;

        /// The default wraparound threshold is 5% of the maximum possible value.
        static const TimeTickType DEFAULT_WRAPAROUND_THRESHOLD =
                static_cast<const TimeTickType>(std::numeric_limits<TimeTickType>::max() * 0.05);

        TimeTick();
        TimeTick(TimeTickType value);

        TimeTick(const TimeTick& copy);

        TimeTickType GetValue() const;

        /// Is this less than rhs, but within the threshold.
        bool IsLess(const TimeTick& rhs,
                    TimeTickType threshold = DEFAULT_WRAPAROUND_THRESHOLD) const;

        /// Is this greater than rhs, but within the threshold.
        bool IsGreater(const TimeTick& rhs,
                       TimeTickType threshold = DEFAULT_WRAPAROUND_THRESHOLD) const;

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
