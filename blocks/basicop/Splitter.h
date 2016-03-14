//
// Created by nsharma on 3/13/16.
//

#ifndef SIGNALBLOCKS_SPLITTER_H
#define SIGNALBLOCKS_SPLITTER_H

#include "../../common/Port.h"

#include <assert.h>
#include <memory>

namespace sigblocks {
    /** Splitter Splits an input into multiple outputs.
     * The Splitter block splits an input received from a
     * single source into multiple outputs, which are then
     * subsequently sent to different sinks.
     *
     * When the data received is scalar, then this block sends
     * it (scalar value) to the first sink (index-0).
     *
     * When the data received is vector (M dimension), then its sent to
     * M output blocks as a scalar values.
     */
    template<int M, class T>
    class Splitter
            : public Port<1, M, T> {
    public:
        Splitter(T defaultValue)
                : mIndices(),
                  mDefaultValue(std::move(defaultValue)) {
            // split all values so indices are [0, M)
            mIndices.reserve(M);
            for (size_t i = 0; i < M; ++i) {
                mIndices.push_back(i);
            }
        }

        Splitter(T defaultValue, std::vector<int> indices)
                : mIndices(std::move(indices)),
                  mDefaultValue(std::move(defaultValue)) {
            assert(mIndices.size() == M);
        }

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            // when scalar values are received then its sent on first output line only
            this->LeakData(0, data, startTime);
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex == 0); // XXX change to an assertion library.
            // extract vector into its raw form and then dispatch each one
            // as std::unique_ptr
            T* raw_data = data.get();
            for (size_t i = 0; i < mIndices.size(); ++i) {
                if (i < static_cast<size_t>(len)) {
                    // forward
                    //std::unique_ptr<T[]> tmp(new T[1]);
                    // swap to avoid copy whenever the data structure of type T allows it
                    //std::swap(tmp[0], raw_data[mIndices[i]]);
                    //this->LeakData(i, std::move(tmp), 1, startTime);
                    this->LeakData(i, raw_data[mIndices[i]], startTime);
                }
            }
        }

    private:
        std::vector<int> mIndices;
        T mDefaultValue;
    };
}

#endif //SIGNALBLOCKS_SPLITTER_H
