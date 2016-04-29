// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#ifndef SIGNALBLOCKS_PULSESOURCE_H
#define SIGNALBLOCKS_PULSESOURCE_H

#include "../../common/PortType.h"
#include "../../common/Port.h"
#include "../../common/TimeTick.h"

#include <assert.h>
#include <vector>

namespace sigblocks {

    /** PulseSource A sample based pulse signal source.
     * This source block generate pulse signal (high and then low)
     * based on samples (and not time ticks). This block generates a scalar,
     * vector or matrix output based on the template parameter.
     *
     * For vector and matrix output each of the elements has the same value in a
     * clock cycle.
     *
     * In case you are looking for a time tick based pulse source then
     * either enhance this block or write a new one. It would be
     * better though to define a new template parameter to
     * specify time tick based pulse generator though.
     */
    template<class T, PortType P = PORT_TYPE_SCALAR>
    class PulseSource
            : public Port<0, 1, T> {
    public:
        PulseSource(std::string name,
                    double periodOffset,
                    int periodSamples,
                    double highRatio,
                    const T& highValue,
                    const T& lowValue)
                : Port<0, 1, T>(std::move(name), "A pulse source."),
                  mPeriodSample(0),
                  mTotalPeriodSamples(periodSamples),
                  mHighRatio(highRatio),
                  mHighValue(highValue),
                  mLowValue(lowValue) {
            assert(periodOffset >= 0.0);
            assert(periodOffset < 1.0);
            assert(highRatio > 0.0);
            assert(highRatio < 1.0);

            mPeriodSample = static_cast<int>(mTotalPeriodSamples * periodOffset);
        }

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            T value = mLowValue;
            double ratio = static_cast<double>(mPeriodSample) / mTotalPeriodSamples;
            if (ratio <= mHighRatio) {
                value = mHighValue;
            }
            this->LeakData(0, value, timeTick);
            mPeriodSample = (mPeriodSample + 1) % mTotalPeriodSamples;
        }

    private:
        int mPeriodSample;
        const int mTotalPeriodSamples;
        const double mHighRatio;
        const T mHighValue;
        const T mLowValue;
    };

    template<class T>
    class PulseSource<T, PORT_TYPE_VECTOR>
            : public Port<0, 1, T> {
    public:
        PulseSource(std::string name,
                    double periodOffset,
                    int periodSamples,
                    double highRatio,
                    const T& highValue,
                    const T& lowValue,
                    int len)
                : Port<0, 1, T>(std::move(name), "A pulse source."),
                  mPeriodSample(0),
                  mTotalPeriodSamples(periodSamples),
                  mHighRatio(highRatio),
                  mHighValue(highValue),
                  mLowValue(lowValue),
                  mFixedLen(len) {
        }

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            T value = mLowValue;
            double ratio = static_cast<double>(mPeriodSample) / mTotalPeriodSamples;
            if (ratio <= mHighRatio) {
                value = mHighValue;
            }
            std::unique_ptr<T[]> data(new T[mFixedLen]);
            for (int i = 0; i < mFixedLen; ++i) {
                data.get()[i] = value;
            }
            this->LeakData(0, std::move(data), mFixedLen, timeTick);
            mPeriodSample = (mPeriodSample + 1) % mTotalPeriodSamples;
        }

    private:
        int mPeriodSample;
        const int mTotalPeriodSamples;
        const double mHighRatio;
        const T mHighValue;
        const T mLowValue;
        const int mFixedLen;
    };

    template<class T>
    class PulseSource<T, PORT_TYPE_MATRIX>
            : public Port<0, 1, T> {
    public:
        PulseSource(std::string name,
                    double periodOffset,
                    int periodSamples,
                    double highRatio,
                    const T& highValue,
                    const T& lowValue,
                    const std::vector<int>& dims)
                : Port<0, 1, T>(std::move(name), "A pulse source."),
                  mPeriodSample(0),
                  mTotalPeriodSamples(periodSamples),
                  mHighRatio(highRatio),
                  mHighValue(highValue),
                  mLowValue(lowValue),
                  mFixedDims(dims),
                  mFixedLen(1) {
            assert(! mFixedDims.empty());
            for (size_t i = 0; i < mFixedDims.size(); ++i) {
                mFixedLen *= mFixedDims[i];
            }
            assert(mFixedLen > 0);
        }

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            T value = mLowValue;
            double ratio = static_cast<double>(mPeriodSample) / mTotalPeriodSamples;
            if (ratio <= mHighRatio) {
                value = mHighValue;
            }

            std::unique_ptr<T[]> data(new T[mFixedLen]);
            for (int i = 0; i < mFixedLen; ++i) {
                data.get()[i] = value;
            }
            this->LeakMatrix(0, std::move(data), mFixedDims, timeTick);
            mPeriodSample = (mPeriodSample + 1) % mTotalPeriodSamples;
        }

    private:
        int mPeriodSample;
        const int mTotalPeriodSamples;
        const double mHighRatio;
        const T mHighValue;
        const T mLowValue;
        const std::vector<int> mFixedDims;
        int mFixedLen;
    };
}

#endif //SIGNALBLOCKS_PULSESOURCE_H
