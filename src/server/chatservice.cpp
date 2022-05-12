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
/*
 * 获取消息id对应的处理函数
 */
MsgHandler ChatService::GetHandler(int msgId)
{
    auto it = msgHandlerMap_.find(msgId);
    if(it != msgHandlerMap_.end())
    {
        //如果找到了消息id对应的处理函数就把函数返回
        return msgHandlerMap_[msgId];
    }
    else
    {
        //没找到就返回一个空操作的处理函数
        return [=](const muduo::net::TcpConnectionPtr &conn, json &js, muduo::Timestamp timestamp)
        {
            LOG_ERROR<<"MsgId:"<<msgId<<" Can not find handler!";
        };
    }
}
/*
 * 注册消息及对应的业务处理
 */
ChatService::ChatService()
{
    //用户基本业务管理相关事件处理回调注册
    msgHandlerMap_.insert({LOGIN_MSG,std::bind(&ChatService::Login,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
    msgHandlerMap_.insert({REG_MSG,std::bind(&ChatService::Reg,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});

}

ChatService::~ChatService()
{

}