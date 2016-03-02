// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
// copyright (c) 2010-14 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE.txt for license information
#ifndef EnhancedUdpSocket_hh
#define EnhancedUdpSocket_hh

#include "UdpSocket.hh"

#include <netinet/in.h>
#include <stdint.h>
#include <string>

namespace SocketProgramming
{
  class EnhancedUdpSocket
    : public UdpSocket
  {
  public:
    EnhancedUdpSocket(const std::string& destIp, int destPort);

  public: // ISocket interface
    /// non-blocking read
    int Receive(uint8_t* pBuff, int length, sockaddr_in* pSrcAddr, sockaddr_in* pDestAddr);
    int Send(const uint8_t* pBuff, int length, const std::string& destIp, int destPort);
  };
}

#endif // EnhancedUdpSocket_hh
