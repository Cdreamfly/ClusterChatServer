//
// Created by Cmf on 2022/5/11.
//

#ifndef CLUSTERCHAT_CHATSERVICE_H
#define CLUSTERCHAT_CHATSERVICE_H

#include <unordered_map>
#include <functional>
#include "muduo/net/TcpConnection.h"
#include "json.hpp"
#include "public.h"

using json = nlohmann::json;
using MsgHandler = std::function<void(const muduo::net::TcpConnectionPtr&,json&, muduo::Timestamp)>;

class ChatService
{
public:
    static ChatService&Instance();
    void Login(const muduo::net::TcpConnectionPtr&conn,json&js, muduo::Timestamp timestamp);
    void Reg(const muduo::net::TcpConnectionPtr&conn,json&js, muduo::Timestamp timestamp);
    MsgHandler GetHandler(int msgId);
    ~ChatService();
private:
    ChatService();
private:
    std::unordered_map<int,MsgHandler> msgHandlerMap_;
};

#endif //CLUSTERCHAT_CHATSERVICE_H
