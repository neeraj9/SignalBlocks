// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_DEMULTIPLEX_H
#define SIGBLOCKS_DEMULTIPLEX_H

#include "../../common/Port.h"

#include <list>

namespace sigblocks {
    template<int N, class T>
    class Demultiplex
            : public Port<1, N, T> {
    public:
        Demultiplex(); // need to set mIsVectorEnabled to false (XXX)
        virtual ~Demultiplex(); // Need for vector buffer when vector support is added (XXX)

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime);

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime);

    private:
        int mNextOutputPort;
    };
}

#endif // SIGBLOCKS_DEMULTIPLEX_H
