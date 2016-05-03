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

#include "SocketSink.h"

#include "../../socket/ISocket.h"

#include <iostream>

using namespace signalblocks;
using namespace std;

#define DESCRIPTION "A sink which passes through the data to a sock."

SocketSink::SocketSink(std::string name)
        : Port(std::move(name), DESCRIPTION),
          mpSocket(nullptr) {
}

SocketSink::SocketSink(std::string name, std::unique_ptr<SocketProgramming::ISocket> pOuts)
        : Port(std::move(name), DESCRIPTION),
          mpSocket(std::move(pOuts)) {
}

void
SocketSink::SetStreamSink(std::unique_ptr<SocketProgramming::ISocket> pOuts) {
    mpSocket.swap(pOuts);
}

void
SocketSink::Process(int sourceIndex, const unsigned char& data, const TimeTick& startTime) {
    mpSocket->Send(&data, sizeof(unsigned char));
}

void
SocketSink::Process(
        int sourceIndex, std::unique_ptr<unsigned char[]> data, int len, const TimeTick& startTime) {
    int bytes_sent =
            mpSocket->Send(data.get(), len * sizeof(unsigned char));
    if (bytes_sent < len) {
        cerr << "Sent " << bytes_sent << ", but wanted to send " << len << " bytes" << endl;
    }
}
