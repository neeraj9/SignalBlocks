// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_SOCKETSINK_H
#define SIGBLOCKS_SOCKETSINK_H

#include "../../common/Port.h"

#include <memory>

namespace SocketProgramming {
    class ISocket;
}

namespace sigblocks {
    class SocketSink
            : public Port<1, 0, unsigned char> {
    public:
        SocketSink(std::string name);

        SocketSink(std::string name, std::unique_ptr<SocketProgramming::ISocket> pIns);

        void SetStreamSink(std::unique_ptr<SocketProgramming::ISocket> pIns);

    public: // Port interface
        void Process(int sourceIndex, const unsigned char& data, const TimeTick& startTime);

        void Process(
                int sourceIndex, std::unique_ptr<unsigned char[]> data, int len, const TimeTick& startTime);

    private:
        std::unique_ptr<SocketProgramming::ISocket> mpSocket;
    };
}

#endif // SIGBLOCKS_SOCKETSINK_H
