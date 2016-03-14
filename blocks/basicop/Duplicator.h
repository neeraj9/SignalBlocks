// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#ifndef SIGNALBLOCKS_DUPLICATOR_H
#define SIGNALBLOCKS_DUPLICATOR_H

#include "../../common/Port.h"

#include <algorithm>
#include <assert.h>
#include <memory>

namespace sigblocks {
    template<int M, class T>
    class Duplicator
            : public Port<1, M, T> {
    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            for (int i = 0; i < M; ++i) {
                this->LeakData(i, data, startTime);
            }
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex == 0); // XXX change to an assertion library.
            for (int i = 0; i < (M - 1); ++i) {
                std::unique_ptr<T[]> datacopy(new T[len]);
                // The std::copy() will not work with complex data structure (T), where
                // shallow-copy is an issue and hence error prone. Instead rely on
                // assignment operator, which should be available for type T.
                //std::copy(data.get(), &(data.get()[len]), datacopy.get());
                for (int j = 0; j < len; ++j) {
                    datacopy.get()[j] = data.get()[j];
                }
                this->LeakData(i, std::move(datacopy), len, startTime);
            }
            constexpr int index = M - 1;
            if (index >= 0) {
                this->LeakData(index, std::move(data), len, startTime);
            }
        }
    };
}

#endif //SIGNALBLOCKS_DUPLICATOR_H
