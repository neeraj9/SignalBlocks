// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#ifndef SIGNALBLOCKS_DELAY_H
#define SIGNALBLOCKS_DELAY_H

#include "../../common/PortType.h"
#include "../../common/Port.h"

#include <assert.h>
#include <deque>
#include <memory>
#include <vector>

namespace sigblocks {

    /** Delay Delay signal by a fixed samples.
     * This block delays the input by fixed number of samples.
     * The delay of vector and matrix inputs is same as that for
     * scalar, which is based on samples.
     *
     */
    template<class T, PortType P = PORT_TYPE_SCALAR>
    class Delay
            : public Port<1, 1, T> {
    public:
        Delay(int numSamples)
                : mDelaySamples(numSamples),
                  mNextIndex(0),
                  mBufferCount(0),
                  mCircularBuffer(numSamples) {
            assert(mDelaySamples > 0);
        }

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(sourceIndex == 0);
            if (mBufferCount < mDelaySamples) {
                ++mBufferCount;
                mCircularBuffer[mNextIndex] = data;
            } else {
                T prev_data = mCircularBuffer[mNextIndex];
                mCircularBuffer[mNextIndex] = data;
                this->LeakData(0, prev_data, startTime);
            }
            mNextIndex = (mNextIndex + 1) % mDelaySamples;
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(0);  // not valid
        }

        virtual void ProcessMatrix(
                int sourceIndex, std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
            assert(0); // not valid
        }

    private:
        int mDelaySamples;
        int mNextIndex;
        int mBufferCount;
        std::vector<T> mCircularBuffer;
    };

    template<class T>
    class Delay<T, PORT_TYPE_VECTOR>
            : public Port<1, 1, T> {
    public:
        Delay(int numSamples)
                : mDelaySamples(numSamples),
                  mQueue() {
            assert(mDelaySamples > 0);
        }

    protected:
        struct Info {
            Info() : mData(nullptr), mLength(0) {
            }

            std::unique_ptr<T[]> mData;
            int mLength;
        };

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(0);  // not supported
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex == 0);
            if (len < 0) {
                return;
            }
            // save the latest data
            mQueue.push_back(Info());
            mQueue.back().mData.swap(data);
            mQueue.back().mLength = len;
            // Leak only when the queue is full.
            // Check for (mDelaySamples + 1) because we are pushing before
            // popping.
            if (mQueue.size() == static_cast<size_t>(mDelaySamples + 1)) {
                std::unique_ptr<T[]> prev_data = std::move(mQueue.front().mData);
                int prev_len = mQueue.front().mLength;
                mQueue.pop_front();
                this->LeakData(0, std::move(prev_data), prev_len, startTime);
            }
        }

        virtual void ProcessMatrix(
                int sourceIndex, std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
            assert(0); // TODO
        }

    private:
        int mDelaySamples;
        std::deque<Info> mQueue;
    };


    template<class T>
    class Delay<T, PORT_TYPE_MATRIX>
            : public Port<1, 1, T> {
    public:
        Delay(int numSamples)
                : mDelaySamples(numSamples),
                  mQueue() {
            assert(mDelaySamples > 0);
        }

    protected:
        struct InfoMatrix {
            InfoMatrix() : mData(nullptr), mDims() {
            }

            std::unique_ptr<T[]> mData;
            std::vector<int> mDims;
        };

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(0);  // not supported
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(0);  // not supported
        }

        virtual void ProcessMatrix(
                int sourceIndex, std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
            assert(sourceIndex == 0);
            if (dims.empty()) {
                return;
            }
            int len = 1;
            for (auto v : dims) {
                len *= v;
            }
            if (len <= 0) {
                return;
            }
            // save the latest data
            mQueue.push_back(InfoMatrix());
            mQueue.back().mData.swap(data);
            mQueue.back().mDims = dims;
            // Leak only when the queue is full.
            // Check for (mDelaySamples + 1) because we are pushing before
            // popping.
            if (mQueue.size() == static_cast<size_t>(mDelaySamples + 1)) {
                std::unique_ptr<T[]> prev_data = std::move(mQueue.front().mData);
                std::vector<int> prev_dims(std::move(mQueue.front().mDims));
                mQueue.pop_front();
                this->LeakMatrix(0, std::move(prev_data), prev_dims, startTime);
            }
        }

    private:
        int mDelaySamples;
        std::deque<InfoMatrix> mQueue;
    };
}

#endif //SIGNALBLOCKS_DELAY_H
