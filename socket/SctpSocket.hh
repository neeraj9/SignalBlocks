// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SctpSocket_hh
#define SctpSocket_hh

#include "ISocket.hh"

#include <stdint.h>
#include <string>

namespace SocketProgramming
{
  class SctpSocket
    : public ISocket
  {
  public:
    SctpSocket(const std::string& destIp, int destPort);
    virtual ~SctpSocket();
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
  };
}

#endif // SctpSocket_hh
