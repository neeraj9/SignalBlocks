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

#include "HttpTcpWebsocketServer.h"
#include "../common/logging.h"
#include "../common/sha1.h"
#include "../common/base64.h"
#include "../socket/SocketUtils.h"

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

using namespace signalblocks;

namespace {
    // setting for light loaded system
    constexpr int TCP_BACKLOG = 10;
    constexpr int MAX_EPOLL_EVENTS = 10;
    constexpr int MAX_EVENT_TIMEOUT_MILLISECONDS = 100;
    // sleep for 2 seconds when no client is connected primarily
    // to allow caller decide to keep running or terminate
    // gracefully.
    constexpr int ZERO_CLIENT_MAX_EVENT_TIMEOUT_MILLISECONDS = 2000;

    const std::string HTTP_GET_METHOD_NAME = std::string("GET");

    int AddSocketToEpoll(int efd, int sockfd, bool isEdgeTriggered) {
        struct epoll_event event;
        int ret = 0;

        event.data.fd = sockfd;
        if (!isEdgeTriggered) {
            event.events = EPOLLIN | EPOLLERR | EPOLLHUP;    /* level triggered */
        } else {
            event.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET;    /* edge triggered */
        }
        ret = epoll_ctl(efd, EPOLL_CTL_ADD, sockfd, &event);
        if (ret < 0) {
            int err = errno;
            char errbuf[256];
            strerror_r(err, errbuf, sizeof(errbuf));
            LOG_ERROR("epoll_ctl(EPOLL_CTL_ADD) failed, errno=%d,"
                              " errmsg=%s", err, errbuf);
            return -1;
        }
        return 0;
    }

    int DeleteSocketFromEpoll(int efd, int sockfd) {
        struct epoll_event event;
        int ret = 0;

        /* support kernel prior to 2.6.9 and pass non-null event pointer */
        ret = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, &event);
        if (ret < 0) {
            int err = errno;
            char errbuf[256];
            strerror_r(err, errbuf, sizeof(errbuf));
            LOG_ERROR("epoll_ctl(EPOLL_CTL_DEL) failed, errno=%d,"
                              " errmsg=%s", err, errbuf);
            return -1;
        }
        return 0;
    }

    ssize_t SendData(int fd, const std::string& msg) {
        return write(fd, msg.c_str(), msg.size());
    }

    ssize_t SendWebsocketData(int fd, const std::string& msg) {

        // see RFC-6455 sec 5.2 Base Framing Protocol
        constexpr int MAX_WEBSOCKET_HEADER_BYTES = 32;
        char websocket_header[MAX_WEBSOCKET_HEADER_BYTES];
        size_t websocket_header_bytes = 0;
        websocket_header[0] = static_cast<uint8_t>(0x81); // or 129 (bits: 1000 0001)

        if (msg.size() <= 125) {
            websocket_header[1] = (uint8_t) msg.size();
            websocket_header_bytes = 2;
        } else if (msg.size() >= 0x7e && msg.size() <= 0xffff) {
            websocket_header[1] = 0x7e;  // or 126
            websocket_header[2] = static_cast<uint8_t>((msg.size() >> 8) & 0x00ff);
            websocket_header[3] = static_cast<uint8_t>(msg.size() & 0x00ff);
            websocket_header_bytes = 4;
        } else {
            websocket_header[1] = 0x7f;  // or 127
            websocket_header[2] = static_cast<uint8_t>((msg.size() >> 56) & 0xff);
            websocket_header[3] = static_cast<uint8_t>((msg.size() >> 48) & 0xff);
            websocket_header[4] = static_cast<uint8_t>((msg.size() >> 40) & 0xff);
            websocket_header[5] = static_cast<uint8_t>((msg.size() >> 32) & 0xff);
            websocket_header[6] = static_cast<uint8_t>((msg.size() >> 24) & 0xff);
            websocket_header[7] = static_cast<uint8_t>((msg.size() >> 16) & 0xff);
            websocket_header[8] = static_cast<uint8_t>((msg.size() >> 8) & 0xff);
            websocket_header[9] = static_cast<uint8_t>((msg.size()) & 0xff);
            websocket_header_bytes = 10;
        }

        //LOG_DEBUG("websocket_header_bytes=%lu, size(msg)=%lu\n", websocket_header_bytes, msg.size());
        //printf("websocket_header=[");
        //for (size_t i = 0; i < websocket_header_bytes; ++i) {
        //    printf("0x%02x, ", websocket_header[i]);
        //}
        //printf("]\n");
        int iovcnt = 2;
        struct iovec iov[iovcnt];
        iov[0].iov_base = websocket_header;
        iov[0].iov_len = websocket_header_bytes;
        iov[1].iov_base = const_cast<char*>(msg.c_str());
        iov[1].iov_len = msg.size();

        return writev(fd, iov, iovcnt);
    }

    inline int SkipSpaces(char* buf, int maxbytes, int offset) {
        while ((buf[offset] == ' ' || buf[offset] == '\t') && (offset < maxbytes)) {
            ++offset;
        }
        return offset;
    }

    inline int FindCharOffset(char* buf, int maxbytes, int offset, char ch) {
        while ((buf[offset] != ch) && (offset < maxbytes)) {
            ++offset;
        }
        return offset;
    }
}


