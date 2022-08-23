//
// Created by Cmf on 2022/5/11.
//

#ifndef CLUSTERCHAT_CHATSERVER_H
#define CLUSTERCHAT_CHATSERVER_H

#include <functional>
#include "CmfNet/net/TcpServer.hpp"
#include "CmfNet/net/EventLoop.hpp"

/**
 * 网路模块
 */
class ChatServer {
public:
    ChatServer(EventLoop *loop, const InetAddress &addr, const std::string &msg);

    void Start();

    ~ChatServer();

private:
    //上报链接相关回调函数
    void onConnection(const TcpConnectionPtr &conn);

    //上报读写相关回调函数
    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp timestamp);

private:
    TcpServer _server;
    EventLoop *_loop;
};

#endif //CLUSTERCHAT_CHATSERVER_H