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

#ifndef SIGBLOCKS_SOCKETSOURCE_H
#define SIGBLOCKS_SOCKETSOURCE_H

#include "../../common/Port.h"
#include "../../common/TimeTick.h"

#include <memory>
#include <stdint.h>

namespace SocketProgramming {
    class ISocket;
}

namespace sigblocks {
    class SocketSource
            : public Port<0, 1, unsigned char> {
    public:
        SocketSource(std::string name, int blockSize);

        SocketSource(std::string name,
                     std::unique_ptr<SocketProgramming::ISocket> pIns,
                     int blockSize);

        void SetStreamSource(std::unique_ptr<SocketProgramming::ISocket> pIns);

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        TimeTick mLastTick;
        std::unique_ptr<SocketProgramming::ISocket> mpSocket;
        const int mBlockSize;
        std::unique_ptr<uint8_t[]> mpBuffer;
        int mBytesRead;
        int mBufferSize;
    };
}

#endif // SIGBLOCKS_SOCKETSOURCE_H
