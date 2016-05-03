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

#include "SocketSource.h"

#include "../../socket/ISocket.h"

#include <iostream>
#include <assert.h>
#include <string.h>

using namespace sigblocks;
using namespace std;

#define DESCRIPTION "A source block gets data from a socket"

namespace {
    const int MAX_BUFFER_SIZE = 4096; // power of 2 (2^12 = 4096)
}

SocketSource::SocketSource(std::string name, int blockSize)
        : Port<0, 1, unsigned char>(std::move(name), DESCRIPTION),
          mLastTick(),
          mpSocket(nullptr),
          mBlockSize(blockSize),
          mpBuffer(new uint8_t[MAX_BUFFER_SIZE]),
          mBytesRead(0),
          mBufferSize(0) {
    assert(mBlockSize > 0);
}

SocketSource::SocketSource(
        std::string name,
        std::unique_ptr<SocketProgramming::ISocket> pIns, int blockSize)
        : Port<0, 1, unsigned char>(std::move(name), DESCRIPTION),
          mLastTick(),
          mpSocket(std::move(pIns)),
          mBlockSize(blockSize),
          mpBuffer(new uint8_t[MAX_BUFFER_SIZE]),
          mBytesRead(0),
          mBufferSize(0) {
    assert(mBlockSize > 0);
}

void
SocketSource::SetStreamSource(std::unique_ptr<SocketProgramming::ISocket> pIns) {
    mpSocket.swap(pIns);
}

void
SocketSource::ClockCycle(const TimeTick& timeTick) {
    if (!mpSocket->IsValid()) {
        cerr << this << " SocketSource is done processing!" << endl;
        return; // dont do anything
    }
    if (mLastTick == timeTick) {
        return;  // already processed the event
    }
    mLastTick = timeTick;

    if ((mBlockSize - (mBufferSize - mBytesRead)) > 0) {
        // get max possible bytes
        int bytes_read =
                mpSocket->Receive(mpBuffer.get() + mBytesRead,
                                  (MAX_BUFFER_SIZE - (mBufferSize - mBytesRead)) * sizeof(uint8_t));
        if (bytes_read > 0) {
            mBufferSize += bytes_read;
            assert(MAX_BUFFER_SIZE >= mBufferSize);
        }
    }
    // This logic will fail to send less than mBlockSize in case of
    // connection oriented socket (eg. TCP) when the connected
    // was lost.
    // TODO: Do we care?
    if ((mBufferSize - mBytesRead) >= mBlockSize) {
        std::unique_ptr<uint8_t[]> data(new uint8_t[mBlockSize]);
        memcpy(data.get(), mpBuffer.get() + mBytesRead, mBlockSize);
        LeakData(0, std::move(data), mBlockSize, timeTick);
        mBytesRead += mBlockSize;
        if (mBytesRead == mBufferSize) {
            mBytesRead = mBufferSize = 0;
        }
    }
}
