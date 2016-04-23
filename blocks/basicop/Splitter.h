//
// Created by nsharma on 3/13/16.
//

#ifndef SIGBLOCKS_SPLITTER_H
#define SIGBLOCKS_SPLITTER_H

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
                if (mIndices[i] < len) {
                    // forward
                    //std::unique_ptr<T[]> tmp(new T[1]);
                    // swap to avoid copy whenever the data structure of type T allows it
                    //std::swap(tmp[0], raw_data[mIndices[i]]);
                    //this->LeakData(i, std::move(tmp), 1, startTime);
                    this->LeakData(i, raw_data[mIndices[i]], startTime);
                }
            }
        }

        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<T[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime) {
            assert(sourceIndex == 0); // XXX change to an assertion library.

            assert(! dims.empty());

            // Matrix is stored as shown in the example below (see IPort doc as well).
            // Example (2 dimensions):
            // dims = {2, 4}
            // data = { {a0, a1},
            //          {a2, a3},
            //          {a4, a5},
            //          {a6, a7}
            //         }
            //
            // Another example (3 dimensions):
            // dims = {2, 3, 4}
            // data = { { {a0, a1}, {a2, a3}, {a4, a5} },
            //          { {a6, a7}, {a8, a9}, {a10, a11} },
            //          { {a12, a13}, {a14, a15}, {a16, a17} },
            //          { {a18, a19}, {a20, a21}, {a22, a23} }
            //        }
            int len = 1;
            for (size_t i = 0; i < (dims.size() - 1); ++i) {
                len *= dims[i];
            }
            assert(len > 0);
            T* raw_data = data.get();
            // unfold the last dimension
            if (dims.size() == 1) {
                // extract vector into its raw form and then dispatch each one
                // as std::unique_ptr
                for (size_t i = 0; i < mIndices.size(); ++i) {
                    if (mIndices[i] < dims.back()) {
                        // leak scalar
                        this->LeakData(i, raw_data[mIndices[i]], startTime);
                    }
                }
            } else if (dims.size() == 2) {
                for (size_t i = 0; i < mIndices.size(); ++i) {
                    if (mIndices[i] < dims.back()) {
                        std::unique_ptr<T[]> tmpdata(new T[len]);
                        for (int j = 0; j < len; ++j) {
                            tmpdata.get()[j] = raw_data[mIndices[i] * len + j];
                        }
                        // leak vector
                        this->LeakData(i, std::move(tmpdata), len, startTime);
                    }
                }
            } else {
                // dont include the last dimension, because that is serialized or unfolded
                std::vector<int> reduced_dims(dims.begin(), dims.end() - 1);
                for (size_t i = 0; i < mIndices.size(); ++i) {
                    if (mIndices[i] < dims.back()) {
                        std::unique_ptr<T[]> tmpdata(new T[len]);
                        // use assignment operator to take care of complex data type T
                        // For optimisation use std::copy() for native
                        // data type (for example int, long, etc).
                        for (int j = 0; j < len; ++j) {
                            tmpdata.get()[j] = raw_data[mIndices[i] * len + j];
                        }
                        this->LeakMatrix(i, std::move(tmpdata), reduced_dims, startTime);
                    }
                }
            }
        }

    private:
        std::vector<int> mIndices;
        T mDefaultValue;
    };
}

#endif //SIGBLOCKS_SPLITTER_H
