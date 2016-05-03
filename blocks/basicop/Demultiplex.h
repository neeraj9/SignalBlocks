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

#ifndef SIGBLOCKS_DEMULTIPLEX_H
#define SIGBLOCKS_DEMULTIPLEX_H

#include "../../common/Port.h"

#include <list>

namespace signalblocks {
    template<int N, class T>
    class Demultiplex
            : public Port<1, N, T> {
    public:
        Demultiplex() : mNextOutputPort(0) {
            // need to set mIsVectorEnabled to false (XXX)
        }

        virtual ~Demultiplex() {
            // Need for vector buffer when vector support is added (XXX)
        }

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < N); // XXX change to an assertion library.

            this->LeakData(mNextOutputPort, data, startTime);

            mNextOutputPort = (mNextOutputPort + 1) % N;
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < N); // XXX change to an assertion library.

            // Mutiple inputs are treated as one when passing downstream
            // use some other class to split such a data into individual data
            // items if those need to be demultiplexed and then apply this module.
            this->LeakData(mNextOutputPort, std::move(data), len, startTime);

            mNextOutputPort = (mNextOutputPort + 1) % N;
        }

        // TODO FIXME based on the matrix storage as specified in IPort.h
        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<T[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < N); // XXX change to an assertion library.

            // Mutiple inputs are treated as one when passing downstream
            // use some other class to split such a data into individual data
            // items if those need to be demultiplexed and then apply this module.
            this->LeakMatrix(mNextOutputPort, std::move(data), dims, startTime);

            mNextOutputPort = (mNextOutputPort + 1) % N;
        }

    private:
        int mNextOutputPort;
    };
}

#endif // SIGBLOCKS_DEMULTIPLEX_H