HttpTcpWebsocketServer::ConnectionContext::ConnectionContext()
        : mFd(-1),
          mRoute(),
          mIsRequestReceived(false),
          mIsFirst(true),
          mTimeTick() {
}

HttpTcpWebsocketServer::HttpTcpWebsocketServer(int port)
        : mPort(port),
          mSockFd(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)),
          mEpollFd(epoll_create(1)),
          mpEvents(new struct epoll_event[MAX_EPOLL_EVENTS]),
          mRoutes(),
          mConnections() {
    assert(mSockFd > 0);
    assert(mEpollFd > 0);
    assert(SocketUtils().MakeNonBlockingSocket(mSockFd));
    assert(SocketUtils().BindToPort(mSockFd, mPort));
    int backlog = TCP_BACKLOG;
    assert(listen(mSockFd, backlog) == 0);

    constexpr bool is_edge_triggered = false;
    assert(AddSocketToEpoll(mEpollFd, mSockFd, is_edge_triggered) == 0);

    // reset mpEvents
    std::memset(mpEvents.get(), 0, sizeof(struct epoll_event) * MAX_EPOLL_EVENTS);
}

HttpTcpWebsocketServer::~HttpTcpWebsocketServer() {
    close(mSockFd);
    close(mEpollFd);
}

bool HttpTcpWebsocketServer::AddRoute(const std::string& route, JsonDataCallbackFuncType cb) {
    auto iter = mRoutes.find(route);
    if (iter != mRoutes.end()) {
        LOG_ERROR("Cannot add duplicate route=%s\n", route.c_str());
        return false;
    }
    auto p = mRoutes.insert(std::make_pair(route, cb));
    return p.second;
}

bool HttpTcpWebsocketServer::RemoveRoute(const std::string& route) {
    auto iter = mRoutes.find(route);
    if (iter == mRoutes.end()) {
        LOG_ERROR("Cannot delete non-existent route=%s\n", route.c_str());
        return false;
    }
    mRoutes.erase(iter);
    return true;
}

