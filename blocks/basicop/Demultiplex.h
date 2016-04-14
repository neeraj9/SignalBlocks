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
        Demultiplex() : mNextOutputPort(0) {
            // need to set mIsVectorEnabled to false (XXX)
        }

        virtual ~Demultiplex() {
            // Need for vector buffer when vector support is added (XXX)
        }

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < N); // XXX change to an assertion library.

            this->LeakData(mNextOutputPort, data, startTime);

            mNextOutputPort = (mNextOutputPort + 1) % N;
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < N); // XXX change to an assertion library.

            // Mutiple inputs are treated as one when passing downstream
            // use some other class to split such a data into individual data
            // items if those need to be demultiplexed and then apply this module.
            this->LeakData(mNextOutputPort, std::move(data), len, startTime);

            mNextOutputPort = (mNextOutputPort + 1) % N;
        }

        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<T[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < N); // XXX change to an assertion library.

            // Mutiple inputs are treated as one when passing downstream
            // use some other class to split such a data into individual data
            // items if those need to be demultiplexed and then apply this module.
            this->LeakMatrix(mNextOutputPort, std::move(data), dims, startTime);

            mNextOutputPort = (mNextOutputPort + 1) % N;
        }

    private:
        int mNextOutputPort;
    };
}

#endif // SIGBLOCKS_DEMULTIPLEX_H
