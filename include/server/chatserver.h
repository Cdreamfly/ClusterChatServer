//
// Created by Cmf on 2022/5/11.
//

#ifndef CLUSTERCHAT_CHATSERVER_H
#define CLUSTERCHAT_CHATSERVER_H

#include <functional>
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"

/**
 * 网路模块
 */
class ChatServer
{
public:
    ChatServer(muduo::net::EventLoop*loop,const muduo::net::InetAddress&addr,const std::string &msg);
    void Start();
    ~ChatServer();
private:
    //上报链接相关回调函数
    void onConnection(const muduo::net::TcpConnectionPtr &conn);
    //上报读写相关回调函数
    void onMessage(const muduo::net::TcpConnectionPtr&conn,muduo::net::Buffer*buf,muduo::Timestamp timestamp);
private:
    muduo::net::TcpServer _server;
    muduo::net::EventLoop* _loop;
};
#endif //CLUSTERCHAT_CHATSERVER_H
