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

#ifndef SIGNALBLOCKS_BINARYOPERATOR_H
#define SIGNALBLOCKS_BINARYOPERATOR_H

#include "../../common/Port.h"

#include <memory>
#include <vector>

namespace signalblocks {
    template<class T>
    class BinaryOperator
            : public Port<2, 1, T> {
    public:
        BinaryOperator(std::string name)
                : Port<2, 1, T>(std::move(name), "A binary operator block."),
                  mDefaultValue(),
                  mStorage({mDefaultValue, mDefaultValue}),
                  mArrayStorage({nullptr, nullptr}),
                  mDims(),
                  mLastTick(),
                  mNumInputsReceived(0) {
        }

        BinaryOperator(std::string name, const T& defaultValue)
                : Port<2, 1, T>(std::move(name), "A binary operator block."),
                  mDefaultValue(defaultValue),
                  mStorage({defaultValue, defaultValue}),
                  mArrayStorage({nullptr, nullptr}),
                  mDims(),
                  mLastTick(),
                  mNumInputsReceived(0) {
        }
    protected:
        virtual T ComputeScalar(const T& arg1, const T& arg2) const = 0;

        // compute over vector and store the result in pArg1 (save unnecessary memory allocation
        virtual void ComputeVector(T* pArg1, T* pArg2, int len) = 0;

        // compute over matrix and store the result in pArg1 (save unnecessary memory allocation
        virtual void ComputeMatrix(T* pArg1, T* pArg2, const std::vector<int>& dims) = 0;

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < 2); // XXX change to an assertion library.
            assert(mDims.empty());
            if (mLastTick == startTime) {
                mStorage[sourceIndex] = data;
                ++mNumInputsReceived;
            }

            if ((mNumInputsReceived == 2) || ((mNumInputsReceived > 0) && (mLastTick != startTime))) {
                // time to push out the data
                T output = this->ComputeScalar(mStorage[0], mStorage[1]);
                LeakData(0, output, mLastTick);
                mNumInputsReceived = 0;
                mStorage[0] = mDefaultValue;
                mStorage[1] = mDefaultValue;
            }
            if (mLastTick != startTime) {
                mLastTick = startTime;
                mStorage[sourceIndex] = data;
            }
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < 2); // XXX change to an assertion library.
            assert(mNumInputsReceived == 0);
            if (! mDims.empty()) {
                mDims.push_back(len);
            } else {
                assert(len == mDims[0]);
                assert(mDims.size() == 1);
            }
            if (mLastTick == startTime) {
                mArrayStorage[sourceIndex].swap(data);
            }
            if (((mArrayStorage[0]) && (mArrayStorage[1])) || ((mArrayStorage[0] || mArrayStorage[1]) && (mLastTick != startTime))) {
                // done, so transmit now
                this->ComputeVector(mArrayStorage[0].get(), mArrayStorage[1].get(), len);
                this->LeakData(0, std::move(mArrayStorage[0]), len, mLastTick);
                mArrayStorage[0].reset(nullptr);
                mArrayStorage[1].reset(nullptr);
                mDims.clear();
            }
            if (mLastTick != startTime) {
                mLastTick = startTime;
                mArrayStorage[sourceIndex].swap(data);
            }
        }

        virtual void ProcessMatrix(
                int sourceIndex, std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < 2); // XXX change to an assertion library.
            assert(mNumInputsReceived == 0);
            if (! mDims.empty()) {
                mDims = dims;
            } else {
                assert(dims == mDims);
            }
            if (mLastTick == startTime) {
                mArrayStorage[sourceIndex].swap(data);
            }
            if (((mArrayStorage[0]) && (mArrayStorage[1])) || ((mArrayStorage[0] || mArrayStorage[1]) && (mLastTick != startTime))) {
                // done, so transmit now
                this->ComputeMatrix(mArrayStorage[0].get(), mArrayStorage[1].get(), dims);
                this->LeakMatrix(0, std::move(mArrayStorage[0]), dims, mLastTick);
                mArrayStorage[0].reset(nullptr);
                mArrayStorage[1].reset(nullptr);
                mDims.clear();
            }
            if (mLastTick != startTime) {
                mLastTick = startTime;
                mArrayStorage[sourceIndex].swap(data);
            }
        }

    private:
        T mDefaultValue;
        T mStorage[2];  ///< for scalar types
        std::unique_ptr<T[]> mArrayStorage[2]; ///< for vector and matrix types
        std::vector<int> mDims;  ///< for vector and matrix types
        TimeTick mLastTick;
        int mNumInputsReceived;
    };
}

#endif // SIGNALBLOCKS_BINARYOPERATOR_H
