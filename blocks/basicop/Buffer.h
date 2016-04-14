// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_BUFFER_H
#define SIGBLOCKS_BUFFER_H

#include "../../common/Port.h"

#include <vector>

namespace sigblocks {
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
        Buffer(int fieldsToBuffer);

        virtual ~Buffer();

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime);

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime);

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

#endif // SIGBLOCKS_BUFFER_H
