// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "SocketSink.hh"

#include "../../socket/ISocket.hh"

#include <iostream>

using namespace sigblocks;
using namespace std;

SocketSink::SocketSink()
        : mpSocket(nullptr) {
}

SocketSink::SocketSink(std::unique_ptr<SocketProgramming::ISocket> pOuts)
        : mpSocket(std::move(pOuts)) {
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
