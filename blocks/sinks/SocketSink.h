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

#ifndef SIGNALBLOCKS_SOCKETSINK_H
#define SIGNALBLOCKS_SOCKETSINK_H

#include "../../common/Port.h"

#include <memory>

namespace SocketProgramming {
    class ISocket;
}

namespace signalblocks {
    class SocketSink
            : public Port<1, 0, unsigned char> {
    public:
        SocketSink(std::string name);

        SocketSink(std::string name, std::unique_ptr<SocketProgramming::ISocket> pIns);

        void SetStreamSink(std::unique_ptr<SocketProgramming::ISocket> pIns);

    public: // Port interface
        void Process(int sourceIndex, const unsigned char& data, const TimeTick& startTime);

        void Process(
                int sourceIndex, std::unique_ptr<unsigned char[]> data, int len, const TimeTick& startTime);

    private:
        std::unique_ptr<SocketProgramming::ISocket> mpSocket;
    };
}

#endif // SIGNALBLOCKS_SOCKETSINK_H
