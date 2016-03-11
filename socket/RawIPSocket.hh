// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef RawIPSocket_hh
#define RawIPSocket_hh

#include <stdint.h>
#include <string>

namespace SocketProgramming
{
  /*
   * You either need to be a root or run the main process as sudo
   * to open a raw socket.
   */
  class RawIPSocket
  {
  public:
    RawIPSocket();
    bool Bind(const std::string& device);
    int Receive(uint8_t* pBuff, int length);
    int SendTo(const std::string& destIp, int destPort, const uint8_t* pBuff, int length);

  private:
    int mPort;
    int mSockfd;
  };
}

#endif // RawIPSocket_hh
