// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_DOWNSAMPLE_H
#define SIGBLOCKS_DOWNSAMPLE_H

#include "../../common/Port.h"

#include <assert.h>

namespace sigblocks {
    /** DownSample Down sample the input with respect to TimeTick.
     * Down sample the input data stream with respect to TimeTick.
     *
     * If you are looking for dropping packets instead, which
     * simply drop packets then for vector and matrix inputs this may
     * not be what you are looking for. This block will drop
     * packets with respect to TimeTick, hence vector and matrix inputs
     * are treated similar to scalar input where complete vector or scalar
     * input at a particular TimeTick is one time sample. This block will
     * pass-through one input (scalar, vector or matrix) for every
     * down-sample cycle.
     */
    template<class T>
    class DownSample
            : public Port<1, 1, T> {
    public:
        DownSample(int factor)
                : mDownSampleFactor(factor),
                  mCurrentCount(0) {
            assert(mDownSampleFactor > 0); // XXX replace with some assertion library
        }

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(sourceIndex == 0); // XXX
            if (mCurrentCount == 0) {
                this->LeakData(0, data, startTime);
            }
            mCurrentCount = (mCurrentCount + 1) % mDownSampleFactor;
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex == 0); // XXX
            if (len < 0) {
                return;
            }
            if (mCurrentCount == 0) {
                this->LeakData(0, std::move(data), len, startTime);
            }
            mCurrentCount = (mCurrentCount + 1) % mDownSampleFactor;
        }

        virtual void ProcessMatrix(
                int sourceIndex, std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
            assert(sourceIndex == 0); // XXX
            if (mCurrentCount == 0) {
                this->LeakMatrix(0, std::move(data), dims, startTime);
            }
            mCurrentCount = (mCurrentCount + 1) % mDownSampleFactor;
        }

    private:
        int mDownSampleFactor;
        int mCurrentCount;
    };
}

#endif // SIGBLOCKS_DOWNSAMPLE_H
