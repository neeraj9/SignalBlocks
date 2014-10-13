// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "SocketTransceiver.hh"

#include "../../common/MultiPtr.hh"
#include "../../common/socket/ISocket.hh"

#include <iostream>

using namespace sigblocks;
using namespace std;

namespace
{
  const int MAX_BUFFER_SIZE = 4096; // power of 2 (2^12 = 4096)
}

SocketTransceiver::SocketTransceiver(
  const TimeTick& startTime, TimeTick& increment,
  int blockSize)
  : mTime(startTime),
    mIncrement(increment),
    mpSocket(0),
    mBlockSize(blockSize),
    mpBuffer(new uint8_t[MAX_BUFFER_SIZE]),
    mBytesRead(0),
    mBufferSize(0)
{
  assert(mBlockSize > 0);
}

SocketTransceiver::SocketTransceiver(
  const TimeTick& startTime, TimeTick& increment,
  int blockSize,
  std::auto_ptr<SocketProgramming::ISocket> pIns)
  : mTime(startTime),
    mIncrement(increment),
    mpSocket(pIns),
    mBlockSize(blockSize),
    mpBuffer(new uint8_t[MAX_BUFFER_SIZE]),
    mBytesRead(0),
    mBufferSize(0)
{
  assert(mBlockSize > 0);
}

void
SocketTransceiver::SetStreamTransceiver(std::auto_ptr<SocketProgramming::ISocket> pOuts)
{
  mpSocket = pOuts;
}

// Needed by the Source
void
SocketTransceiver::Generate()
{
  if (!mpSocket->IsValid())
  {
    cerr << this << " SocketSource is done processing!" << endl;
    return; // dont do anything
  }

  if ((mBlockSize - (mBufferSize - mBytesRead)) > 0)
  {
    // get max possible bytes
    int bytes_read = 
      mpSocket->Receive(mpBuffer.get() + mBytesRead, (MAX_BUFFER_SIZE - (mBufferSize - mBytesRead)) * sizeof(uint8_t));
    if (bytes_read > 0)
    {
      mBufferSize += bytes_read;
      assert(MAX_BUFFER_SIZE >= mBufferSize);
    }
  }
  // This logic will fail to send less than mBlockSize in case of
  // connection oriented socket (eg. TCP) when the connected
  // was lost.
  // TODO: Do we care?
  if ((mBufferSize - mBytesRead) >= mBlockSize)
  {
    MultiPtr<uint8_t> data(new uint8_t[mBlockSize]);
    memcpy(data.get(), mpBuffer.get() + mBytesRead, mBlockSize);
    LeakData(0, data, mBlockSize, mTime);
    mBytesRead += mBlockSize;
    if (mBytesRead == mBufferSize)
    {
      mBytesRead = mBufferSize = 0;
    }
  }
  mTime += mIncrement;
}

// Needed by the Sink
void
SocketTransceiver::Process(int sourceIndex, const unsigned char& data, const TimeTick& startTime)
{
  mpSocket->Send(&data, sizeof(unsigned char));
}
  
// Needed by the Sink
void
SocketTransceiver::Process(
  int sourceIndex, MultiPtr<unsigned char> pData, int len, const TimeTick& startTime)
{
  int bytes_sent =
    mpSocket->Send(pData.get(), len * sizeof(unsigned char));
  if (bytes_sent < len)
  {
    cerr << "Sent " << bytes_sent << ", but wanted to send " << len << " bytes" << endl;
  }
}
