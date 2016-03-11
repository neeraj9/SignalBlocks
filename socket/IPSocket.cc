// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "IPSocket.hh"

#include <arpa/inet.h>
#include <iostream>

using namespace SocketProgramming;
using namespace std;

IPSocket::IPSocket(const string& destIp, int destPort)
        : mLocalPort(-1),
          mRawSocket(),
          mDestIp(destIp),
          mDestPort(destPort) {
    mDestPort = htons(mDestPort);
}

IPSocket::~IPSocket() {
}

void
IPSocket::SetDestination(const string& destIp, int destPort) {
    mDestIp = destIp;
    mDestPort = htons(destPort);
}

bool
IPSocket::Bind(int port) {
    if (mLocalPort > 0) {
        cerr << "Cannot bind same socket to another port" << endl;
        return false;
    }
    if (!mRawSocket.Bind("any")) {
        return false;
    }

    mLocalPort = htons(port);
    return true;
}

bool
IPSocket::Connect() {
    return true; // no connection required
}

bool
IPSocket::Listen(int backlog) {
    return true; // no connection required
}

int
IPSocket::Receive(uint8_t* pBuff, int length) {
    return mRawSocket.Receive(pBuff, length);
}

int
IPSocket::Send(const uint8_t* pBuff, int length) {
    return mRawSocket.SendTo(mDestIp, mDestPort, pBuff, length);
}

bool
IPSocket::IsValid() const {
    return true; // always valid
}
