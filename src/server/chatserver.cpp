//
// Created by Cmf on 2022/5/11.
//

#include "server/chatserver.h"
#include "server/chatservice.h"
#include <iostream>

ChatServer::ChatServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &addr, const std::string &msg)
        :server_(loop,addr,msg),loop_(loop)
{
    //和注册链接回调
    server_.setConnectionCallback(std::bind(&ChatServer::onConnection,this,std::placeholders::_1));
    //注册消息回调
    server_.setMessageCallback(std::bind(&ChatServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    //设置线程数量
    server_.setThreadNum(4);
}
/*
 * 启动服务
 */
void ChatServer::Start()
{
    server_.start();
}

ChatServer::~ChatServer() {}
/*
 * 上报读写回调函数，收到消息进行处理
 */
void ChatServer::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf,muduo::Timestamp timestamp)
{
    //读取所有消息转成字符串
    std::string str = buf->retrieveAllAsString();
    std::cout<<str<<std::endl;
    //反序列化
    json js = json::parse(str);
    //通过json获取业务处理模块事先绑定的业务处理函数
    auto msgHandler = ChatService::Instance().GetHandler(js["msg-id"].get<int>());
    //执行这个业务
    msgHandler(conn,js,timestamp);
}
/*
 * 上报链接相关的回调函数
 */
void ChatServer::onConnection(const muduo::net::TcpConnectionPtr &conn)
{
    //客户端断开链接
    if(!conn->connected())
    {
        //客户端异常退出处理
        ChatService::Instance().clientCloseException(conn);
        //关闭文件描述符
        conn->shutdown();
    }
}