// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "TcpServerSocket.h"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

using namespace SocketProgramming;
using namespace std;

namespace {
    const int GETHOSTBYNAME_BUFLEN = 512;
}

TcpServerSocket::TcpServerSocket()
        : mLocalPort(-1),
          mListeningSockfd(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)),
          mDataSockfd(-1),
          mDestAddr({0}) {
    // Make socket non-blocking
    int ret = fcntl(mListeningSockfd, F_GETFL);
    if (ret < 0) {
        // XXX error
    }
    else {
        ret = fcntl(mListeningSockfd, F_SETFL, ret | O_NONBLOCK);
        if (ret < 0) {
            // XXX error
        }
    }
}

TcpServerSocket::~TcpServerSocket() {
    close(mListeningSockfd);
    if (mDataSockfd > 0) {
        close(mDataSockfd);
    }
}

bool
TcpServerSocket::Bind(int port) {
    if (mLocalPort > 0) {
        cerr << "Cannot bind same socket to another port" << endl;
        return false;
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    // hostent* h = gethostbyname("server.ip.address");
    //addr.sin_addr = *reinterpret_cast<in_addr *> (h->h_addr);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(mListeningSockfd, (const sockaddr*) (&addr), sizeof(addr));
    if (ret < 0) {
        cerr << "Cannot bind socket to local port/addess = " << port << "/" <<
        "any" << endl;
        return false;
    }
    mLocalPort = htons(port);
    return true;
}

bool
TcpServerSocket::Listen(int backlog) {
    int ret = listen(mListeningSockfd, backlog);
    if (ret < 0) {
        // XXX error
        return false;
    }
    return true;
}

bool
TcpServerSocket::Accept(struct sockaddr* from,
                        socklen_t* fromlen) {
    int ret = accept(mListeningSockfd, from, fromlen);
    if (ret < 0) {
        // XXX error
        return false;
    }

    mDataSockfd = ret;
    return true;
}

int
TcpServerSocket::Receive(uint8_t* pBuff, int length,
                         struct sockaddr* from,
                         socklen_t* fromlen) {
    int flags = 0;
    int bytes_read =
            recv(mDataSockfd, (void*) pBuff, (size_t) length, 0);
    if ((bytes_read < 0) && (errno != EAGAIN)) {
        cerr << "Something is very wrong for this TCPServerSocket (fd=" << mDataSockfd <<
        "), errno:err=(" << errno << ":" << strerror(errno) << ")" << endl;
        return -1;
    }
    memcpy(from, &mDestAddr, sizeof(struct sockaddr_in));
    *fromlen = sizeof(struct sockaddr_in);
    return bytes_read;
}

int
TcpServerSocket::Send(const uint8_t* pBuff, int length,
                      struct sockaddr* to,
                      socklen_t tolen) {
    // XXX check if "to" is same as mDestAddr.
    int bytes_written =
            send(mDataSockfd, (void*) pBuff, (size_t) length, 0);
    return bytes_written;
}

bool
TcpServerSocket::IsValid() const {
    // XXX FIXME
    // Check when the connection is down
    return true;
}

bool
TcpServerSocket::IsConnected() {
    // XXX FIXME
    // check when the connection goes down
    return (mDataSockfd > 0);
}
