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
        ConstantSource(std::string name, const T& value)
                : Port<0, 1, T>(std::move(name), "A constant source."),
                  mFixedValue(value) {
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
        ConstantSource(std::string name, const T& value, int len)
                : Port<0, 1, T>(std::move(name), "A constant source."),
                  mFixedValue(value),
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
        ConstantSource(std::string name, const T& value, const std::vector<int>& dims)
                : Port<0, 1, T>(std::move(name), "A constant source."),
                  mFixedValue(value),
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
