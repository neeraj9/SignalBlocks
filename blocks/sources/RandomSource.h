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

#ifndef SIGNALBLOCKS_RANDOMSOURCE_H
#define SIGNALBLOCKS_RANDOMSOURCE_H

#include "../../common/PortType.h"
#include "../../common/Port.h"
#include "../../common/TimeTick.h"

#include <assert.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <limits>

namespace signalblocks {
    template <class T>
    T GetRandomNumber(unsigned int* pSeed) {
		srand(*pSeed);
        T random_value = static_cast<T>(rand());
        return random_value;
    }

    template <>
    char GetRandomNumber(unsigned int* pSeed) {
        int total_values = (std::numeric_limits<char>::max() + 1);
		srand(*pSeed);
        char random_value = static_cast<char>(rand() % total_values);
        return random_value;
    }

    template <>
    unsigned char GetRandomNumber(unsigned int* pSeed) {
        int total_values = (std::numeric_limits<unsigned char>::max() + 1);
		srand(*pSeed);
        unsigned char random_value = static_cast<unsigned char>(rand() % total_values);
        return random_value;
    }

    template <>
    short GetRandomNumber(unsigned int* pSeed) {
        int total_values = (std::numeric_limits<short>::max() + 1);
		srand(*pSeed);
        short random_value = static_cast<short>(rand() % total_values);
        return random_value;
    }

    template <>
    unsigned short GetRandomNumber(unsigned int* pSeed) {
        int total_values = (std::numeric_limits<unsigned short>::max() + 1);
		srand(*pSeed);
        unsigned short random_value = static_cast<unsigned short>(rand() % total_values);
        return random_value;

    }

    /** RandomSource A sample based random signal source.
     * This source block generate a random signal
     * based on samples. This block generates a scalar,
     * vector or matrix output based on the template parameter.
     * The output value is between [0, MAX], where
     * MAX depends on the data types and ranges are as follows:
     *
     * char: [0, 127],
     * unsigned char: [0, 255],
     * short: [0, 2^15-1],
     * unsigned short: [0, 2^16-1],
     * int: [0, 2^31-1],
     * unsigned int: [0, 2^31-1],
     * long: [0, 2^31-1],
     * unsigned long: [0, 2^31-1],
     * float: [0, 2^31-1],
     * double: [0, 2^31-1]
     *
     * This block generates psuedo-random values even for vector
     * and matrix outputs. This block treats each
     * of the vector or matrix output elements independently and
     * the values (within a vector or matrix output) are random
     * as well. This block uses the POSIX rand() method for
     * generating psuedo-random numbers.
     *
     * @todo generate psuedo-random numbers based on other
     *       distributions as well.
     */
    template <class T, PortType P = PORT_TYPE_SCALAR>
    class RandomSource
            : public Port<0, 1, T> {
    public:
        RandomSource(std::string name)
                : Port<0, 1, T>(std::move(name), "A random source."),
                  mLastTick(),
                  mSeed(static_cast<unsigned int>(time(nullptr))) {
        }

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            if (mLastTick == timeTick) {
                return;  // already processed the event
            }
            mLastTick = timeTick;
            T random_value = GetRandomNumber<T>(&mSeed);
            this->LeakData(0, random_value, timeTick);
        }

    private:
        TimeTick mLastTick;
        unsigned int mSeed;
    };

    template <class T>
    class RandomSource<T, PORT_TYPE_VECTOR>
            : public Port<0, 1, T> {
    public:
        RandomSource(std::string name, int len)
                : Port<0, 1, T>(std::move(name), "A random source."),
                  mLastTick(),
                  mSeed(static_cast<unsigned int>(time(nullptr))),
                  mFixedLen(len) {
        }

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            if (mLastTick == timeTick) {
                return;  // already processed the event
            }
            mLastTick = timeTick;

            std::unique_ptr<T[]> data(new T[mFixedLen]);
            for (int i = 0; i < mFixedLen; ++i) {
                T random_value = GetRandomNumber<T>(&mSeed);
                data.get()[i] = random_value;
            }
            this->LeakData(0, std::move(data), mFixedLen, timeTick);
        }

    private:
        TimeTick mLastTick;
        unsigned int mSeed;
        const int mFixedLen;
    };

    template <class T>
    class RandomSource<T, PORT_TYPE_MATRIX>
            : public Port<0, 1, T> {
    public:
        RandomSource(std::string name, const std::vector<int>& dims)
                : Port<0, 1, T>(std::move(name), "A random source."),
                  mLastTick(),
                  mSeed(static_cast<unsigned int>(time(nullptr))),
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
            if (mLastTick == timeTick) {
                return;  // already processed the event
            }
            mLastTick = timeTick;

            std::unique_ptr<T[]> data(new T[mFixedLen]);
            for (int i = 0; i < mFixedLen; ++i) {
                T random_value = GetRandomNumber<T>(&mSeed);
                data.get()[i] = random_value;
            }
            this->LeakMatrix(0, std::move(data), mFixedDims, timeTick);
        }

    private:
        TimeTick mLastTick;
        unsigned int mSeed;
        const std::vector<int> mFixedDims;
        int mFixedLen;
    };
}

#endif // SIGNALBLOCKS_RANDOMSOURCE_H