void HttpTcpWebsocketServer::Run() {
    // Dont wait infinitely when no client is connected
    // primarily to allow the caller control on a repeated
    // basis even though no clients are connected.
    // This is useful for many cases including the scenario where
    // the caller wants to terminate (based on some flag or event).
    int timeout_milliseconds = ZERO_CLIENT_MAX_EVENT_TIMEOUT_MILLISECONDS;
    if (! mConnections.empty()) {
        timeout_milliseconds = MAX_EVENT_TIMEOUT_MILLISECONDS;
    }
    int num_events = epoll_wait(
            mEpollFd, mpEvents.get(), MAX_EPOLL_EVENTS, timeout_milliseconds);
    if (num_events > 0) {
        int rc = HandleEvents(num_events);
        if (rc < 0) {
            return;
        }
    }
    // push data to connected clients
    for (auto&& iter: mConnections) {
        if (iter.second.mIsRequestReceived) {
            auto&& routeiter = mRoutes.find(iter.second.mRoute);
            if (routeiter != mRoutes.end()) {
                // found a route
                std::string json_data;
                TimeTick t;
                std::tie(json_data, t) = routeiter->second(iter.second.mIsFirst, iter.second.mTimeTick);
                if (!json_data.empty()) {
                    iter.second.mTimeTick = t;
                    iter.second.mIsFirst = false;
                    // send to client
                    int clientfd = iter.first;
                    ssize_t bytes_sent = SendWebsocketData(clientfd, json_data);
                    if ((bytes_sent < 0) || (bytes_sent < static_cast<ssize_t>(json_data.size()))) {
                        LOG_ERROR("cannot send data to clientfd=%d (bytes_sent=%ld out of %lu), so close connection\n",
                                  clientfd, bytes_sent, json_data.size());
                        RemoveConnection(clientfd);
                    }
                }
            }
        }
    }
}

int HttpTcpWebsocketServer::HandleEvents(int numEvents) {
    for (int i = 0; i < numEvents; ++i) {
        if (mpEvents[i].data.fd == mSockFd) { // listening fd
            if ((mpEvents[i].events & EPOLLERR) ||
                (mpEvents[i].events & EPOLLHUP) ||
                (!(mpEvents[i].events & EPOLLIN))) {
                // fatal error!
                LOG_ERROR("bail out and terminate the thread,"
                                  "epoll events=0x%0x\n",
                          mpEvents[i].events);
                return -1;
            } else {
                // accept new connection
                LOG_DEBUG("Accept a new connection attempt\n");
                AcceptNewConnection();
            }
        } else { // clientfd
            auto iter = mConnections.find(mpEvents[i].data.fd);
            if (iter != mConnections.end()) {
                int clientfd = mpEvents[i].data.fd;
                if ((mpEvents[i].events & EPOLLERR) ||
                    (mpEvents[i].events & EPOLLHUP) ||
                    (!(mpEvents[i].events & EPOLLIN))) {
                    // fatal error!
                    LOG_ERROR("Connection closed with clientfd=%d,"
                                      "epoll events=0x%0x\n",
                              mpEvents[i].data.fd, mpEvents[i].events);
                    RemoveConnection(clientfd);
                } else {
                    // process message from the client
                    ProcessClientRequest(clientfd);
                }
            }
        }
    }
    return numEvents;
}

void HttpTcpWebsocketServer::AcceptNewConnection() {

    int clientfd = SocketUtils().AcceptTcpConnection(mSockFd);
    if (clientfd < 0) {
        LOG_ERROR("cannot accept tcp conection on fd=%d\n", mSockFd);
        return;  // error
    }

    auto p = mConnections.insert(std::make_pair(clientfd, ConnectionContext()));
    if (!p.second) {
        LOG_WARN("ConnectionContext already exists for clientfd=%d, so overwrite it.",
                 clientfd);
    }
    p.first->second.mFd = clientfd;
    p.first->second.mIsRequestReceived = false;
    p.first->second.mIsFirst = true;
    
    constexpr bool is_edge_triggered = false;
    int rc = AddSocketToEpoll(mEpollFd, clientfd, is_edge_triggered);
    if (rc < 0) {
        LOG_ERROR("Cannot wait for events on clientfd=%d, so closing connection.\n",
                  clientfd);
        mConnections.erase(p.first);
        close(clientfd);
    }
}

void HttpTcpWebsocketServer::RemoveConnection(int fd) {
    close(fd);
    mConnections.erase(fd);
}

