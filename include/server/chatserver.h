//
// Created by Cmf on 2022/5/11.
//

#ifndef CLUSTERCHAT_CHATSERVER_H
#define CLUSTERCHAT_CHATSERVER_H

#include <functional>
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"

class ChatServer
{
public:
    ChatServer(muduo::net::EventLoop*loop,const muduo::net::InetAddress&addr,const std::string &msg);
    void Start();
    ~ChatServer();
private:
    void onConnection(const muduo::net::TcpConnectionPtr &conn);
    void onMessage(const muduo::net::TcpConnectionPtr&conn,muduo::net::Buffer*buf,muduo::Timestamp timestamp);
private:
    muduo::net::TcpServer server_;
    muduo::net::EventLoop* loop_;
};
#endif //CLUSTERCHAT_CHATSERVER_H
