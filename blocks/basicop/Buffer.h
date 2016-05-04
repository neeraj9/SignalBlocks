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

#ifndef SIGNALBLOCKS_BUFFER_H
#define SIGNALBLOCKS_BUFFER_H

#include "../../common/Port.h"

#include <memory>
#include <string>
#include <vector>

namespace signalblocks {
    /** Buffer Buffer and send downstream after the buffer is full.
     * This block buffers the input data until the buffer limit is
     * reached and then sends the buffered data in one shot and the
     * overflowed data is buffered again.
     *
     * Note that the data is sent out as a vector or a matrix, which
     * depends on the input data which when received (as input)
     * caused the buffer full (or overflow) event. Alternatively,
     * there could be an initial configuration which sets the personality,
     * of this block thereby forcing it to operate in vector or matrix
     * output mode but this design is not implemented to provide maximum
     * possible flexibility with minimal configuration requirements.
     *
     * It is always better to connect this block to a sane block which
     * should transmit either scalar or vector or matrix and keep it
     * at that. Additionally, pick and choose same dimensions for the
     * matrix because mixing dimensions of a matrix type is tricky
     * and must be done with utmost care. Having said that there is no
     * hard restrictions against it for complex fine-tuned cases
     * where such requirements exist.
     */
    template<class T>
    class Buffer
            : public Port<1, 1, T> {
    public:
        Buffer(std::string name, int fieldsToBuffer);

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime);

        /// Note: the source of this module can call any combination of the above
        /// or this method and the buffering will work. This is done to allow the
        /// additional flexibility to the user to mix both interfaces (scalar and vector).
        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime);

        /// Note: the source of this module can call any combination of the above
        /// or this method and the buffering will work. This is done to allow the
        /// additional flexibility to the user to mix interfaces (scalar, vector or matrix).
        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<T[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime);
    private:
        int mFieldsToBuffer; // avoid const so that it can be changed dynamically
        int mNumFields; // fields buffered so far
        std::unique_ptr<T[]> mData;
    };
}

#endif // SIGNALBLOCKS_BUFFER_H

#include "Buffer.tcc"