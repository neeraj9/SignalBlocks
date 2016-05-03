// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// This file is part of SignalBlocks.
//
// SignalBlocks is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SignalBlocks is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SignalBlocks.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SIGBLOCKS_SPLITTER_H
#define SIGBLOCKS_SPLITTER_H

#include "../../common/Port.h"

#include <assert.h>
#include <memory>

namespace signalblocks {
    /** Splitter Splits an input into multiple outputs based on columns.
     * The Splitter block splits an input received from a
     * single source into multiple outputs, which are then
     * subsequently sent to different sinks.
     *
     * When the data received is scalar, then this block sends
     * it (scalar value) to the first sink (index-0).
     *
     * When the data received is vector (M values per vector), then its sent to
     * M output blocks as a scalar values.
     *
     * When the data is a matrix then the number of output port is matched with
     * the last dimension, so the data separation happens only for the
     * last dimension. Its easier to explain when there are only 2 dimensions,
     * although this logic applies for multiple dimensions.
     * For example the transformation will happen as follows:
     *
     *                                     +----> d-mcol0 {10, 4} (port-0)
     * d {10, 4, 2} ----> splitter<2, T> --+
     *                                     +----> d-mcol1 {10, 4} (port-1)
     *
     * Another example, but this time the user wants only one output:
     *
     *                                     +----> d-mcol0 {10, 5}
     * d {10, 5, 2} ----> splitter<1, T> --+
     *                                     +----- dropped
     *
     * Another example, but this time user selectively chooses only
     * few of the items from the last dimension.
     *
     *                                            +----> d-mcol0 {10, 3}
     *                                            |
     *                                            +----- dropped
     *                                            |
     *                                            +----- dropped
     * d {10, 3, 5} ----> splitter<2, T>({0,4}) --+
     *                                            +----- dropped
     *                                            |
     *                                            +----> d-mcol4 {10, 3}
     *
     * Note that all the outputs are matrix, which is same as the first
     * two dimensions.
     * There is an exception to this rule, wherein when the
     * incoming data is a matrix of single dimension (which is odd
     * because then it should have been passed as a vector), but
     * for that case a scalar is produced as an output. See below:
     *
     *                                     +----> d-col3 scalar (port-0)
     * d {10} --> splitter<2, T>({3, 2}) --+
     *                                     +----> d-col2 scalar (port-1)
     *
     * Notice that the index can be in any order (3 and 2 are not sorted)
     * and the same will be sent out.
     *
     * There is another exception wherein when there are only 2 dimensions
     * of the input data then the output is a vector (see below).
     *
     *
     *                                       +----> d-col3 vector [10] (port-0)
     * d {10,4} --> splitter<2, T>({3, 2}) --+
     *                                       +----> d-col2 vector [10] (port-1)
     *
     *
     * There is another example, where the input matrix has lesser
     * number in its last dimension. In such a case default value
     * is used to generate data. See the example below for details.
     *
     *                                         +----> d-col0 vector [10, 1]
     *                                         |
     *                                         +----- dropped
     *                                         |
     *                                         +----- dropped (but not present)
     * d {10, 2} ----> splitter<2, T>({0,4}) --+
     *                                         +----- dropped (but not present)
     *                                         |
     *                                         +----> default-value vector [10]
     *
     */
    template<int M, class T>
    class Splitter
            : public Port<1, M, T> {
    public:
        Splitter(std::string name, T defaultValue)
                : Port<1, M, T>(std::move(name), "A Splitter block."),
                  mIndices(),
                  mDefaultValue(std::move(defaultValue)) {
            // split all values so indices are [0, M)
            mIndices.reserve(M);
            for (size_t i = 0; i < M; ++i) {
                mIndices.push_back(i);
            }
        }

        Splitter(std::string name, T defaultValue, std::vector<int> indices)
                : Port<1, M, T>(std::move(name), "A Splitter block."),
                  mIndices(std::move(indices)),
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
            for (int i = 0; i < static_cast<int>(mIndices.size()); ++i) {
                if (mIndices[i] < len) {
                    // forward
                    //std::unique_ptr<T[]> tmp(new T[1]);
                    // swap to avoid copy whenever the data structure of type T allows it
                    //std::swap(tmp[0], raw_data[mIndices[i]]);
                    //this->LeakData(i, std::move(tmp), 1, startTime);
                    this->LeakData(i, raw_data[mIndices[i]], startTime);
                } else {
                    this->LeakData(i, mDefaultValue, startTime);
                }
            }
        }

        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<T[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime) {
            assert(sourceIndex == 0);
            assert(! dims.empty());

            // Matrix are stored in row-major format. See IPort.h for details.
            int len = 1;
            //printf("dims=[");
            for (size_t i = 0; i < (dims.size() - 1); ++i) {
                //printf("%d, ", dims[i]);
                len *= dims[i];
            }
            //printf("%d]\n", dims[dims.size()-1]);
            assert(len > 0);
            T* raw_data = data.get();
            //printf("dims.size()=%lu, len=%d, mIndices.size()=%lu\n", dims.size(), len, mIndices.size());
            // unfold the last dimension
            if (dims.size() == 1) {
                // extract vector into its raw form and then dispatch each one
                // as std::unique_ptr
                for (int i = 0; i < static_cast<int>(mIndices.size()); ++i) {
                    if (mIndices[i] < dims.back()) {
                        // leak scalar
                        this->LeakData(i, raw_data[mIndices[i]], startTime);
                    } else {
                        this->LeakData(i, mDefaultValue, startTime);
                    }
                }
            } else if (dims.size() == 2) {
                for (int i = 0; i < static_cast<int>(mIndices.size()); ++i) {
                    std::unique_ptr<T[]> tmpdata(new T[len]);
                    if (mIndices[i] < dims.back()) {
                        for (int j = 0; j < len; ++j) {
                                tmpdata.get()[j] = raw_data[(j * dims.back()) + mIndices[i]];
                        }
                    } else {
                        for (int j = 0; j < len; ++j) {
                            tmpdata.get()[j] = mDefaultValue;
                        }
                    }
                    // leak vector
                    this->LeakData(i, std::move(tmpdata), len, startTime);
                }
            } else {
                // dont include the last dimension, because that is serialized or unfolded
                std::vector<int> reduced_dims(dims.begin(), dims.end() - 1);
                for (int i = 0; i < static_cast<int>(mIndices.size()); ++i) {
                    std::unique_ptr<T[]> tmpdata(new T[len]);
                    if (mIndices[i] < dims.back()) {
                        for (int j = 0; j < len; ++j) {
                            // use assignment operator to take care of complex data type T
                            // For optimisation use std::copy() for native
                            // data type (for example int, long, etc).
                            tmpdata.get()[j] = raw_data[(j * dims.back()) + mIndices[i]];
                        }
                    } else {
                        for (int j = 0; j < len; ++j) {
                            tmpdata.get()[j] = mDefaultValue;
                        }
                    }
                    this->LeakMatrix(i, std::move(tmpdata), reduced_dims, startTime);
                }
            }
        }

    private:
        std::vector<int> mIndices;
        T mDefaultValue;
    };
}

#endif //SIGBLOCKS_SPLITTER_H
