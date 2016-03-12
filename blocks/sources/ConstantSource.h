// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_CONSTANTSOURCE_H
#define SIGBLOCKS_CONSTANTSOURCE_H

#include "../../common/Port.h"
#include "../../common/TimeTick.h"

namespace sigblocks {
    template<class T>
    class ConstantSource
            : public Port<0, 1, T> {
    public:
        ConstantSource(const T& value);

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        const T mFixedValue;
    };
}

#endif // SIGBLOCKS_CONSTANTSOURCE_H
