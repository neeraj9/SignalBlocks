// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#ifndef SIGNALBLOCKS_VECTORTOMATRIXCONVERTER_H
#define SIGNALBLOCKS_VECTORTOMATRIXCONVERTER_H

#include "../../common/Port.h"

#include <assert.h>
#include <memory>
#include <vector>

namespace sigblocks {
    template<class T>
    class ToMatrixConverter
            : public Port<1, 1, T> {
    public:
        ToMatrixConverter(std::vector<int> dims)
                : mDims(std::move(dims)), mTotalElements(1) {
            for (auto v : mDims) {
                mTotalElements *= v;
            }
            assert(mTotalElements > 0);
        }

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(sourceIndex == 0);
            assert(mTotalElements == 1);
            std::unique_ptr<T[]> tmpdata(new T[1]);
            tmpdata[0] = data;
            this->LeakMatrix(0, std::move(tmpdata), mDims, startTime);
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex == 0);
            assert(len == mTotalElements);
            // Data is passed as-is but the dimensions are passed along too.
            // This requires that the data translates from vector to matrix
            // in row-major format as required by matrix storage requirements.
            this->LeakMatrix(0, std::move(data), mDims, startTime);
        }

        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<T[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime) {
            assert(sourceIndex == 0);
            int len = 1;
            for (auto v : dims) {
                len *= v;
            }
            assert(mTotalElements == len);
            // The idea is reorganisation of matrix data where just
            // the dimensions are changed but not the underlying data.
            // This is a dangerous operation, so you better know what you
            // are trying to do before using this block with matrix
            // input data.
            this->LeakMatrix(0, std::move(data), mDims, startTime);
        }

    private:
        std::vector<int> mDims;
        int mTotalElements;
    };
}

#endif //SIGNALBLOCKS_VECTORTOMATRIXCONVERTER_H
