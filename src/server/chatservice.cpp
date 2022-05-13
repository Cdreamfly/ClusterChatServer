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
/*
 * 登录
 */
void ChatService::Login(const muduo::net::TcpConnectionPtr &conn, json &js, muduo::Timestamp timestamp)
{
    int id = js["id"].get<int>();
    std::string pwd = js["pwd"];
    User user = userModel_.Query(id);
    //如果id和密码正确
    if(user.getId() == id && user.getPwd() == pwd)
    {
        //如果已经是登录状态
        if(user.getState() == "online")
        {
            json res;
            res["msg-id"] = LOGIN_MSG_ACK;
            res["errno"] = 2;
            res["errmsg"] = "this account is using, input another!";
            conn->send(res.dump());
        }
        else
        {
            //不是则登录
            {
                //记录用户连接信息，要考虑线程安全，因为多线程访问
                std::lock_guard<std::mutex> lk(mtx_);
                userConnMap_.insert({id,conn});
            }
            //更新用户状态信息 state offline => online
            user.setState("online");
            userModel_.UpdateState(user);
            json res;
            res["msg-id"] = LOGIN_MSG_ACK;
            res["errno"] = 0;
            res["id"] = user.getId();
            res["name"] = user.getName();
            //查询是否有离线消息
            std::vector<std::string>vec = offlineMsgModel_.Query(id);
            if(!vec.empty())
            {
                res["offlinemsg"] = vec;
                offlineMsgModel_.Remove(id);
            }
            conn->send(res.dump());
        }
    }
    else
    {
        //不正确或找不到的返回
        json res;
        res["msg-id"] = LOGIN_MSG_ACK;
        res["errno"] = 1;
        res["errmsg"] = "id or password is invalid!";
        conn->send(res.dump());
    }
}
/*
 * 注册
 */
void ChatService::Reg(const muduo::net::TcpConnectionPtr &conn, json &js, muduo::Timestamp timestamp)
{
    //获取客户端的注册信息创建对象
    User user(-1,js["name"],js["pwd"]);
    if(userModel_.Insert(user))
    {
        //插入新用户成功
        json res;
        res["msg-id"] = REG_MSG_ACK;
        res["errno"] = 0;
        res["id"] = user.getId();
        conn->send(res.dump());
    }
    else
    {
        //插入新用户失败
        json res;
        res["msg-id"] = REG_MSG_ACK;
        res["errno"] = 1;
        res["id"] = user.getId();
        conn->send(res.dump());
    }
}
/*
 * 私聊
 */
void ChatService::oneChat(const muduo::net::TcpConnectionPtr &conn, json &js, muduo::Timestamp timestamp)
{
    int toid = js["to-id"].get<int>();
    {
        std::lock_guard<std::mutex> lk(mtx_);
        auto it = userConnMap_.find(toid);
        if(it != userConnMap_.end())
        {
            //如果在线就发送消息
            it->second->send(js.dump());
        }
    }
    //不在线就存储离线消息
    offlineMsgModel_.Install(toid,js.dump());
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
    msgHandlerMap_.insert({ONE_CHAT_MSG,std::bind(&ChatService::oneChat,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
}

ChatService::~ChatService()
{

}
/*
 * 找到异常退出的链接删了它并设置为离线状态
 */
void ChatService::clientCloseException(const muduo::net::TcpConnectionPtr &conn)
{
    User user;
    {
        std::lock_guard<std::mutex>lk(mtx_);
        for(auto it = userConnMap_.begin();it!=userConnMap_.end();++it)
        {
            if(it->second == conn)
            {
                user.setId(it->first);
                user.setState("offline");
                userConnMap_.erase(it);
                break;
            }
        }
    }
    if(user.getId() != -1)
    {
        userModel_.UpdateState(user);
    }
}
