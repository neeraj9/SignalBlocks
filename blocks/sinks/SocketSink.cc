// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "SocketSink.hh"

#include "../../common/MultiPtr.hh"
#include "../../common/socket/ISocket.hh"

#include <iostream>

using namespace iblocks;
using namespace std;

SocketSink::SocketSink()
  : mpSocket(0)
{
}

SocketSink::SocketSink(std::auto_ptr<SocketProgramming::ISocket> pOuts)
  : mpSocket(pOuts)
{
}

void
SocketSink::SetStreamSink(std::auto_ptr<SocketProgramming::ISocket> pOuts)
{
  mpSocket = pOuts;
}

void
SocketSink::Process(int sourceIndex, const unsigned char& data, const TimeTick& startTime)
{
  mpSocket->Send(&data, sizeof(unsigned char));
}
  
void
SocketSink::Process(
  int sourceIndex, MultiPtr<unsigned char> pData, int len, const TimeTick& startTime)
{
  int bytes_sent =
    mpSocket->Send(pData.get(), len * sizeof(unsigned char));
  if (bytes_sent < len)
  {
    cerr << "Sent " << bytes_sent << ", but wanted to send " << len << " bytes" << endl;
  }
}
