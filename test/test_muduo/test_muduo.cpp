#include <iostream>
#include <string>
#include <functional>
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"

class ChatServer
{
public:
    ChatServer(muduo::net::EventLoop*loop,const muduo::net::InetAddress&addr,const std::string &msg)
    :server_(loop,addr,msg),loop_(loop)
    {
        server_.setConnectionCallback(std::bind(&ChatServer::onConnection,this,std::placeholders::_1));
        server_.setMessageCallback(std::bind(&ChatServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        server_.setThreadNum(4);
    }
    void Start()
    {
        server_.start();
    }
    ~ChatServer()
    {

    }
private:
    void onConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        if(conn->connected())
        {
            std::cout<<conn->peerAddress().toIpPort()<<"->"<<conn->localAddress().toIpPort()<<"State:Online"<<std::endl;
        }
        else
        {
            std::cout<<conn->peerAddress().toIpPort()<<"->"<<conn->localAddress().toIpPort()<<"State:Online"<<std::endl;
            conn->shutdown();
        }
    }
    void onMessage(const muduo::net::TcpConnectionPtr&conn,muduo::net::Buffer*buf,muduo::Timestamp timestamp)
    {
        std::string message = buf->retrieveAllAsString();
        std::cout << "recv data:" << message << "time:" << timestamp.toFormattedString() << std::endl;
        conn->send(message);
    }
private:
    muduo::net::TcpServer server_;
    muduo::net::EventLoop* loop_;
};


int main() {
    muduo::net::EventLoop loop;
    muduo::net::InetAddress addr("127.0.0.1",9190);
    ChatServer server(&loop,addr,"ChatServer");
    server.Start();
    loop.loop();
    return 0;
}
