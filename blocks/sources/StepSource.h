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

#ifndef SIGNALBLOCKS_STEPSOURCE_H
#define SIGNALBLOCKS_STEPSOURCE_H

#include "../../common/PortType.h"
#include "../../common/Port.h"
#include "../../common/TimeTick.h"

#include <assert.h>
#include <vector>

namespace signalblocks {
    /** StepSource A sample based step signal source.
     * This source block generate step signal (increasing by 1)
     * based on samples (and not time ticks). This block generates a scalar,
     * vector or matrix output based on the template parameter.
     *
     * For vector and matrix output each of the elements has the same value in a
     * clock cycle.
     *
     * In case you are looking for a time tick based pulse source then
     * either enhance this block or write a new one. It would be
     * better though to define a new template parameter to
     * specify time tick based step generator though.
     */
    template<class T, PortType P = PORT_TYPE_SCALAR>
    class StepSource
            : public Port<0, 1, T> {
    public:
        StepSource(std::string name, const T& initialValue)
                : Port<0, 1, T>(std::move(name), "A step source."),
                  mValue(initialValue),
                  mIncrement(1) {
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
    class StepSource<T, PORT_TYPE_VECTOR>
            : public Port<0, 1, T> {
    public:
        StepSource(std::string name, const T& initialValue, int len)
                : Port<0, 1, T>(std::move(name), "A step source."),
                  mValue(initialValue),
                  mIncrement(1),
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
    class StepSource<T, PORT_TYPE_MATRIX>
            : public Port<0, 1, T> {
    public:
        StepSource(std::string name, const T& initialValue, const std::vector<int>& dims)
                : Port<0, 1, T>(std::move(name), "A step source."),
                  mValue(initialValue),
                  mIncrement(1),
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

#endif //SIGNALBLOCKS_STEPSOURCE_H
