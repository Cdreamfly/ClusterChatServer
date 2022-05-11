//
// Created by Cmf on 2022/5/11.
//

#include "server/chatserver.h"

ChatServer::ChatServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &addr, const std::string &msg)
        :server_(loop,addr,msg),loop_(loop)
{
    server_.setConnectionCallback(std::bind(&ChatServer::onConnection,this,std::placeholders::_1));
    server_.setMessageCallback(std::bind(&ChatServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    server_.setThreadNum(4);
}

void ChatServer::Start()
{
    server_.start();
}

ChatServer::~ChatServer() {}

void ChatServer::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf,muduo::Timestamp timestamp)
{

}
void ChatServer::onConnection(const muduo::net::TcpConnectionPtr &conn)
{

}