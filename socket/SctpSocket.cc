// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "SctpSocket.hh"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netinet/sctp.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>

using namespace SocketProgramming;
using namespace std;

namespace {
    const int GETHOSTBYNAME_BUFLEN = 512;
}

SctpSocket::SctpSocket(const string& destIp, int destPort)
        : mLocalPort(-1),
          mSockfd(socket(PF_INET, SOCK_STREAM, IPPROTO_SCTP)),
          mDestIp(destIp),
          mDestPort(destPort) {
    mDestPort = htons(mDestPort);
    /* Enable ancillary data and notifications */
    struct sctp_event_subscribe events;
    memset((char*) &events, 1, sizeof(events));
    int ret =
            setsockopt(mSockfd, IPPROTO_SCTP, SCTP_EVENTS,
                       &events, sizeof(events));
    if (ret < 0) {
        // XXX error!
    }

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

SctpSocket::~SctpSocket() {
    close(mSockfd);
}

void
SctpSocket::SetDestination(const string& destIp, int destPort) {
    mDestIp = destIp;
    mDestPort = htons(destPort);
}

bool
SctpSocket::Bind(int port) {
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
    int ret = bind(mSockfd, (const sockaddr*) (&addr), sizeof(addr));
    if (ret < 0) {
        cerr << "Cannot bind socket to local port/addess = " << port << "/" <<
        "any" << endl;
        return false;
    }
    mLocalPort = htons(port);
    return true;
}

bool
SctpSocket::Connect() {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = mDestPort;

    hostent h;
    hostent* hp = &h;
    char buf[GETHOSTBYNAME_BUFLEN];
    int host_error = 0;
    gethostbyname_r(
            mDestIp.c_str(), &h, buf, GETHOSTBYNAME_BUFLEN - 1, &hp, &host_error);
    if (hp == 0) {
        // XXX error!
        // error = host_error
        return false;
    }

    // or use the following
    // n = inet_pton(AF_INET, mDestIp.c_str(), &addr.sin_addr);
    addr.sin_addr = *reinterpret_cast<in_addr*> (h.h_addr);

    const int ret = connect(mSockfd, (const sockaddr*) (&addr), sizeof(addr));
    if (ret < 0) {
        //int err = errno;
        // XXX throw an error
        return false;
    }
    return true;
}

bool
SctpSocket::Listen(int backlog) {
    int ret = listen(mSockfd, backlog);
    if (ret < 0) {
        // XXX error
        return false;
    }

    sockaddr_in addr;
    socklen_t addr_len = 0;
    ret = accept(mSockfd, (struct sockaddr*) &addr, &addr_len);
    if (ret < 0) {
        // XXX error
        return false;
    }

    return true;
}

int
SctpSocket::Receive(uint8_t* pBuff, int length) {
    struct sctp_sndrcvinfo sri;
    int flags = 0;
    int bytes_read =
            sctp_recvmsg(mSockfd, (void*) pBuff, (size_t) length,
                         (struct sockaddr*) 0, 0, &sri, &flags);
    if ((bytes_read < 0) && (errno != EAGAIN)) {
        cerr << "Something is very wrong for this RawIPSocket (fd=" << mSockfd <<
        "), errno:err=(" << errno << ":" << strerror(errno) << ")" << endl;
        return -1;
    }
    if (flags & MSG_NOTIFICATION) {
        // XXX handle notification
        return 0;
    }
    return bytes_read;
}

int
SctpSocket::Send(const uint8_t* pBuff, int length) {
    struct sctp_sndrcvinfo sri;
    memset(&sri, 0, sizeof(struct sctp_sndrcvinfo));

    int bytes_written =
            sctp_send(mSockfd, (void*) pBuff, (size_t) length, &sri, 0);
    return bytes_written;
}

bool
SctpSocket::IsValid() const {
    // XXX FIXME
    // Check when the connection is down
    return true;
}
