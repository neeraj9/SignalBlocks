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

#ifndef SIGNALBLOCKS_GAIN_H
#define SIGNALBLOCKS_GAIN_H

#include "../../common/Port.h"

namespace signalblocks {
    template<class T>
    class Gain
            : public Port<1, 1, T> {
    public:
        Gain(std::string name, const T& scale)
                : Port<1, 1, T>(std::move(name), "A gain block"),
                  mScale(scale) {
        }

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            this->LeakData(0, data * mScale, startTime);
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            for (int i = 0; i < len; ++i) {
                data.get()[i] *= mScale;
            }
            this->LeakData(0, std::move(data), len, startTime);
        }

        virtual void ProcessMatrix(
                int sourceIndex, std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
            assert(! dims.empty());
            int len = dims[0];
            for (size_t i = 1; i < dims.size(); ++i) {
                len *= dims[i];
            }
            for (int i = 0; i < len; ++i) {
                data.get()[i] *= mScale;
            }
            this->LeakMatrix(0, std::move(data), dims, startTime);
        }

    private:
        const T mScale;
    };
}

#endif // SIGNALBLOCKS_GAIN_H
