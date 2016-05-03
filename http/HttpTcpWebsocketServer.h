// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// This file is part of SignalBlocks.
//
// SignalBlocks is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SignalBlocks is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SignalBlocks.  If not, see <http://www.gnu.org/licenses/>.
//

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
