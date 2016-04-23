// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_COMPLEXSTREAMSOURCE_H
#define SIGBLOCKS_COMPLEXSTREAMSOURCE_H

#include "../../common/PortType.h"
#include "../../common/Port.h"
#include "../../common/TimeTick.h"
#include "../../common/logging.h"

#include <fstream>
#include <memory>

namespace sigblocks {

    /** ComplexStreamSource A sample based complex signal source.
     * This source block generate a complex signal
     * based on samples read from a file. This block generates a
     * vector output only at present. The complex output is
     * read from a file as {I, Q}, where I and Q are read as-is
     * without endian conversion. Hence the endianess of the
     * encoded I and Q will depend on the system which stored
     * those complex values. The target system which is reading
     * these values (where this block is run) must also be
     * of the same endianess for things to be read correctly.
     *
     * In case the template parameter T is float or double, then
     * the values (of I and Q) stored the file are assumed to
     * be a direct memory write. This requires same hardware platform
     * to generate I and Q as the one which is running this block
     * to read it back directly into memory.
     *
     * @note scalar output is never generated because there are
     *       always {I, Q} outputs which is generated as a
     *       vector of size 2.
     *
     * @todo allow user to specify endianness in addition to
     *       native, which is used at present.
     *
     * @todo for decoding of I and Q as floating point must
     *       follow some standard instead of just reading
     *       directly into memory (which is used at present).
     */
    template<class T, PortType P = PORT_TYPE_VECTOR>
    class ComplexStreamSource
            : public Port<0, 1, T> {
    public:
        ComplexStreamSource(int blockSize) : mLastTick(),
                                             mpComplexStream(nullptr),
                                             mLoopOver(true),
                                             mBlockSize(blockSize) {
        }

        ComplexStreamSource(std::unique_ptr<std::istream> pIns,
                            int blockSize) : mLastTick(),
                                             mpComplexStream(std::move(pIns)),
                                             mLoopOver(true),
                                             mBlockSize(blockSize) {
        }

        void SetStreamSource(std::unique_ptr<std::istream> pIns) {
            mpComplexStream.swap(pIns);
        }

        void Loop(bool loopOver) {
            mLoopOver = loopOver;
        }

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            if (mpComplexStream->eof() && !mLoopOver) {
                LOG_INFO("%p ComplexStreamSource is done processing!\n", this);
                return; // dont do anything
            }

            if (mLastTick == timeTick) {
                return;  // already processed the event
            }
            mLastTick = timeTick;

            std::unique_ptr<T[]> data(new T[2 * mBlockSize]);
            // There are two values per sample, RF Sample = {I,Q}.
            mpComplexStream->read(reinterpret_cast<char*>(data.get()), mBlockSize * 2 * sizeof(T));
            int bytes_read = mpComplexStream->gcount() / sizeof(char);
            this->LeakData(0, std::move(data), bytes_read, timeTick);
            if (mpComplexStream->eof() && mLoopOver) {
                mpComplexStream->clear();
                mpComplexStream->seekg(0, std::ios::beg);
            }
        }

    private:
        TimeTick mLastTick;
        std::unique_ptr<std::istream> mpComplexStream;
        bool mLoopOver;
        int mBlockSize;
    };

    template<class T>
    class ComplexStreamSource<T, PORT_TYPE_MATRIX>
            : public Port<0, 1, T> {
    public:
        ComplexStreamSource(const std::vector<int>& dims)
                : mLastTick(),
                  mpComplexStream(nullptr),
                  mLoopOver(true),
                  mFixedDims(dims),
                  mFixedLen(1) {
            assert(! mFixedDims.empty());
            // since we need I and Q so double the last dimension
            // to store I and Q values
            mFixedDims.back() = mFixedDims.back() * 2;
            for (size_t i = 0; i < mFixedDims.size(); ++i) {
                mFixedLen *= mFixedDims[i];
            }
            assert(mFixedLen > 0 && (mFixedLen % 2 == 0));
        }

        ComplexStreamSource(std::unique_ptr<std::istream> pIns,
                            const std::vector<int>& dims)
                : mLastTick(),
                  mpComplexStream(std::move(pIns)),
                  mLoopOver(true),
                  mFixedDims(dims),
                  mFixedLen(1) {
            assert(! mFixedDims.empty());
            // since we need I and Q so double the last dimension
            // to store I and Q values
            mFixedDims.back() = mFixedDims.back() * 2;
            for (size_t i = 0; i < mFixedDims.size(); ++i) {
                mFixedLen *= mFixedDims[i];
            }
            assert(mFixedLen > 0 && (mFixedLen % 2 == 0));
        }

        void SetStreamSource(std::unique_ptr<std::istream> pIns) {
            mpComplexStream.swap(pIns);
        }

        void Loop(bool loopOver) {
            mLoopOver = loopOver;
        }

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            if (mpComplexStream->eof() && !mLoopOver) {
                LOG_INFO("%p ComplexStreamSource is done processing!\n", this);
                return; // dont do anything
            }

            if (mLastTick == timeTick) {
                return;  // already processed the event
            }
            mLastTick = timeTick;

            std::unique_ptr<T[]> data(new T[mFixedLen]);
            // There are two values per sample, RF Sample = {I,Q}.
            mpComplexStream->read(reinterpret_cast<char*>(data.get()), mFixedLen * sizeof(T));
            size_t bytes_read = mpComplexStream->gcount() / sizeof(char);
            if (bytes_read > 0) {
                this->LeakMatrix(0, std::move(data), mFixedDims, timeTick);
            }
            if (mpComplexStream->eof() && mLoopOver) {
                mpComplexStream->clear();
                mpComplexStream->seekg(0, std::ios::beg);
            }
        }

    private:
        TimeTick mLastTick;
        std::unique_ptr<std::istream> mpComplexStream;
        bool mLoopOver;
        std::vector<int> mFixedDims;
        int mFixedLen;
    };
}

#endif // SIGBLOCKS_COMPLEXSTREAMSOURCE_H
