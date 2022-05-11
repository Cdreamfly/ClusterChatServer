//
// Created by Cmf on 2022/5/11.
//
#include "server/chatservice.h"
#include "muduo/base/Logging.h"

ChatService &ChatService::Instance()
{
    static ChatService service;
    return service;
}

void ChatService::Login(const muduo::net::TcpConnectionPtr &conn, json &js, muduo::Timestamp timestamp)
{
    LOG_INFO<<"Do Login Service!";
}
void ChatService::Reg(const muduo::net::TcpConnectionPtr &conn, json &js, muduo::Timestamp timestamp)
{
    LOG_INFO<<"Do Login Service!";
}

MsgHandler ChatService::GetHandler(int msgId)
{
    auto it = msgHandlerMap_.find(msgId);
    if(it != msgHandlerMap_.end())
    {
        return msgHandlerMap_[msgId];
    }
    else
    {
        return [=](const muduo::net::TcpConnectionPtr &conn, json &js, muduo::Timestamp timestamp)->void
        {
            LOG_ERROR<<"MsgId:"<<msgId<<" Can not find handler!";
        };
    }
}

ChatService::ChatService()
{
    msgHandlerMap_.insert({LOGIN_MSG,std::bind(&ChatService::Login,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
    msgHandlerMap_.insert({REG_MSG,std::bind(&ChatService::Reg,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});

}

ChatService::~ChatService()
{

}