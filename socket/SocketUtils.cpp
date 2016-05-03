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

#include "SocketUtils.h"

#include "../common/logging.h"

#include <arpa/inet.h>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>

using namespace sigblocks;


bool SocketUtils::MakeNonBlockingSocket(int fd) {
    // Make socket non-blocking
    int ret = fcntl(fd, F_GETFL);
    if (ret < 0) {
        int err = errno;
        LOG_ERROR("fcntl(F_GETFL) failed, errno=%d\n", err);
        return false; // error
    }
    else {
        ret = fcntl(fd, F_SETFL, ret | O_NONBLOCK);
        if (ret < 0) {
            int err = errno;
            LOG_ERROR("fcntl(F_SETFL) failed, errno=%d\n", err);
            return false;  // error
        }
    }
    return true;
}

bool SocketUtils::BindToPort(int fd, int port) {
    if ((fd < 0) || (port <= 0)){
        LOG_ERROR("Cannot bind socket fd=%d to port=%d\n", fd, port);
        return false;
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(fd, (const sockaddr*) (&addr), sizeof(addr));
    if (ret < 0) {
        LOG_ERROR("Cannot bind socket to local port/addess=%d/any\n", port);
        return false;
    }
    return true;
}

bool SocketUtils::SetTcpNoDelay(int sockfd) {
    int op = 1;
    int rc = setsockopt(sockfd, SOL_TCP, TCP_NODELAY, &op, sizeof(op));
    if (rc < 0) {
        LOG_DEBUG("setsockopt(TCP_NODELAY) failed\n");
        return false;
    }
    return true;
}

int SocketUtils::AcceptTcpConnection(int listenFd) {
    struct sockaddr client_addr;
    socklen_t client_addr_len;

    client_addr_len = sizeof(client_addr);
    int clientfd = accept(listenFd, &client_addr, &client_addr_len);
    if (clientfd < 0) {
        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
            /* this should not happen but lets handle it as well */
            return -1;
        } else {
            LOG_ERROR("accept(), errno=%d\n", errno);
            return -1;
        }
    }
    bool success = MakeNonBlockingSocket(clientfd);
    if (!success) {
        LOG_ERROR("Cannot make clientfd=%d non-blocking, so closing connection.\n", clientfd);
        close(clientfd);
        return -1;
    }
    LOG_DEBUG("clientfd %d attempted connect.\n", clientfd);

    char hbuf[NI_MAXHOST];
    char sbuf[NI_MAXSERV];

    int rc = getnameinfo(&client_addr, client_addr_len,
                         hbuf, sizeof(hbuf),
                         sbuf, sizeof(sbuf),
                         NI_NUMERICHOST | NI_NUMERICSERV);
    if (rc == 0) {
        LOG_DEBUG("Accepted connection on sockfd %d, clientfd = %d"
                          " (host=%s, port=%s)\n",
                  listenFd, clientfd, hbuf, sbuf);
    }

    /* Use TCP_NODELAY to disable nagle algorithm and send the
     * packets as soon as possible. This will ensure that the
     * tcp stack doesnt do any buffering and maintain the latency
     * requirement. Additionally, this will ensure that a client
     * which is keeping the connection alive gets the responses
     * immediately.
     */
    success = SetTcpNoDelay(clientfd);
    if (!success) {
        LOG_ERROR("Cannot disable nagle algorithm on clientfd=%d, so closing connection.\n",
                  clientfd);
        close(clientfd);
        return -1;
    }
    return clientfd;
}
