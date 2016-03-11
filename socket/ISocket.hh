// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef ISocket_hh
#define ISocket_hh

#include <stdint.h>

namespace SocketProgramming
{
  class ISocket
  {
  public:
    virtual ~ISocket()
    {
    }

    virtual bool Bind(int localPort) = 0;
    virtual bool Connect() = 0;
    virtual bool Listen(int backlog) = 0;
    virtual int Receive(uint8_t* pBuff, int length) = 0;
    virtual int Send(const uint8_t* pBuff, int length) = 0;
    virtual bool IsValid() const = 0;
  };
}

#endif // ISocket_hh
