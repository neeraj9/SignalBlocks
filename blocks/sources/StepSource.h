// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_STEPSOURCE_H
#define SIGBLOCKS_STEPSOURCE_H

#include "../../common/Port.h"
#include "../../common/TimeTick.h"

namespace sigblocks {
    template<class T>
    class StepSource
            : public Port<0, 1, T> {
    public:
        StepSource(std::string name, const T& value);

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        TimeTick mLastTick;
        T mValue;
    };
}

#endif // SIGBLOCKS_STEPSOURCE_H
