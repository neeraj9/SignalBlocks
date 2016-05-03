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

#include "TcpSocket.h"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>

using namespace SocketProgramming;
using namespace std;

namespace {
    const int GETHOSTBYNAME_BUFLEN = 512;
}

TcpSocket::TcpSocket(const string& destIp, int destPort)
        : mLocalPort(-1),
          mSockfd(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)),
          mDestIp(destIp),
          mDestPort(htons(destPort)),
          mIsConnected(false) {

#if 0 // if we need non-blocking
    ret = fcntl(s, F_GETFL);
    if (ret < 0)
    {
      // XXX error
    }
    else
    {
      ret = fcntl(s, F_SETFL, ret | O_NONBLOCK);
      if (ret < 0)
      {
        // XXX error
      }
    }
#endif // non-blocking
}

TcpSocket::~TcpSocket() {
    close(mSockfd);
}

void
TcpSocket::SetDestination(const string& destIp, int destPort) {
    mDestIp = destIp;
    mDestPort = htons(destPort);
}

bool
TcpSocket::Bind(int port) {
    if ((mLocalPort > 0) && (port != mLocalPort)) {
        cerr << "Cannot bind same socket to another port" << endl;
        return false;
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    // hostent* h = gethostbyname("server.ip.address");
    //addr.sin_addr = *reinterpret_cast<in_addr *> (h->h_addr);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = ::bind(mSockfd, (const sockaddr*) (&addr), sizeof(addr));
    if (ret < 0) {
        cerr << "Cannot bind socket to local port/addess = " << port << "/" <<
        "any, errno:err=(" << errno << ":" << strerror(errno) << ")" << endl;
        return false;
    }
    mLocalPort = htons(port);
    return true;
}

bool
TcpSocket::Connect() {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = mDestPort; // mDestPort is already in network byte order

    hostent h;
    hostent* hp = &h;
    char buf[GETHOSTBYNAME_BUFLEN];
    int host_error = 0;
#if defined __APPLE__ && defined __MACH__
    hp = gethostbyname(mDestIp.c_str());
#else
    gethostbyname_r(
            mDestIp.c_str(), &h, buf, GETHOSTBYNAME_BUFLEN - 1, &hp, &host_error);
#endif
    if (hp == 0) {
        cerr << "Cannot find hostname for host = " << mDestIp
        << ", host_error=" << host_error << endl;
        // error = host_error
        return false;
    }

    // or use the following
    // n = inet_pton(AF_INET, mDestIp.c_str(), &addr.sin_addr);
    addr.sin_addr = *reinterpret_cast<in_addr*> (hp->h_addr);

    const int ret = connect(mSockfd, (const sockaddr*) (&addr), sizeof(addr));
    if (ret < 0) {
        // XXX throw an error
        cerr << "Cannot connect to port/addess = " << ntohs(mDestPort) << "/"
        << mDestIp << "any, errno:err=(" << errno
        << ":" << strerror(errno) << ")" << endl;
        return false;
    }
    mIsConnected = true;
    return true;
}

bool
TcpSocket::Listen(int backlog) {
    int ret = listen(mSockfd, backlog);
    if (ret < 0) {
        cerr << "Cannot listen on TcpSocket (fd=" << mSockfd <<
        "), errno:err=(" << errno << ":" << strerror(errno) << ")" << endl;
        return false;
    }

    sockaddr_in addr;
    socklen_t addr_len = 0;
    ret = accept(mSockfd, (struct sockaddr*) &addr, &addr_len);
    if (ret < 0) {
        cerr << "Cannot accept tcp cxn, TcpSocket (fd=" << mSockfd <<
        "), errno:err=(" << errno << ":" << strerror(errno) << ")" << endl;
        return false;
    }

    mIsConnected = true;
    return true;
}

int
TcpSocket::Receive(uint8_t* pBuff, int length) {
    int flags = 0;
    int bytes_read =
            recv(mSockfd, (void*) pBuff, (size_t) length, 0);
    if ((bytes_read < 0) && (errno != EAGAIN)) {
        cerr << "Something is very wrong for this TcpSocket (fd=" << mSockfd <<
        "), errno:err=(" << errno << ":" << strerror(errno) << ")" << endl;
        return -1;
    }
    return bytes_read;
}

int
TcpSocket::Send(const uint8_t* pBuff, int length) {
    int bytes_written =
            send(mSockfd, (void*) pBuff, (size_t) length, 0);
    return bytes_written;
}

bool
TcpSocket::IsValid() const {
    // XXX FIXME
    // Check when the connection is down
    return mIsConnected;
}
