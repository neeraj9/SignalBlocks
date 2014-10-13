// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_source_SocketSourceCreator_hh
#define sigblocks_blocks_source_SocketSourceCreator_hh

#include "SocketSource.hh"

#include "../../common/IPort.hh"
#include "../../common/TimeTick.hh"
#include "../../common/socket/ISocket.hh"

#include <memory>
#include <string>

namespace sigblocks
{
  template <class X>
  class SocketSourceCreator
  {
  public:
    static SocketSource* Create(
      const TimeTick& startTime,
      TimeTick& increment,
      int blockSize,
      const std::string& destIp,
      int destPort,
      int localPort)
    {
      SocketSource* socket_source =
        new SocketSource(startTime, increment, blockSize);
      std::auto_ptr<SocketProgramming::ISocket> pIns(new X(destIp, destPort));
      if (pIns->Bind(localPort))
      {
        if (pIns->Listen(1)) // TODO: Note for TCP of connection oriented
                            // protocols this will stop to get one client
                            // connection and only then do the rest of the tasks.
                            // Think about Lazy initialization here (how about a new thread
                            // while listens)!
        {
          // save only if successful
          socket_source->SetStreamSource(pIns);
        }
        else
        {
          // cerr TODO
        }
        return socket_source;
      }
      else
      {
        // cerr TODO
      }
      return socket_source;
    }
  };
}

#endif // sigblocks_blocks_source_SocketSourceCreator_hh
