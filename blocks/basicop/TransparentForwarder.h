// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#ifndef SIGNALBLOCKS_TRANSPARENTFORWARDER_H
#define SIGNALBLOCKS_TRANSPARENTFORWARDER_H

#include "../../common/Port.hh"

namespace sigblocks {
    template<int M, class T>
    class TransparentForwarder
            : public Port<1, M, T> {
    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            for (int i = 0; i < M; ++i) {
                this->LeakData(i, data, startTime);
            }
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            for (int i = 0; i < M; ++i) {
                this->LeakData(i, std::move(data), len, startTime);
            }
        }
    };
}

#endif //SIGNALBLOCKS_TRANSPARENTFORWARDER_H
