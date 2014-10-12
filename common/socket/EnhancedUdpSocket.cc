// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "EnhancedUdpSocket.hh"

#include <cerrno>
#include <cstdio>
#include <iostream>

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

using namespace SocketProgramming;
using namespace std;


EnhancedUdpSocket::EnhancedUdpSocket(const std::string& destIp, int destPort)
  : UdpSocket(destIp, destPort) {

  int opt = 1;
  if (setsockopt(mSockfd, IPPROTO_IP, IP_PKTINFO, &opt, sizeof(opt)) < 0)
  {
    const int err = errno;
    cerr << "setsockopt(): " << strerror(err);
    throw runtime_error(strerror(err));
  }
}

int
EnhancedUdpSocket::Send(const uint8_t* pBuff, int length, const std::string& destIp, int destPort) {
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(destPort);
  hostent* h = gethostbyname(destIp.c_str());
  addr.sin_addr = *reinterpret_cast<in_addr *> (h->h_addr);

  int bytes_written =
    sendto(mSockfd, (void*) pBuff, (size_t) length, MSG_DONTWAIT,
           (const sockaddr*)&addr, sizeof(addr));
  return bytes_written;
}

int
EnhancedUdpSocket::Receive(uint8_t* pBuff, int length, sockaddr_in* pSrcAddr, sockaddr_in* pDestAddr) {

  char cmbuf[0x100];  // control data
  struct iovec io;
  io.iov_base = pBuff; // udp payload
  io.iov_len = length;

  struct msghdr mh;
  mh.msg_name = pSrcAddr; // peer address
  mh.msg_namelen = sizeof(*pSrcAddr);
  mh.msg_control = cmbuf;
  mh.msg_controllen = sizeof(cmbuf);
  mh.msg_iov = &io;
  mh.msg_iovlen = 1;

  const int num_bytes = recvmsg(mSockfd, &mh, MSG_DONTWAIT);

  if (num_bytes < 0)
  {
    const int err = errno;
    switch (err) {
    case EAGAIN:
    case EINTR:
      break;
    default:
      cerr << "Could not recvfrom: " << strerror(err);
    }
    return num_bytes;
  }

  struct cmsghdr *cmsg = 0;

  for (cmsg = CMSG_FIRSTHDR(&mh); cmsg != 0; cmsg = CMSG_NXTHDR(&mh, cmsg))
  {
    // printf("cmsg->cmsg_level = %d, cmsg->cmsg_type=%d\n", cmsg->cmsg_level, cmsg->cmsg_type);
    if (cmsg->cmsg_level != IPPROTO_IP || cmsg->cmsg_type != IP_PKTINFO)
    {
      continue;
    }
    break;
  }

  // CHECK(cmsg != 0);
  struct in_pktinfo* pi = reinterpret_cast<struct in_pktinfo*>(CMSG_DATA(cmsg));
  //char ipi_spec_dst[256] = {0};
  //strcpy(ipi_spec_dst, inet_ntoa(pi->ipi_spec_dst));
  //char ipi_addr[256] = {0};
  //strcpy(ipi_addr, inet_ntoa(pi->ipi_addr));
  //printf("Packet received peeraddr_s=%s, [ipi_ifindex=%d, ipi_spec_dst=%s, ipi_addr=%s]\n", peeraddr_s, pi->ipi_ifindex, ipi_spec_dst, ipi_addr);
  // at this point, peeraddr is the source sockaddr
  // pi->ipi_ifindex is the interface index
  // pi->ipi_spec_dst is the destination in_addr
  // pi->ipi_addr is the receiving interface in_addr

  pDestAddr->sin_family = AF_INET;
  pDestAddr->sin_port = htons(mLocalPort);
  pDestAddr->sin_addr = pi->ipi_spec_dst;
  // optional
  // memset(pDestAddr->sin_zero, 0, sizeof(pDestAddr->sin_zero));
  return num_bytes;
}
