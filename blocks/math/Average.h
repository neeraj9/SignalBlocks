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

#ifndef SIGNALBLOCKS_AVERAGE_H
#define SIGNALBLOCKS_AVERAGE_H

#include "../../common/Port.h"

#include <cassert>
#include <stdint.h>

namespace signalblocks {
    ///
    ///
    /// @caution No under/overflow checking is performed.
    ///

    /** Average computes an average of the inputs.
     * Computes an average of the inputs (over scalar, vector or matrix).
     *
     * For scalar the average is the value itself
     * and for vector & matrix the output is the
     * average of all the elements.
     *
     * The output of this block is always scalar.
     */
    template<class T>
    class Average
            : public Port<1, 1, T> {
    public:
        Average(std::string name)
                : Port<1, 1, T>(std::move(name), "An averaging block.") {
        }

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            // average of single value is the value itself
            this->LeakData(0, data, startTime);
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex == 0);
            assert(len > 0);
            T* raw_data = data.get();
            T data_sum = raw_data[0];
            for (int i = 1; i < len; ++i) {
                data_sum += raw_data[i];
            }
            T result = data_sum / len;
            this->LeakData(0, result, startTime);
        }

        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<T[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime) {
            assert(sourceIndex == 0);
            assert(! dims.empty());

            // Matrix are stored in row-major format. See IPort.h for details.
            int len = 1;
            //printf("dims=[");
            for (size_t i = 0; i < (dims.size() - 1); ++i) {
                //printf("%d, ", dims[i]);
                len *= dims[i];
            }
            //printf("%d]\n", dims[dims.size()-1]);
            assert(len > 0);
            T* raw_data = data.get();
            T data_sum = raw_data[0];
            for (int i = 1; i < len; ++i) {
                data_sum += raw_data[i];
            }
            T result = data_sum / len;
            this->LeakData(0, result, startTime);
        }
    };
}

#endif // SIGNALBLOCKS_AVERAGE_H
