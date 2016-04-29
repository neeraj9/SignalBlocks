// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "UdpLiteSocket.h"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>

using namespace SocketProgramming;
using namespace std;

namespace {
    // An alternate to updating /usr/include/netinet/in.h
    const int MY_IPPROTO_UDPLITE = 136; // UDP-Lite (RFC 3828)

    // An alternate to installing udplite.h in /usr/include/netinet
    /*
     * Socket options for UDP Lite, see set|getsockopt(2).
     */
    const int SOL_UDPLITE = 136;         /* Sockopt level  */
    /*
     * The following socket options set the checksum coverage lengths.
     * Sender   checksum coverage sets the coverage to exactly the value passed to
     *          sockopt (given a valid value is passed).
     * Receiver checksum coverage is a _minimum_ checksum coverage: all incoming
     *          packets not matching this value will be discarded.
     * See also RFC 3828.
     */
    const int UDPLITE_SEND_CSCOV = 10;
    /* Actual coverage length      */
    const int UDPLITE_RECV_CSCOV = 11;   /* Minimum acceptable coverage */
}

UdpLiteSocket::UdpLiteSocket(const string& destIp, int destPort,
                             int sendCoverageLength, int recvCoverageLength)
        : mLocalPort(-1),
          mSockfd(socket(PF_INET, SOCK_DGRAM, MY_IPPROTO_UDPLITE)),
          mDestIp(destIp),
          mDestPort(destPort),
          mSendCoverage(sendCoverageLength),
          mRecvCoverage(recvCoverageLength) {
    mDestPort = htons(mDestPort);
#if 0
    int status =
      setsockopt(mSockfd, SOL_UDPLITE, UDPLITE_SEND_CSCOV,
                 &mSendCoverage, sizeof(int));
    if (!status)
    {
      cerr << "UdpLite send coverage setting failed!" << endl;
    }
    status =
      setsockopt(mSockfd, SOL_UDPLITE, UDPLITE_RECV_CSCOV,
                 &mRecvCoverage, sizeof(int));
    if (!status)
    {
      cerr << "UdpLite receive coverage setting failed!" << endl;
    }
#endif
}

UdpLiteSocket::~UdpLiteSocket() {
    close(mSockfd);
}

void
UdpLiteSocket::SetDestination(const std::string& destIp, int destPort) {
    mDestIp = destIp;
    mDestPort = htons(destPort);
}

bool
UdpLiteSocket::Bind(int port) {
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
    int ret = ::bind(mSockfd, (const sockaddr*) (&addr), sizeof(addr));
    if (ret < 0) {
        cerr << "Cannot bind socket to local port/addess = " << port << "/" <<
        "any" << endl;
        return false;
    }
    mLocalPort = htons(port);
    return true;
}

bool
UdpLiteSocket::Connect() {
    return true; // no connection required
}

bool
UdpLiteSocket::Listen(int backlog) {
    return true; // no connection required
}

int
UdpLiteSocket::Receive(uint8_t* pBuff, int length) {
    int bytes_read =
            recv(mSockfd, (void*) pBuff, (size_t) length, MSG_DONTWAIT);
    if ((bytes_read < 0) && (errno != EAGAIN)) {
        cerr << "Something is very wrong for this UdpLiteSocket (fd=" << mSockfd <<
        "), errno:err=(" << errno << ":" << strerror(errno) << ")" << endl;
    }
    return bytes_read;
}

int
UdpLiteSocket::Send(const uint8_t* pBuff, int length) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = mDestPort;
    hostent* h = gethostbyname(mDestIp.c_str());
    addr.sin_addr = *reinterpret_cast<in_addr*> (h->h_addr);

    int bytes_written =
            sendto(mSockfd, (void*) pBuff, (size_t) length, MSG_DONTWAIT,
                   (const sockaddr*) &addr, sizeof(addr));
    return bytes_written;
}

bool
UdpLiteSocket::IsValid() const {
    return true; // always valid
}