void HttpTcpWebsocketServer::ProcessClientRequest(int clientfd) {
    constexpr int MAX_BUFFER_LEN = 4096;
    char buffer[MAX_BUFFER_LEN];
    ssize_t bytes_received = read(clientfd, buffer, MAX_BUFFER_LEN);
    LOG_DEBUG("clientfd=%d received %lu bytes\n", clientfd, bytes_received);
    if (bytes_received <= 0) {
        LOG_ERROR("Connection with clientfd=%d closed.\n", clientfd);
        RemoveConnection(clientfd);
    } else {
        int offset = 0;
        int http_method_start_offset = SkipSpaces(buffer, bytes_received, offset);
        LOG_DEBUG("http_method_start_offset=%d\n", http_method_start_offset);
        int space_offset = FindCharOffset(buffer, bytes_received, http_method_start_offset, ' ');
        LOG_DEBUG("space_offset=%d\n", space_offset);
        // TODO validate that space_offset >= http_method_start_offset
        std::string method_name(&buffer[http_method_start_offset], &buffer[space_offset]);
        LOG_DEBUG("method_name=%s\n", method_name.c_str());
        if (HTTP_GET_METHOD_NAME != method_name) {
            LOG_ERROR("clientfd=%d cannot handle HTTP method=%s, so close connection\n",
                      clientfd, method_name.c_str());
            RemoveConnection(clientfd);
        } else {
            int path_start_offset = SkipSpaces(buffer, bytes_received, space_offset);
            LOG_DEBUG("path_start_offset=%d\n", path_start_offset);
            space_offset = FindCharOffset(buffer, bytes_received, path_start_offset, ' ');
            LOG_DEBUG("space_offset=%d\n", space_offset);
            // TODO validate that space_offset >= path_start_offset
            std::string url_path(&buffer[path_start_offset], &buffer[space_offset]);
            LOG_DEBUG("clientfd=%d, requested http method_name=%s, url_path=%s\n",
                      clientfd, method_name.c_str(), url_path.c_str());
            auto iter = mConnections.find(clientfd);
            if (iter != mConnections.end()) {
                // found connection
                iter->second.mIsRequestReceived = true;
                if (iter->second.mRoute != url_path) {
                    iter->second.mRoute.swap(url_path);
                    iter->second.mIsFirst = true;  // reset

                    // look for websocket client
                    // TODO error checking for misbehaving clients
                    std::string ws_key_header("Sec-WebSocket-Key");
                    char* loc = strstr(buffer, ws_key_header.c_str());
                    int ws_key_value_start_offset = FindCharOffset(buffer, bytes_received, loc - buffer, ':');
                    ++ws_key_value_start_offset;  // skip over ':'
                    ws_key_value_start_offset = SkipSpaces(buffer, bytes_received, ws_key_value_start_offset);
                    int ws_key_value_end_offset = FindCharOffset(buffer, bytes_received, ws_key_value_start_offset, '\r');
                    std::string sec_websocket_key(&buffer[ws_key_value_start_offset], &buffer[ws_key_value_end_offset]);
                    LOG_DEBUG("sec_websocket_key=[%s]\n", sec_websocket_key.c_str());
                    // compute the Sec-Websocket-Accept value (see wikipedia)
                    // https://en.wikipedia.org/wiki/WebSocket
                    sec_websocket_key.append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
                    uint8_t hash[20];
                    sha1::calc(sec_websocket_key.c_str(), sec_websocket_key.length(), hash);
                    std::string sec_websocket_accept_value = base64::Base64::base64_encode(hash, 20);
                    LOG_DEBUG("sec_websocket_accept_value=[%s]\n", sec_websocket_accept_value.c_str());

                    LOG_DEBUG("buffer=[%s]\n", buffer);
                    std::string response = "HTTP/1.1 101 Switching Protocols\r\n"
                            "Upgrade: websocket\r\n"
                            "Connection: Upgrade\r\n"
                            "Sec-WebSocket-Accept: ";
                    response += sec_websocket_accept_value;
                    response += "\r\n\r\n";
                    LOG_DEBUG("response=[%s]\n", response.c_str());
                    SendData(clientfd, response);
                }
            } else {
                LOG_ERROR("clientfd=%d cannot find connection information, so close connection.\n",
                          clientfd);
                close(clientfd);
            }
        }
    }
}
