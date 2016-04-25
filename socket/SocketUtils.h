// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#ifndef SIGNALBLOCKS_SOCKETUTILS_H
#define SIGNALBLOCKS_SOCKETUTILS_H


namespace sigblocks {

    class SocketUtils {
    public:
        bool MakeNonBlockingSocket(int fd);
        bool BindToPort(int fd, int port);
        bool SetTcpNoDelay(int sockfd);
        int AcceptTcpConnection(int listenFd);
    };
}

#endif //SIGNALBLOCKS_SOCKETUTILS_H
