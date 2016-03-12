// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_STDOUTSINK_H
#define SIGBLOCKS_STDOUTSINK_H

#include "../../common/Port.h"

namespace sigblocks {
    template<class T>
    class StdoutSink
            : public Port<1, 0, T> {
    public: // Port interface
        void Process(int sourceIndex, const T& data, const TimeTick& startTime);

        void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime);
    };
}

#endif // SIGBLOCKS_STDOUTSINK_H
