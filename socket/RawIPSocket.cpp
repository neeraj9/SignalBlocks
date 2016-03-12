// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "RawIPSocket.h"

#include "tap.h"

#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h> // for hostent
#include <netinet/in.h>
#include <iostream>

using namespace SocketProgramming;
using namespace std;

RawIPSocket::RawIPSocket()
        : mSockfd(-1) {
}

bool
RawIPSocket::Bind(const string& device) {
    if (mSockfd > 0) {
        cerr << "Cannot bind to device again" << endl;
        return false;
    }
    // the '1' is for promescious mode
    mSockfd = tap(device.c_str(), 1);
    //cout << "mSockfd = " << mSockfd << endl;
    if (mSockfd < 0) {
        return false;
    }
    return true;
}

int
RawIPSocket::Receive(uint8_t* pBuff, int length) {
    int bytes_read =
            recv(mSockfd, (void*) pBuff, (size_t) length, MSG_DONTWAIT);
    if ((bytes_read < 0) && (errno != EAGAIN)) {
        cerr << "Something is very wrong for this RawIPSocket (fd=" << mSockfd <<
        "), errno:err=(" << errno << ":" << strerror(errno) << ")" << endl;
    }
    return bytes_read;
}

int
RawIPSocket::SendTo(
        const string& destIp, int destPort, const uint8_t* pBuff, int length) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = destPort;
    hostent* h = gethostbyname(destIp.c_str());
    addr.sin_addr = *reinterpret_cast<in_addr*> (h->h_addr);

    int bytes_written =
            sendto(mSockfd, (void*) pBuff, (size_t) length, MSG_DONTWAIT,
                   (const sockaddr*) &addr, sizeof(addr));
    return bytes_written;
}
