// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#ifndef SIGNALBLOCKS_HTTPTCPWEBSOCKETSERVER_H
#define SIGNALBLOCKS_HTTPTCPWEBSOCKETSERVER_H

#include "../common/CallbackTypes.h"
#include "../common/TimeTick.h"

#include <functional>
#include <memory>
#include <string>
#include <sys/epoll.h>
#include <tuple>
#include <unordered_map>

namespace sigblocks {
    class HttpTcpWebsocketServer {
    public:
        HttpTcpWebsocketServer(int port);
        ~HttpTcpWebsocketServer();

        bool AddRoute(const std::string& route, JsonDataCallbackFuncType cb);
        bool RemoveRoute(const std::string& route);

        // Run once and return when the work is complete.
        // Note that this method will not block and no threads are created
        // internally. Any thread management must be done externally (if required).
        void Run();

    private:
        int HandleEvents(int numEvents);
        void AcceptNewConnection();
        void RemoveConnection(int fd);
        void ProcessClientRequest(int clientfd);

    private:
        struct ConnectionContext {

            ConnectionContext();

            int mFd;
            std::string mRoute;
            bool mIsRequestReceived;
            bool mIsFirst;
            TimeTick mTimeTick;
        };

    private:
        const int mPort;
        int mSockFd;
        int mEpollFd;
        std::unique_ptr<struct epoll_event[]> mpEvents;
        std::unordered_map<std::string, JsonDataCallbackFuncType> mRoutes;
        std::unordered_map<int, ConnectionContext> mConnections;
    };
}

#endif //SIGNALBLOCKS_HTTPTCPWEBSOCKETSERVER_H
