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
    template<class T, PortType P = PORT_TYPE_SCALAR>
    class LinearSource
            : public Port<0, 1, T> {
    public:
        LinearSource(const T& initialValue, const T& increment)
                : mValue(initialValue),
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
        LinearSource(const T& initialValue, const T& increment, int len)
                : mValue(initialValue),
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
        LinearSource(const T& initialValue, const T& increment, const std::vector<int>& dims)
                : mValue(initialValue),
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
