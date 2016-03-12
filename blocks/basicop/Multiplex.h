// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_MULTIPLEX_H
#define SIGBLOCKS_MULTIPLEX_H

#include "../../common/Port.h"

#include <list>

namespace sigblocks {
    template<int N, class T>
    class Multiplex
            : public Port<N, 1, T> {
    public:
        Multiplex(); // need to set mIsVectorEnabled to false
        virtual ~Multiplex(); // Need for vector buffer when vector support is added (XXX)

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime);

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime);

    private:
        // scalar input buffer
        std::list<T> mDataPort[N];
        std::list<TimeTick> mDataPortTime[N];
    };
}

#endif // SIGBLOCKS_MULTIPLEX_H
