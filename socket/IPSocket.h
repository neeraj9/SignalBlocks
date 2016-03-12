// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef IPSocket_hh
#define IPSocket_hh

#include "ISocket.hh"
#include "RawIPSocket.hh"

#include <stdint.h>
#include <string>

namespace SocketProgramming {
    class IPSocket
            : public ISocket {
    public:
        IPSocket(const std::string& destIp, int destPort);

        virtual ~IPSocket();

        void SetDestination(const std::string& destIp, int destPort);

    public: // ISocket interface
        bool Bind(int port);

        bool Connect(); // dummy in case of udp
        bool Listen(int backlog); // dummy in case of udp
        int Receive(uint8_t* pBuff, int length);

        int Send(const uint8_t* pBuff, int length);

        bool IsValid() const;

    private:
        // mLocalPort here is just used to check for previous Bind() call.
        int mLocalPort; // in network byte order
        RawIPSocket mRawSocket;
        std::string mDestIp;
        int mDestPort; // in network byte order
    };
}

#endif // IPSocket_hh
