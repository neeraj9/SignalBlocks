// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "UdpSocket.h"

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

UdpSocket::UdpSocket(const string& destIp, int destPort)
        : mLocalPort(-1),
          mSockfd(socket(PF_INET, SOCK_DGRAM, 0)),
          mDestIp(destIp),
          mDestPort(destPort) {
    mDestPort = htons(mDestPort);
}

UdpSocket::~UdpSocket() {
    close(mSockfd);
}

void
UdpSocket::SetDestination(const string& destIp, int destPort) {
    mDestIp = destIp;
    mDestPort = htons(destPort);
}

bool
UdpSocket::Bind(int port) {
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
UdpSocket::Connect() {
    return true; // no connection required
}

bool
UdpSocket::Listen(int backlog) {
    return true; // no connection required
}

int
UdpSocket::Receive(uint8_t* pBuff, int length) {
    int bytes_read =
            recv(mSockfd, (void*) pBuff, (size_t) length, MSG_DONTWAIT);
    //recv(mSockfd, (void*) pBuff, (size_t) length, 0);
    if ((bytes_read < 0) && (errno != EAGAIN)) {
        cerr << "Something is very wrong for this UdpSocket (fd=" << mSockfd <<
        "), errno:err=(" << errno << ":" << strerror(errno) << ")" << endl;
    }
    return bytes_read;
}

int
UdpSocket::Send(const uint8_t* pBuff, int length) {
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
UdpSocket::IsValid() const {
    return true; // always valid
}
