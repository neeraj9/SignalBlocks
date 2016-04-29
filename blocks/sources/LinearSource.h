// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#ifndef SIGNALBLOCKS_LINEARSOURCE_H
#define SIGNALBLOCKS_LINEARSOURCE_H

#include "../../common/PortType.h"
#include "../../common/Port.h"
#include "../../common/TimeTick.h"

#include <assert.h>
#include <vector>

namespace sigblocks {
    /** LinearSource A sample based linear signal source.
     * This source block generate linear signal (increasing or decreasing)
     * based on samples (and not time ticks). This block generates a scalar,
     * vector or matrix output based on the template parameter.
     *
     * For vector and matrix output each of the elements has the same value in a
     * clock cycle.
     *
     * In case you are looking for a time tick based pulse source then
     * either enhance this block or write a new one. It would be
     * better though to define a new template parameter to
     * specify time tick based linear generator though.
     */
    template<class T, PortType P = PORT_TYPE_SCALAR>
    class LinearSource
            : public Port<0, 1, T> {
    public:
        LinearSource(std::string name, const T& initialValue, const T& increment)
                : Port<0, 1, T>(std::move(name), "A linear source."),
                  mValue(initialValue),
                  mIncrement(increment) {
        }

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            this->LeakData(0, mValue, timeTick);
            mValue += mIncrement;
        }

    private:
        T mValue;
        const T mIncrement;
    };

    template<class T>
    class LinearSource<T, PORT_TYPE_VECTOR>
            : public Port<0, 1, T> {
    public:
        LinearSource(std::string name, const T& initialValue, const T& increment, int len)
                : Port<0, 1, T>(std::move(name), "A linear source."),
                  mValue(initialValue),
                  mIncrement(increment),
                  mFixedLen(len) {
        }

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            std::unique_ptr<T[]> data(new T[mFixedLen]);
            for (int i = 0; i < mFixedLen; ++i) {
                data.get()[i] = mValue;
            }
            this->LeakData(0, std::move(data), mFixedLen, timeTick);
            mValue += mIncrement;
        }

    private:
        T mValue;
        const T mIncrement;
        const int mFixedLen;
    };

    template<class T>
    class LinearSource<T, PORT_TYPE_MATRIX>
            : public Port<0, 1, T> {
    public:
        LinearSource(std::string name, const T& initialValue, const T& increment, const std::vector<int>& dims)
                : Port<0, 1, T>(std::move(name), "A linear source."),
                  mValue(initialValue),
                  mIncrement(increment),
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
            std::unique_ptr<T[]> data(new T[mFixedLen]);
            for (int i = 0; i < mFixedLen; ++i) {
                data.get()[i] = mValue;
            }
            this->LeakMatrix(0, std::move(data), mFixedDims, timeTick);
            mValue += mIncrement;
        }

    private:
        T mValue;
        const T mIncrement;
        const std::vector<int> mFixedDims;
        int mFixedLen;
    };
}

#endif //SIGNALBLOCKS_LINEARSOURCE_H
