//
// Created by Cmf on 2022/5/11.
//

#include "server/chatserver.h"
#include "server/chatservice.h"
#include <iostream>

ChatServer::ChatServer(EventLoop *loop, const InetAddress &addr, const std::string &msg)
        : _server(loop, addr, msg), _loop(loop) {
    //和注册链接回调
    _server.SetConnectionCallback(std::bind(&ChatServer::onConnection, this, std::placeholders::_1));
    //注册消息回调
    _server.SetMessageCallback(std::bind(&ChatServer::onMessage, this, std::placeholders::_1, std::placeholders::_2,
                                         std::placeholders::_3));
    //设置线程数量
    _server.SetThreadNum(4);
}

/*
 * 启动服务
 */
void ChatServer::Start() {
    _server.Start();
}

ChatServer::~ChatServer() {}

/*
 * 上报读写回调函数，收到消息进行处理
 */
void ChatServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp timestamp) {
    //读取所有消息转成字符串
    std::string str = buf->RetrieveAllAsString();
    std::cout << str << "size:" << str.size() << std::endl;
    //反序列化
    json js = json::parse(str);
    std::cout << js << std::endl;
    //通过json获取业务处理模块事先绑定的业务处理函数
    auto msgHandler = ChatService::Instance().GetHandler(js[Agreement::MSG_ID].get<EnMsgType>());
    //执行这个业务
    msgHandler(conn, js, timestamp);
}

/*
 * 上报链接相关的回调函数
 */
void ChatServer::onConnection(const TcpConnectionPtr &conn) {
    //客户端断开链接
    if (!conn->Connect()) {
        //客户端异常退出处理
        ChatService::Instance().clientCloseException(conn);
        //关闭文件描述符
        conn->Shutdown();
    }
}