// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_CONSTANTSOURCE_H
#define SIGBLOCKS_CONSTANTSOURCE_H

#include "../../common/PortType.h"
#include "../../common/Port.h"
#include "../../common/TimeTick.h"

#include <vector>

namespace sigblocks {
    template<class T, PortType P = PORT_TYPE_SCALAR>
    class ConstantSource
            : public Port<0, 1, T> {
    public:
        ConstantSource(const T& value)
                : mFixedValue(value) {
        }

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            this->LeakData(0, mFixedValue, timeTick);
        }

    private:
        const T mFixedValue;
    };

    template<class T>
    class ConstantSource<T, PORT_TYPE_VECTOR>
            : public Port<0, 1, T> {
    public:
        ConstantSource(const T& value, int len)
                : mFixedValue(value),
                  mFixedLen(len) {
        }

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            std::unique_ptr<T[]> data(new T[mFixedLen]);
            for (int i = 0; i < mFixedLen; ++i) {
                data.get()[i] = mFixedValue;
            }
            this->LeakData(0, std::move(data), mFixedLen, timeTick);
        }

    private:
        const T mFixedValue;
        const int mFixedLen;
    };

    template<class T>
    class ConstantSource<T, PORT_TYPE_MATRIX>
            : public Port<0, 1, T> {
    public:
        ConstantSource(const T& value, const std::vector<int>& dims)
                : mFixedValue(value),
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
                data.get()[i] = mFixedValue;
            }
            this->LeakMatrix(0, std::move(data), mFixedDims, timeTick);
        }

    private:
        const T mFixedValue;
        const std::vector<int> mFixedDims;
        int mFixedLen;
    };
}

#endif // SIGBLOCKS_CONSTANTSOURCE_H
