// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef TcpSocket_hh
#define TcpSocket_hh

#include "ISocket.hh"

#include <stdint.h>
#include <string>

namespace SocketProgramming
{
  class TcpSocket
    : public ISocket
  {
  public:
    TcpSocket(const std::string& destIp, int destPort);
    virtual ~TcpSocket();
    void SetDestination(const std::string& destIp, int destPort);

  public: // ISocket interface
    bool Bind(int port);
    bool Connect();
    bool Listen(int backlog);
    int Receive(uint8_t* pBuff, int length);
    int Send(const uint8_t* pBuff, int length);
    bool IsValid() const;

  private:
    int mLocalPort; // in network byte order
    int mSockfd;
    std::string mDestIp;
    int mDestPort; // in network byte order
    bool mIsConnected;
  };
}

#endif // TcpSocket_hh
