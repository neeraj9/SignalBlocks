// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef IClientSocket_hh
#define IClientSocket_hh

#include <stdint.h>
#include <sys/socket.h>

namespace SocketProgramming {
    class IClientSocket {
    public:
        virtual ~IClientSocket() {
        }

        virtual bool Bind(int localPort) = 0;

        virtual bool Connect(struct sockaddr* to, socklen_t tolen) = 0;

        virtual int Receive(uint8_t* pBuff,
                            int length,
                            struct sockaddr* from,
                            socklen_t* fromlen) = 0;

        virtual int Send(const uint8_t* pBuff,
                         int length) = 0;

        virtual int Send(const uint8_t* pBuff,
                         int length,
                         struct sockaddr* to,
                         socklen_t tolen) = 0;

        virtual bool IsValid() = 0;
    };
}

#endif // IClientSocket_hh
